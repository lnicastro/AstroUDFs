/*
  Utility functions.


  LN@INAF-OAS, March 2007  ( Last change: 05/06/2020 )
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include <string.h>
//#include <ctype.h>

const static double PI = 3.1415926535897932384626433;
const static double PI2 = 6.2831853071795864769252867665590057683943387987502;

/* degrees to radians */
const static double DEG2RAD = 1.74532925199432957692369E-2;


/*
  Haversine formula (R.W Sinnot, 1984) for distance of 2 objects on the sphere.

  Input coords in "degrees", returned value in "arcmin". 
*/

double skysep_h(double theta1, double phi1, double theta2, double phi2)
{
  double a1r, b1r, a2r, b2r, radif, sin2a, sin2d;

  if ( theta1 < 0. || theta2 < 0. )
	return -1.;

  a1r = theta1*DEG2RAD;
  b1r = phi1*DEG2RAD;
  a2r = theta2*DEG2RAD;
  b2r = phi2*DEG2RAD;

  radif  = fabs(a2r-a1r);
  if (radif > M_PI)
	radif = PI2 - radif;
  sin2a = sin(radif/2.);
  sin2a *= sin2a;
  sin2d = sin((b2r-b1r)/2.);
  sin2d *= sin2d;
  return (2 * asin( sqrt(sin2d + cos(b1r)*cos(b2r)*sin2a) )/DEG2RAD * 60.);
}


/* arcsin with range check */
double asine(const double arg)
{
  if ( arg >= 1. )
	return(PI / 2);
   if ( arg <= -1. )
	return(-PI / 2.);

   return (asin(arg));
}


/* Vector precession given rotation matrix */
int precess_vector(const double *matrix, const double *v1, double *v2)
{
  int i = 3;

  while (i--)
  {
      *v2++ = matrix[0] * v1[0] + matrix[1] * v1[1] + matrix[2] * v1[2];
      matrix += 3;
  }

  return 0;
}


/*
  Name: char *enc_str_rah

  Purpose:
    Encode into a string of the format "hh:mm:ss.ss" a right ascension
    passed as fractional hours.

  Input paramenters:
    rahr (double): RA in hours
    separator (char*) if passed and not NULL, it will be used in place of ":"


   LN@INAF-OAS, July 2003                   ( Last change: 05/06/2020 )
*/

char *enc_str_rah(double rahr, const char* separator)
{
  unsigned int rah=0,ram=0;
  float rafs=0.;
  double ra;
  char sep[] = {":"}; 
  static char str_ra[13];

  if ( separator )
	sep[0] = *separator;

  ra  = rahr;
  while (ra > 24.) ra -= 24;
  while (ra < 0.) ra += 24;

  rah = (unsigned int)(ra);
  ra -= rah;
  ra *= 60.;
  ram = (unsigned int)(ra);
  ra -= ram;
  rafs = ra*60.;

  sprintf(str_ra,"%2.2d%1s%2.2d%1s%05.2f",rah, sep, ram, sep, rafs);
//printf("%f  %s\n",rahr, str_ra);
  return (str_ra);
}


/*
  Name: char *enc_str_radeg

  Purpose:
    Encode into a string of the format "hh:mm:ss.ss" a right ascension
    passed as fractional degrees.

  Input paramenters:
    radeg (double): RA in degrees
    separator (char*) if passed and not NULL, it will be used in place of ":"


   LN@INAF-OAS, July 2003                   ( Last change: 28/05/2020 )
*/

char *enc_str_radeg(double radeg, const char* separator)
{
  return enc_str_rah(radeg/15., separator);
}


/*
   Name: char *enc_str_decdeg

    Purpose:
      Encode into a string of the format "±dd:mm:ss.s" a declination
      passed as fractional degrees.

  Input paramenters:
    rdecdeg (double): Dec in decimal degrees
    separator (char*) if passed and not NULL, it will be used in place of ":"


   LN@INAF-OAS, July 2003                   ( Last change: 05/06/2020 )
*/

char *enc_str_decdeg(double decdeg, const char* separator)
{
  int decd;
  unsigned int decm;
  float decfs;
  double dec;
  char sign[] = {"+"}, sep[] = {":"};
  static char str_dec[13];

  if ( separator )
        sep[0] = *separator;

  dec = decdeg;
  while (dec > 90.) dec -= 90;
  while (dec < -90.) dec += 90;

  if (dec < 0.)
  {
    sign[0] = '-';
    dec *= -1.;
  }
  decd = (long)(dec);
  dec -= decd;
  dec *= 60.;
  decm = (unsigned int)(dec);
  dec -= decm;
  decfs = dec*60.;

  sprintf(str_dec,"%s%2.2d%1s%2.2d%1s%04.1f", sign, decd, sep, decm, sep, decfs);
  return (str_dec);
}
