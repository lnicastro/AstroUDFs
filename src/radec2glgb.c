/*
  Name: radec2GlGb, radec2Gl, radec2Gb

   Purpose: from equatorial RA, Dec to galactic l, b.

   Note: input and oputput coordinates are in "degrees".


  LN@INAF-OAS, Dec. 2009                   ( Last change: 29/05/2020 )
*/

#include <math.h>

extern double asine(const double arg);

const static double RAD2DEG = 57.295779513082320876798155;
const static double DEG2RAD = 1.74532925199432957692369E-2;
//const static double PI = 3.141592653589793238462643383279;
const static double PI2 = 6.2831853071795864769252867665590057683943387987502;

static const double MEQ2GL[9] = {  /* Equatorial 2000.0 -> Galactic */
      -.054875539726, -.873437108010, -.483834985808,
       .494109453312, -.444829589425,  .746982251810,
      -.867666135858, -.198076386122,  .455983795705 };


int inline precess_vector(const double  *matrix, const double  *v1, double  *v2)
{
  int i = 3;

  while (i--)
  {
    *v2++ = matrix[0] * v1[0] + matrix[1] * v1[1] + matrix[2] * v1[2];
    matrix += 3;
  }
  return 0;
}


int radec2GlGb(const double ra, const double dec, double *gl, double *gb)
{
  double opt, v1[3], v2[3];
  const double rarad = ra*DEG2RAD, decrad = dec*DEG2RAD, *matrix = MEQ2GL;

  v1[0] = cos(rarad) * cos(decrad);
  v1[1] = sin(rarad) * cos(decrad);
  v1[2] =              sin(decrad);

  precess_vector(matrix, v1, v2);

  if (v2[1] || v2[0])
    opt = atan2( v2[1], v2[0]);
  else
    opt = 0.;

  opt = fmod(opt, PI2);

  if (opt < 0.)
    opt += PI2;

  *gl = (opt * RAD2DEG);
  *gb = (asine(v2[2]) * RAD2DEG);

  return 0;
}


double radec2Gl( const double ra, const double dec)
{
  double opt, v1[3], v2[3];
  const double rarad = ra*DEG2RAD, decrad = dec*DEG2RAD, *matrix = MEQ2GL;

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


double radec2Gb( const double ra, const double dec)
{
  double v1[3], v2[3];
  const double rarad = ra*DEG2RAD, decrad = dec*DEG2RAD, *matrix = MEQ2GL;

  v1[0] = cos(rarad) * cos(decrad);
  v1[1] = sin(rarad) * cos(decrad);
  v1[2] =              sin(decrad);

  precess_vector(matrix, v1, v2);

  return asine(v2[2]) * RAD2DEG;
}
