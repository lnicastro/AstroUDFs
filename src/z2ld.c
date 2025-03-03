/*
  Derived from Ned Wright's Javascript Cosmology Calculator
  
  https://www.astro.ucla.edu/~wright/CosmoCalc.html
  or
  http://www.astro.yale.edu/simmons/cosmocalc/
  
  Note:
    The "Planck 2018" cosmology for a flat LCDM universe is assumed: H_0 = 67.7, Omega_m = 0.31, Omega_l = 0.69


  LN @ INAF-OAS, Jan 2024.  Last changed: 03/03/2025
*/

#include <stdlib.h>
#include <math.h>

double DCMT(double DCMR, double WK)
{
  double ratio, x, y;
  ratio = 1.0;
  x = sqrt(fabs(WK)) * DCMR;
  if ( x > 0.1 )
  {
	ratio =  (WK > 0) ? 0.5*(exp(x)-exp(-x))/x : sin(x)/x;
	y = ratio * DCMR;
	return y;
  }

  y = x * x;
  if ( WK < 0 )
	y = -y;
  ratio = 1 + y / 6 + y * y / 120;
  y = ratio * DCMR;

  return y;
}


double z2ld(const float z) {
  if ( z <= 0.00001 )
	return 0.;

  int i;                   // index
  const int n = 1000;      // number of points in integrals
  const float H0 = 67.7;   // Hubble constant
  const float h = 0.677;   // H0/100
  const float WM = 0.310;  // Omega(matter)
  const float WV = 0.690;  // Omega(vacuum) or lambda
  const double WR = 4.165E-5/(h*h);  // Omega(radiation) - includes 3 massless neutrino species, T0 = 2.72528
  const double WK = 1.0-WM-WR-WV;    // Omega curvaturve = 1-Omega(total)
  const double c = 299792.458;       // velocity of light in km/s
  double DCMR = 0.0;    // comoving radial distance in units of c/H0
  double DA;            // angular size distance
  double DL_Mpc = 0.0;  // luminosity distance (Mpc)
  double a;             // 1/(1+z), the scale factor of the Universe
  double az;            // 1/(1+z(object))
  double adot;

  az = 1.0/(1.0 + z);  // 1/(1+z(object))
  for (i = 0; i != n; i++) {
	a = az*(i + 0.5)/n;
	adot = sqrt(WK + (WM/a) + (WR/(a*a)) + (WV*a*a));
  }

// do integral over a=1/(1+z) from az to 1 in n steps, midpoint rule
  for (i = 0; i != n; i++) {
	a = az + (1-az) * (i+0.5)/n;
	adot = sqrt(WK + (WM/a) + (WR/(a*a)) + (WV*a*a));
	DCMR = DCMR + 1/(a*adot);
  }

  DCMR = (1.0 - az) * DCMR/n;
  DA = az * DCMT(DCMR, WK);
  DL_Mpc = DA/(az*az) * (c/H0);

  return DL_Mpc;
}
