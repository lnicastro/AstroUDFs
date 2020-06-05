#include <stdlib.h>
#include <stdio.h>

double radec2El(const double ra, const double dec);
double radec2Eb(const double ra, const double dec);

static void
print_usage(void)
{
  static char progname[]="EQ2EC";
  static char verid[]="v.1.0";

  printf("%s %s - Usage:\n", progname, verid);
  printf("   eq2ec  RA Dec\n");
  printf("   Input values are in fractional degrees.\n");
  exit(0);
}

/* The entry point for the program */

int
main(int argc, char *argv[])
{
  double di1deg, di2deg, l_deg, b_deg;

  int args = 1;

  if (argc < 3)
    print_usage();

  di1deg = atof(argv[args++]);
  di2deg = atof(argv[args++]);

  l_deg = radec2El(di1deg, di2deg);
  b_deg = radec2Eb(di1deg, di2deg);
  printf("l=%12.7lf b=%12.7lf\n", l_deg, b_deg);

  return 0;
}
