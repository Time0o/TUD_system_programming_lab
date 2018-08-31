# Exercise 3

A simple shell supporting commands with the following syntax:

```
command <args> [< input_redirect] (| command <args>)* [> output_redirect] [&]
```

as well as the following builtins:

* `cd`
* `pwd`
* `kill <signo> <pid>`
* `alias/unalias`

Some features implemented by the shell that are not (directly) part of the
exercise task specification include:

* automatic lookup of programs in directories that are part of `$PATH`

* execution of scripts starting with a shebang

* wildcard expansion (not exhaustively tested)

* `SIGINT`, `SIGTSTP` and `SIGQUIT` handling

# Installation

To build and execute the shell run `make` followed by `./bin/shell`.

This should only require the following:

* GNU `make`
* recent versions of `gcc` and `g++`, the latter must support C++11
* `flex` and `bison`
* the GNU `readline` and Linux `util` libraries
