#include <dirent.h>
#include <errno.h>
#include <fnmatch.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define USAGE_MSG "Usage: %s <directory name> [-name <pattern>] [-type <f | d>] [-follow] [-xdev]\n"

#define HELP_MSG "Recursively print files in directory <directory name>.\n\n" \
                 "  -name <pattern>  only consider files matching <pattern>\n" \
                 "  -type <f|d>      only consider regular files (f) / directories (d)\n" \
                 "  -follow          follow symbolic links\n" \
                 "  -xdev            do not cross file system boundaries\n"

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

static void
find (char *dir, char *pattern, int f, int d, int follow, int xdev, dev_t dev)
{
  DIR *dird = opendir (dir);
  if (!dird)
    {
      fprintf (stderr, "%s: %s\n", prog_name, strerror (errno));
      exit (EXIT_FAILURE);
    }

  for (;;)
    {
      struct dirent *dirent = readdir (dird);
      if (!dirent)
        {
          closedir (dird);

          if (errno == 0)
            return;

          fprintf (stderr, "%s: %s\n", prog_name, strerror (errno));
          exit (EXIT_FAILURE);
        }

      if (strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0)
        continue;

      int match = 0;
      if (pattern)
        {
          int flags = 0;
          switch (fnmatch (pattern, dirent->d_name, flags))
            {
            case 0:
              match = 1;
              break;
            case FNM_NOMATCH:
              break;
            default:
              fprintf (stderr, "%s: fnmatch failed\n", prog_name);
              closedir (dird);
              exit (EXIT_FAILURE);
            }
        }
      else
        match = 1;

      char path[4096];
      snprintf (path, sizeof(path), "%s/%s", dir, dirent->d_name);

      int ignore = (f == 0 && dirent->d_type == DT_REG)
                || (d == 0 && dirent->d_type == DT_DIR);

      if (match && !ignore)
        printf ("%s\n", path);

      if (dirent->d_type == DT_DIR || (dirent->d_type == DT_LNK && follow))
        {
          if (xdev)
            {
              struct stat sb;
              stat(path, &sb);

              if (sb.st_dev != dev)
                continue;
            }

          find (path, pattern, f, d, follow, xdev, dev);
        }
    }
}

int
main (int argc, char **argv)
{
  prog_name = argv[0];

  static struct option const long_options[] =
  {
    {"follow", no_argument, NULL, 'f'},
    {"help", no_argument, NULL, 'h'},
    {"name", required_argument, NULL, 'n'},
    {"type", required_argument, NULL, 't'},
    {"xdev", no_argument, NULL, 'x'},
    {NULL, 0, NULL, 0}
  };

  char *pattern;
  int f, d, follow, xdev;

  pattern = NULL;
  f = d = 1;
  follow = xdev = 0;

  int c;
  while ((c = getopt_long_only (argc, argv, "h", long_options, NULL)) != -1)
    {
      switch (c)
        {
        case 'f':
          follow = 1;
          break;
        case 'h':
          usage (EXIT_SUCCESS);
        case 'n':
          pattern = malloc(strlen(optarg) + 1);
          strcpy(pattern, optarg);
          break;
        case 't':
          if (strcmp (optarg, "f") == 0)
            d = 0;
          else if (strcmp (optarg, "d") == 0)
            f = 0;
          else
            {
              fprintf (stderr, "%s: argument to 'type' should be 'f' or 'd'\n",
                       prog_name);

              usage (EXIT_FAILURE);
            }
          break;
        case 'x':
          xdev = 1;
          break;
        default:
          usage (EXIT_FAILURE);
        }
    }

  if (!argv[optind] || argv[optind + 1])
    {
      fprintf (stderr, "%s: should receive single mandatory directory argument\n",
               prog_name);

      usage (EXIT_FAILURE);
    }

  char *dir = argv[optind];

  if (dir[strlen(dir) - 1] == '/')
    dir[strlen(dir) - 1] = '\0';

  struct stat sb;
  stat(dir, &sb);
  dev_t dev = sb.st_dev;

  find (dir, pattern, f, d, follow, xdev, dev);

  free (pattern);

  return EXIT_SUCCESS;
}
