/*

  Compute luminosity and angular size distance in Mpc for the Planck cosmology.
  Also compute the angular scale in kpc/arcsec.

  Note:
    The "Planck 2018" cosmology for a flat LCDM universe is assumed: H_0 = 67.4, Omega_m = 0.315, Omega_l = 0.685
    Derived from Ned Wright's Javascript Cosmology Calculator
  
    https://www.astro.ucla.edu/~wright/CosmoCalc.html
    or
    http://www.astro.yale.edu/simmons/cosmocalc/


  LN @ INAF-OAS, Jan 2024.  Last changed: 05/03/2025
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


//-- Angular size distance in Mpc
//
double z2ad(const float z) {
  if ( z <= 0.00001 )
	return 0.;

  int i;                   // index
  const int n = 1000;      // number of points in integrals
  const float H0 = 67.7;   // Hubble constant
  const float h = 0.674;   // H0/100
  const float WM = 0.315;  // Omega(matter)
  const float WV = 0.685;  // Omega(vacuum) or lambda
  const double WR = 4.165E-5/(h*h);  // Omega(radiation) - includes 3 massless neutrino species, T0 = 2.72528
  const double WK = 1.0-WM-WR-WV;    // Omega curvaturve = 1-Omega(total)
  const double c = 299792.458;       // velocity of light in km/s
  double DCMR = 0.0;    // comoving radial distance in units of c/H0
  double DA;            // angular size distance
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
  DA = az * DCMT(DCMR, WK) * (c/H0);  // angular distance (Mpc)

  return DA;
}


//-- Luminosity distance in Mpc
//
double z2ld(const float z) {
  double az = 1.0/(1.0 + z);
  double DA = z2ad(z);
  //double DL_Mpc = DA/(az*az) * (c/H0);  // luminosity distance (Mpc)
  double DL_Mpc = DA/(az*az);  // luminosity distance (Mpc)

  return DL_Mpc;
}


//-- Angular scale as kpc/arcsec
//
double z2ascale(const float z) {
  //scale = 0.0048481368 = pi/180 / 3.6 (-> kpc)
  double DA = z2ad(z) * 0.0048481368;
  return DA;
}

