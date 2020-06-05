/*
  Name: radec2ElEb, radec2El, radec2Eb

   Purpose: from equatorial RA, Dec to Ecliptic l, b.

   Note: input and oputput coordinates are in "degrees".


  LN@INAF-OAS, Dec. 2009                   ( Last change: 29/05/2020 )
*/

#include <math.h>

extern double asine(const double arg);
extern int precess_vector(const double  *matrix, const double  *v1, double  *v2);


const static double RAD2DEG = 57.295779513082320876798155;
const static double DEG2RAD = 1.74532925199432957692369E-2;
//const static double PI = 3.141592653589793238462643383279;
const static double PI2 = 6.2831853071795864769252867665590057683943387987502;

static const double MEQ2EC[9] = {  /* Equatorial 2000.0 -> Ecliptic */
      1.0000000000,   0.0000000000,   0.0000000000,
      0.0000000000,   0.9173688522,   0.3978515869,
      0.0000000000,  -0.3978812429,   0.9174369278
};


int radec2ElEb(const double ra, const double dec, double *el, double *eb)
{
  double opt, v1[3], v2[3];
  const double rarad = ra*DEG2RAD, decrad = dec*DEG2RAD, *matrix = MEQ2EC;

  v1[0] = cos(rarad) * cos(decrad);
  v1[1] = sin(rarad) * cos(decrad);
  v1[2] =              sin(decrad);

  precess_vector(matrix, v1, v2);

  if (v2[1] || v2[0])
    opt = atan2(v2[1], v2[0]);
  else
    return 0.;

  opt = fmod(opt, PI2);

  if (opt < 0.)
    opt += PI2;

  *el = (opt * RAD2DEG);
  *eb = (asine(v2[2]) * RAD2DEG);

  return 0;
}


double radec2El(const double ra, const double dec)
{
  double opt, v1[3], v2[3];
  const double rarad = ra*DEG2RAD, decrad = dec*DEG2RAD, *matrix = MEQ2EC;

  v1[0] = cos(rarad) * cos(decrad);
  v1[1] = sin(rarad) * cos(decrad);
  v1[2] =              sin(decrad);

  precess_vector(matrix, v1, v2);

  if (v2[1] || v2[0])
    opt = atan2(v2[1], v2[0]);
  else
    return 0.;

  opt = fmod(opt, PI2);

  if (opt < 0.)
    opt += PI2;

  return opt * RAD2DEG;
}


double radec2Eb(const double ra, const double dec)
{
  double v1[3], v2[3];
  const double rarad = ra*DEG2RAD, decrad = dec*DEG2RAD, *matrix = MEQ2EC;

  v1[0] = cos(rarad) * cos(decrad);
  v1[1] = sin(rarad) * cos(decrad);
  v1[2] =              sin(decrad);

  precess_vector(matrix, v1, v2);

  return asine(v2[2]) * RAD2DEG;
}
