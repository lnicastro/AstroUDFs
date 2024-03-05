/*
  Name: radec2GlGb, radec2Gl, radec2Gb

   Purpose: from Equatorial RA, Dec to Galactic l, b and vice-versa.

   Note: input and oputput coordinates are in "degrees".


  LN@INAF-OAS, Dec. 2009                   ( Last change: 05/03/2024 )
*/

#include <math.h>

extern double asine(const double arg);

const static double RAD2DEG = 57.295779513082320876798155;
const static double DEG2RAD = 1.74532925199432957692369E-2;
const static double PI2 = 6.2831853071795864769252867665590057683943387987502;

static const double MEQ2GL[9] = {  /* Equatorial 2000.0 <-> Galactic */
      -.054875539726, -.873437108010, -.483834985808,
       .494109453312, -.444829589425,  .746982251810,
      -.867666135858, -.198076386122,  .455983795705 };


int inline precess_vector(const double *matrix, const double *v1, double *v2)
{
  int i = 3;

  while (i--)
  {
     *v2++ = matrix[0] * v1[0] + matrix[1] * v1[1] + matrix[2] * v1[2];
     matrix += 3;
  }
  return 0;
}


int inline deprecess_vector(const double *matrix, const double *v1, double *v2)
{
  int i = 3;

  while( i--)
  {
     *v2++ = matrix[0] * v1[0] + matrix[3] * v1[1] + matrix[6] * v1[2];
     matrix++;
  }
  return 0;
}


int radec2GlGb(const double ra, const double dec, double *gl, double *gb)
{
  double opt, v1[3], v2[3];
  const double rarad = ra*DEG2RAD, decrad = dec*DEG2RAD, *matrix = MEQ2GL;

  double cosd = cos(decrad);

  v1[0] = cos(rarad) * cosd;
  v1[1] = sin(rarad) * cosd;
  v1[2] = sin(decrad);

  precess_vector(matrix, v1, v2);

  if (v2[1] || v2[0])
    opt = atan2(v2[1], v2[0]);
  else
    opt = 0.;

  opt = fmod(opt, PI2);

  if (opt < 0.)
    opt += PI2;

  *gl = (opt * RAD2DEG);
  *gb = (asine(v2[2]) * RAD2DEG);

  return 0;
}


double radec2Gl(const double ra, const double dec)
{
  double opt, v1[3], v2[3];
  const double rarad = ra*DEG2RAD, decrad = dec*DEG2RAD, *matrix = MEQ2GL;

  double cosd = cos(decrad);

  v1[0] = cos(rarad) * cosd;
  v1[1] = sin(rarad) * cosd;
  v1[2] = sin(decrad);

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


double radec2Gb(const double ra, const double dec)
{
  double v1[3], v2[3];
  const double rarad = ra*DEG2RAD, decrad = dec*DEG2RAD, *matrix = MEQ2GL;

  double cosd = cos(decrad);

  v1[0] = cos(rarad) * cosd;
  v1[1] = sin(rarad) * cosd;
  v1[2] = sin(decrad);

  precess_vector(matrix, v1, v2);

  return asine(v2[2]) * RAD2DEG;
}


double GlGb2ra(const double gl, const double gb)
{
  double opt, v1[3], v2[3];
  const double glrad = gl*DEG2RAD, gbrad = gb*DEG2RAD, *matrix = MEQ2GL;

  double cosb = cos(gbrad);

  v1[0] = cos(glrad) * cosb;
  v1[1] = sin(glrad) * cosb;
  v1[2] = sin(gbrad);

  deprecess_vector(matrix, v1, v2);

  if (v2[1] || v2[0])
    opt = atan2(v2[1], v2[0]);
  else
    return 0.;

  opt = fmod(opt, PI2);

  if (opt < 0.)
    opt += PI2;

  return opt * RAD2DEG;
}


double GlGb2dec(const double gl, const double gb)
{
  double v1[3], v2[3];
  const double glrad = gl*DEG2RAD, gbrad = gb*DEG2RAD, *matrix = MEQ2GL;

  double cosb = cos(gbrad);

  v1[0] = cos(glrad) * cosb;
  v1[1] = sin(glrad) * cosb;
  v1[2] = sin(gbrad);

  deprecess_vector(matrix, v1, v2);

  return asine(v2[2]) * RAD2DEG;
}


int GlGb2radec(const double gl, const double gb, double *ra, double *dec)
{
  double opt, v1[3], v2[3];
  const double glrad = gl*DEG2RAD, gbrad = gb*DEG2RAD, *matrix = MEQ2GL;

  double cosb = cos(gbrad);

  v1[0] = cos(glrad) * cosb;
  v1[1] = sin(glrad) * cosb;
  v1[2] = sin(gbrad);

  deprecess_vector(matrix, v1, v2);

  if (v2[1] || v2[0])
    opt = atan2(v2[1], v2[0]);
  else
    opt = 0.;

  opt = fmod(opt, PI2);

  if (opt < 0.)
    opt += PI2;

  *ra = (opt * RAD2DEG);
  *dec = (asine(v2[2]) * RAD2DEG);

  return 0;
}
