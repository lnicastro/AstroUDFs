/*
 * TESTING myepoch_coords ...
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


void myepoch_coords(const double ra, const double de, const double pmra, const double pmde,
                const double epoch0, const double epoch1, double *ra1, double *de1);


int main(int argc, char *argv[])
{
  double mjd0,mjd1=0., ra,de, ra1,de1, pmra,pmde;
  char s[20];

// Groombridge 1830
  ra = 11.88299133 * 15.;
  de = 37.71867646;
  pmra = 4003.27;
  pmde = -5815.07;

  mjd0 = 51544.5;  // J2000.0

  if (argc == 1) {
	printf( "MJD: " );
	if (fgets(s, 20, stdin))
	  sscanf( s, "%lf", &mjd1 );
  } else 
	sscanf( argv[1], "%lf", &mjd1 );
 
  if (mjd1 == 0.) exit(0);

  printf("mjd0,mjd1, ra,de, pmra,pmde: %lf %lf %lf %lf  %lf %lf\n", mjd0,mjd1, ra,de, pmra,pmde);

  myepoch_coords(ra,de, pmra,pmde, mjd0, mjd1, &ra1,&de1);

  printf("RA, Dec: %lf %lf\n", ra1, de1);

  ra = 2.530301028 * 15.;
  de = 89.264109444;
  pmra = 44.22;
  pmde = -11.75;

  printf("\nra,de, pmra,pmde: %lf %lf  %lf %lf\n", ra,de, pmra,pmde);

  myepoch_coords(ra,de, pmra,pmde, mjd0, mjd1, &ra1,&de1);

  printf("RA, Dec: %lf %lf\n\n", ra1, de1);

  ra = 10.715944806 * 15.;
  de = -64.394450;
  pmra = -18.87;
  pmde = 12.06;

  printf("\nra,de, pmra,pmde: %lf %lf  %lf %lf\n", ra,de, pmra,pmde);

  myepoch_coords(ra,de, pmra,pmde, mjd0, mjd1, &ra1,&de1);

  printf("RA, Dec: %lf %lf\n\n", ra1, de1);

  return 0;
}
