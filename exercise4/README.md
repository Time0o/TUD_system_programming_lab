# Exercise 4

Some security exploit exercises. A password is extracted from a program's
object file and then the program's buffer overflow vulnerability is exploited.
The program in question can be obtained from
[here](https://www.inf.tu-dresden.de/content/institutes/sya/os/teaching/programming/simple_login).


# Extracting the password

```
>$ readelf -S simple_login | grep -P "Name|rodata"
[Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
[15] .rodata           PROGBITS        08048554 000554 000078 00   A  0   0  4

>$ hexdump -C -s 0x554 -n 0x78 simple_login
00000554  03 00 00 00 01 00 02 00  25 73 00 41 61 4d 69 6c  |........%s.AaMil|
00000564  6d 46 53 2b 7a 31 31 2f  32 4a 00 45 6e 74 65 72  |mFS+z11/2J.Enter|
00000574  20 74 68 65 20 63 6f 72  72 65 63 74 20 70 61 73  | the correct pas|
00000584  73 77 6f 72 64 3a 00 4e  69 63 65 20 74 72 79 21  |sword:.Nice try!|
00000594  00 00 00 00 53 75 63 63  65 73 73 66 75 6c 20 6c  |....Successful l|
000005a4  6f 67 69 6e 21 20 4e 6f  77 2c 20 77 65 20 77 6f  |ogin! Now, we wo|
000005b4  75 6c 64 20 65 78 65 63  75 74 65 20 61 20 73 68  |uld execute a sh|
000005c4  65 6c 6c 20 2e 2e 2e 00                           |ell ....|
000005cc

>$ ./simple_login
Enter the correct password:
AaMilmFS+z11/2J
Successful login! Now, we would execute a shell ...
```

# Buffer Overflow exploitation

The vulnerability arises from the call to `scanf` inside the `verify_password`
function. Compile the cracker program with `make` and crack the program by
running `./bin/cracker 8048473 | ./simple_login`.

We must be careful to overwrite the saved base pointer with a value such
that `-0x18(%ebp)` is a readable and writeable memory address (because it is
accessed by `main` after the call to `verify_password`).

This only works (on Linux) if address space randomization is not enabled i.e.
if `/proc/sys/kernel/randomize_va_space` contains the value `0` because
otherwise it becomes impossible to reliably guess a valid stack address for the
saved base pointer and the program will segfault after printing the success
message (the text segment's location is typically not randomized so this does
not affect the jump to the target address).

This can be demonstrated by first running (with ASLR enabled):

```
>$ ./bin/cracker 08048473 | ./bin/simple_login
Enter the correct password:
Successful login! Now, we would execute a shell ...
Segmentation fault (core dumped)
```

And then running:

```
>$ ./bin/cracker 08048473 | setarch $(uname -m) --addr-no-randomize ./bin/simple_login
Enter the correct password:
Successful login! Now, we would execute a shell ...
```

In principle it would still be possible to write a large number of NOP's
followed by arbitrary malicious code into the buffer such that we can exploit
the program in some other way (e.g. by starting a shell) if we can guess a
return address that points anywhere in the corresponding stack section. Of
course this only works if the stack marked as an executable region which is not
the case for `simple_login` as can be verified by running
`readelf -l simple_login | grep GNU_STACK`.

So on systems that use ASLR we would have to use more advanced methods such as
_Return Oriented Programming_ if we still wanted to exploit the program.

To further guard against buffer overflow exploits, the `simple_login` binary
could have been compiled with the `-fstack-protector` option. This adds random
canary integer values on the stack before function return addresses as well as
code that aborts the program if they are changed by buffer overflows.
