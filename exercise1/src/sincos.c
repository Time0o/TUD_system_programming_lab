#include <math.h>
#include <stdio.h>

#include "sincos.h"

#define LIMIT 360.0
#define COLUMN 10

void
print_sincos (long steps)
{
  double stepsize = LIMIT / (double) steps;

  for (double d = 0.0; d <= 360.0; d += stepsize)
    {
      double s = sin (2.0 * M_PI * (d / 360.0));
      double c = cos (2.0 * M_PI * (d / 360.0));

      printf ("%-*.3f %-*.3f %-*.3f\n", COLUMN, d, COLUMN, s, COLUMN, c);
    }
}
