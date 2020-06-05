#include <stdlib.h>
#include <stdio.h>

double radec2Gl( const double ra, const double dec);
double radec2Gb( const double ra, const double dec);


static void
print_usage(void)
{
  static char progname[]="EQ2GA";
  static char verid[]="v.1.0";

  printf("%s %s - Usage:\n", progname, verid);
  printf("   eq2ga  RA Dec\n");
  printf("   Input values are in fractional degrees.\n");
  exit(0);
}

/* The entry point for the program */

int
main(int argc, char *argv[])
{
  double di1deg, di2deg;

  int args = 1;
  double l_deg, b_deg;

  if (argc < 3)
    print_usage();

  di1deg = atof(argv[args++]);
  di2deg = atof(argv[args++]);

  l_deg = radec2Gl(di1deg, di2deg);
  b_deg = radec2Gb(di1deg, di2deg);
  printf("l=%12.7lf b=%12.7lf\n", l_deg, b_deg);

  return 0;
}
