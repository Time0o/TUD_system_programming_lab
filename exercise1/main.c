#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sincos.h"

#define USAGE_MSG "Usage: %s [STEPS]\n"

#define HELP_MSG \
  "Print values of sin/cos for angles between 0 and 360 degrees in STEPS increments.\n" \
  "STEPS must be a whole number which is greater than 0.\n"

static char *prog_name;

static void
usage (int status)
{
  if (status != EXIT_SUCCESS)
    fprintf (stderr, "Try '%s --help' for more information.\n", prog_name);
  else
    {
      printf (USAGE_MSG, prog_name);
      printf (HELP_MSG);
    }

  exit (status);
}

int
main (int argc, char **argv)
{
  prog_name = argv[0];

  static struct option const long_options[] =
  {
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0}
  };

  int c;
  while ((c = getopt_long (argc, argv, "h", long_options, NULL)) != -1)
    {
      switch (c)
        {
        case 'h':
          usage (EXIT_SUCCESS);
        default:
          usage (EXIT_FAILURE);
        }
    }

  if (argc - optind != 1)
    {
      fprintf (stderr, "%s: should receive a single STEPS argument\n",
               prog_name);

      usage (EXIT_FAILURE);
    }

  char *endptr = NULL;
  long steps = strtol (argv[optind], &endptr, 10);

  if (endptr != (argv[optind] + strlen (argv[optind])))
    {
      fprintf (stderr, "%s: '%s' is not a number\n", prog_name, argv[optind]);
      usage (EXIT_FAILURE);
    }
  else if (errno != 0)
    {
      fprintf (stderr, "%s: failed to parse STEPS argument: %s\n", prog_name,
               strerror (errno));

      usage (EXIT_FAILURE);
    }

  if (steps < 1)
    {
      fprintf (stderr, "%s: STEPS must be greater than 0\n", prog_name);
      usage (EXIT_FAILURE);
    }

  print_sincos (steps);

  return EXIT_SUCCESS;
}
