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
  return str_ra;
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

  return str_dec;
}


/*
  Name: double deg_ra
 
  Purpose:
    Decode a right ascension string formatted as "hh mm ss.s" or "hh:mm:ss.s"
    into a fractional double precision value.


  LN@INAF-OAS, November 2004                   ( Last change: 11/02/2016 )
*/

#include <stdio.h>

double deg_ra(const char *ra_str)
{
  unsigned int rah,ram, n=0;
  float rafs;
  double ra;

  if (ra_str[2] == ':' || ra_str[2] == ' ') {
    sscanf(ra_str,"%2d", &rah);
    ra_str += 3;
    n += 3;
  } else {
    sscanf(ra_str,"%1d", &rah);
    ra_str += 2;
    n += 2;
  }
  if (ra_str[2] == ':' || ra_str[2] == ' ') {
    sscanf(ra_str,"%2d", &ram);
    ra_str += 3;
    n += 3;
  } else {
    sscanf(ra_str,"%1d", &ram);
    ra_str += 2;
    n += 2;
  }
  sscanf(ra_str,"%f", &rafs);
  ra_str -= n;
  ra  = (rah + ram/60. + rafs/3.6e3) * 15.;
  if (ra < 0.) ra += 360;  // Should never happen

  return ra;
}


/*
  Name: double deg_dec

  Purpose:
    Decode a declination string formatted as "+/-dd mm ss.s" or "+/-dd:mm:ss.s"
    into a fractional double precision value.


  LN@INAF-OAS, November 2004                   ( Last change: 02/10/2010 )
*/

double deg_dec(const char *dec_str)
{
  unsigned int deg, dem, n=0;
  float defs;
  double de;

  if (dec_str[0] == '+' || dec_str[0] == '-') {
    dec_str += 1;
    n += 1;
  }

  if (dec_str[2] == ':' || dec_str[2] == ' ') {
    sscanf(dec_str,"%2d", &deg);
    dec_str += 3;
    n += 3;
  } else {
    sscanf(dec_str,"%1d", &deg);
    dec_str += 2;
    n += 2;
  }
  if (dec_str[2] == ':' || dec_str[2] == ' ') {
    sscanf(dec_str,"%2d", &dem);
    dec_str += 3;
    n += 3;
  } else {
    sscanf(dec_str,"%1d", &dem);
    dec_str += 2;
    n += 2;
  }
  sscanf(dec_str,"%f", &defs);
  de  = deg + dem/60. + defs/3.6e3;
  dec_str -= n;
  if (dec_str[0] == '-') de *= -1;

  return de;
}


/*
  Name: char *mjd2datef_gen

  Purpose:
    From MJD to date in the form
      "2006-08-07T12:15:11.123" (fractional sec => millisec)
    or
      "2006-08-07T12:15:11" (ISO 8601 string date format, e.g. DATE-OBS)

  Return '\0' for bad input date.


  LN@INAF-OAS, Aug 2006                        ( Last change: 30/01/2020 )
*/
char *mjd2date_gen(double mjd, unsigned int with_ms)
{
  static char date[24];
  const double DJMIN = -68569.5;
  const double DJMAX = 1e9;
  const double dj1=2400000.5;

  int iy, im, id, h, mm, s, fs;
  long jd, l, n, i, k;
  double dj, d1, d2, f1, f2, f, d;


/* Verify date is acceptable */
  dj = dj1 + mjd;
  if (dj < DJMIN || dj > DJMAX) return '\0';

/* Copy the date, big then small, and re-align to midnight */
  if (dj1 >= mjd) {
     d1 = dj1;
     d2 = mjd;
  } else {
     d1 = mjd;
     d2 = dj1;
  }
  d2 -= 0.5;

/* Separate day and fraction */
  f1 = fmod(d1, 1.);
  f2 = fmod(d2, 1.);
  f = fmod(f1 + f2, 1.);
  if (f < 0.) f += 1.;
  d = floor(d1 - f1) + floor(d2 - f2) + floor(f1 + f2 - f);
  jd = (long) floor(d) + 1L;

/* Express day in Gregorian calendar */
  l = jd + 68569L;
  n = (4L * l) / 146097L;
  l -= (146097L * n + 3L) / 4L;
  i = (4000L * (l + 1L)) / 1461001L;
  l -= (1461L * i) / 4L - 31L;
  k = (80L * l) / 2447L;
  id = (int) (l - (2447L * k) / 80L);
  l = k / 11L;
  im = (int) (k + 2L - 12L * l);
  iy = (int) (100L * (n - 49L) + i + l);
  h = (int) (f*24);
  f -= h/24.;
  mm = (int) (f*1440);
  f -= mm/1440.;

  if ( with_ms ) {
    s = (int)(f*86400);
    f -= s/86400.;
    fs = (int)(f*8.64e7 + 0.5);

    sprintf(date, "%4d-%2.2d-%2.2dT%2.2d:%2.2d:%2.2d.%3.3d", iy, im, id, h, mm, s, fs);
  } else {
    s = (int)(f*86400 + 0.5);

    sprintf(date, "%4d-%2.2d-%2.2dT%2.2d:%2.2d:%2.2d", iy, im, id, h, mm, s);
  }

  return date;
}


/*
  Name: char *mjd2date

  Purpose:
    From MJD to ISO 8601 string date format (DATE-OBS), e.g. "2006-08-07T12:15:11".
    (no fractional seconds).

  Note:
    call mjd2datef_gen

  Return '\0' for bad input date.


  LN@INAF-OAS, Aug 2006                        ( Last change: 30/01/2020 )
*/

char *mjd2date(double mjd)
{
  return mjd2date_gen(mjd, 0);
}


/*
  Name: char *mjd2datef

  Purpose:
    From MJD to date in the form "2006-08-07T12:15:11.123" (fractional seconds => millisec).

  Note:
    call mjd2datef_gen

  Return '\0' for bad input date.


  LN@INAF-OAS, Aug 2006                        ( Last change: 30/01/2020 )
*/

char *mjd2datef(double mjd)
{
  return mjd2date_gen(mjd, 1);
}
