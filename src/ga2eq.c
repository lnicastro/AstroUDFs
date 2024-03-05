#include <stdlib.h>
#include <stdio.h>

int GlGb2radec(const double gl, const double gb, double *ra, double *dec);


static void
print_usage(void)
{
  static char progname[]="GA2EQ";
  static char verid[]="v.1.0";

  printf("%s %s - Usage:\n", progname, verid);
  printf("   ga2eq  Gl Gb\n");
  printf("   Input values are in fractional degrees.\n");
  exit(0);
}

/* The entry point for the program */

int
main(int argc, char *argv[])
{
  double di1deg, di2deg;

  int args = 1;
  double ra_deg, dec_deg;

  if (argc < 3)
    print_usage();

  di1deg = atof(argv[args++]);
  di2deg = atof(argv[args++]);

  GlGb2radec(di1deg, di2deg, &ra_deg, &dec_deg);
  printf("RA=%12.7lf Dec=%12.7lf\n", ra_deg, dec_deg);
  return 0;
}
