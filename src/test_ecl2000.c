/*
 * TESTING conversions from Equatorial to ecliptic ...
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DEG2RAD 1.74532925199432957692369E-2

int dcs2c(double a, double b, double *v)
{

double cosb;

    cosb = cos(b);
    v[0] = cos(a) * cosb;
    v[1] = sin(a) * cosb;
    v[2] = sin(b);

return 0;
}


int dcc2s(double *v, double *a, double *b)
{

    double r, x, y, z;

    x = v[0];
    y = v[1];
    z = v[2];
    r = sqrt(x * x + y * y);
    if (r == 0.) {
        *a = 0.;
    } else {
        *a = atan2(y, x);
    }
    if (z == 0.) {
        *b = 0.;
    } else {
        *b = atan2(z, r);
    }

return 0;
}


int dmxv(double *dm, double *va, double *vb)
{

int i, j;
  double w, vw[3];

    /* Parameter adjustments */
    --vb;
    --va;
    dm -= 4;

    /* Function Body */
    for (j = 1; j <= 3; ++j) {
        w = 0.;
        for (i = 1; i <= 3; ++i) {
            w += dm[j + i * 3] * va[i];
        }
        vw[j - 1] = w;
    }
/*  Vector VW -> vector VB */
    for (j = 1; j <= 3; ++j) {
        vb[j] = vw[j - 1];
    }

return 0;
}

int deuler_phi( const double phi, double *rmat)
{

double rotn[9] = {
       1.0, 0.0, 0.0,
       0.0, 1.0, 0.0,
       0.0, 0.0, 1.0
       };

double result[9] = {
       1.0, 0.0, 0.0,
       0.0, 1.0, 0.0,
       0.0, 0.0, 1.0
       };

double wm[9];
  double s = sin(phi);
  double c = cos(phi);


result[4] = c;
result[7] = s;
result[5] = -s;
result[8] = c;

double w;
int i, j, k;

/* Parameter adjustments */
    rmat -= 4;

/* Apply the current rotation (matrix ROTN x matrix RESULT) */
    for (i = 1; i <= 3; ++i) {
        for (j = 1; j <= 3; ++j) {
            w = 0.;
            for (k = 1; k <= 3; ++k) {
                w += rotn[i + k * 3 - 4] * result[k + j * 3 - 4];
            }
            wm[i + j * 3 - 4] = w;
        }
    }
    for (j = 1; j <= 3; ++j) {
        for (i = 1; i <= 3; ++i) {
            result[i + j * 3 - 4] = wm[i + j * 3 - 4];
        }
    }
/*  Copy the result */
    for (j = 1; j <= 3; ++j) {
        for (i = 1; i <= 3; ++i) {
            rmat[i + j * 3] = result[i + j * 3 - 4];
        }
    }

return 0;
}


void ecmat ( double *rmat) {
 
/*  Arc seconds to radians */
static const double as2r=0.484813681109535994E-5;

double eps0 = as2r*84381.448;


  deuler_phi(eps0,rmat);

}




int main(int argc, char **argv)
{

  double rmat[9];
  double v1[3], v2[3], ra, de, lat, lon;
  int i;
  char s[20];

  if (argc < 3) {
        printf( "RA: " );
	if (fgets(s, sizeof s, stdin))
	  ra = atof(s) * DEG2RAD;
        printf( "Dec: " );
	if (fgets(s, sizeof s, stdin))
	  de = atof(s) * DEG2RAD;
  } else {
	ra = atof(argv[1]) * DEG2RAD;
	de = atof(argv[2]) * DEG2RAD;
  }

// spherical to cartesian
  dcs2c(ra,de,v1);
// Mean J2000 to mean of date NOT required (all J2000.)
//  prec(2000D0,sla_EPJ(DATE),rmat);
//  dmxv(rmat,v1,v2);

  v2[0] = v1[0];
  v2[1] = v1[1];
  v2[2] = v1[2];

// Equatorial to ecliptic
  ecmat(rmat);
  dmxv(rmat,v2,v1);

  for( i = 0; i < 9; i++)
	printf("%15.11lf%s", rmat[i], (i % 3 == 2) ? "\n" : " ");

  dcc2s(v1, &lon, &lat);

// Express in conventional ranges
//      DL=sla_DRANRM(DL)
//      DB=sla_DRANGE(DB)


  printf("%15.11lf %15.11lf\n", lat/DEG2RAD, lon/DEG2RAD);

  return 0;
}
