/*
  From equatorial rectangular coordinates to spherical RA, Dec.

  Input:
    pos[3] (double)	Position vector, equatorial rectangular coordinates.

  Output:
    *ra, *de (double)	Right ascension and declination (degrees)


  Return: 0 on success. 


  LN @ INAF-OAS, Dec 2009.  Last changed: 04/06/2020
*/

#include <stdlib.h>
#include <math.h>

/* radians to degrees */
static const double RAD2DEG = 57.295779513082320876798155;

/* degrees to radians */
static const double DEG2RAD = 1.74532925199432957692369E-2;

/* arcsec to radians */
static const double ASEC2RAD = 4.848136811095359935899141e-6;


int vector2radec(const double *pos, double *ra, double *de)
{

  //if ( pos[0] > 1.  || pos[1]  > 1. || pos[2] > 1.  )
	//return 1;
 
  double xyproj = sqrt (pos[0] * pos[0] + pos[1] * pos[1]);

  if ( (xyproj == 0.) && (pos[2] == 0.) )
  {
	*ra = 0.;
	*de = 0.;

  } else if ( xyproj == 0. ) {
	*ra = 0.;
	if (pos[2] < 0.)
		*de = -90.;
	else
		*de = 90.;

  } else {

	*ra = atan2(pos[1], pos[0]) * RAD2DEG;
      	*de = atan2(pos[2], xyproj) * RAD2DEG;

	if ( *ra < 0. )
		*ra += 360.;
  }

  return 0;
}


int vector2ra(const double *pos, double *ra)
{
 
  double xyproj = sqrt (pos[0] * pos[0] + pos[1] * pos[1]);

  if ( (xyproj == 0.) && (pos[2] == 0.) )
	*ra = 0.;

  else if ( xyproj == 0. )
	*ra = 0.;

  else {
	*ra = atan2(pos[1], pos[0]) * RAD2DEG;
	if ( *ra < 0. )
		*ra += 360.;
  }

  return 0;
}


int vector2dec(const double *pos, double *de)
{
 
  double xyproj = sqrt (pos[0] * pos[0] + pos[1] * pos[1]);

  if ( (xyproj == 0.) && (pos[2] == 0.) )
	*de = 0.;

  else if ( xyproj == 0. ) {
	if (pos[2] < 0.)
		*de = -90.;
	else
		*de = 90.;

  } else
      	*de = atan2(pos[2], xyproj) * RAD2DEG;

  return 0;
}


/*
  From equatorial spherical RA, Dec to unit rectangular coordinates.

  Input:
    ra, de (double)	Right ascension and declination (in degrees)

  Output:
    pos[3] (double)	Position vector, equatorial rectangular coordinates.


 Return: void function. 
*/

void radec2vec(const double ra, const double de, double *pos)
{

  double r = ra * DEG2RAD,
	 d = de * DEG2RAD,
	 cde = cos(d);
  pos[0] = cde * cos(r);
  pos[1] = cde * sin(r);
  pos[2] = sin(d);

  return;
}


/*
  From equatorial spherical RA, Dec and PMs (mas/yr) to unit rectangular coordinates.

  Input:
    ra, de (double)		Right ascension and declination (in degrees)
    pmra, pmde (double)		pmRA*cos(Dec), pmDec (mas/yr)
    parallax (double)		Parallax (mas)
    epoch0, epoch1 (double)	Position reference and second epochs

  Output:
    pos[3] (double)     Position vector, equatorial rectangular coordinates.


  Note: star radial velocity assumed 0.


 Return: void function. 
*/

void radecpm2vec(const double ra, const double de, const double pmra, const double pmde,
		const double parallax, const double epoch0, const double epoch1, double *pos)
{

  double pax = parallax;
/* 1e-6 mas, corresponding to a distance of 1 gigaparsec */
  if ( pax <= 0. )
	pax = 1.e-6;

  double dist = 1. / sin(pax * 1.0e-3 * ASEC2RAD);  // From mas to rad

/* Times the mean year -> PM components as AU/Day */
  pax *= 365.25;

  double r = ra * DEG2RAD,
	 d = de * DEG2RAD,
	 cra = cos(r),
	 sra = sin(r),
	 cde = cos(d),
	 sde = sin(d);
  pos[0] = dist * cde * cra;
  pos[1] = dist * cde * sra;
  pos[2] = dist * sde;

  double dt =  epoch1 - epoch0,
	 pmr = pmra / pax,
  	 pmd = pmde / pax,
	 vel[3];

  vel[0] = - pmr * sra - pmd * sde * cra;
  vel[1] =   pmr * cra - pmd * sde * sra;
  vel[2] =   pmd * cde;


  for (int i = 0; i < 3; i++) {
	pos[i] += (vel[i] * dt);
  }

  return;
}


/*
  From equatorial spherical RA, Dec and PMs (mas/yr) to unit rectangular coordinates
  using a fixed 1 mas parallax.

  Input:
    ra, de (double)		Right ascension and declination (in degrees)
    pmra, pmde (double)		pmRA*cos(Dec), pmDec (mas/yr)
    epoch0, epoch1 (double)	Position reference and second epochs

  Output:
    pos[3] (double)     Position vector, equatorial rectangular coordinates.


  Note: star radial velocity assumed 0.


 Return: void function. 
*/

void radecpm2vecn(const double ra, const double de, const double pmra, const double pmde,
		const double epoch0, const double epoch1, double *pos)
{

  double dist = 1. / sin(1.0e-3 * ASEC2RAD);  // From mas to rad. Do not use parallax.

  double r = ra * DEG2RAD,
	 d = de * DEG2RAD,
	 cra = cos(r),
	 sra = sin(r),
	 cde = cos(d),
	 sde = sin(d);
  pos[0] = dist * cde * cra;
  pos[1] = dist * cde * sra;
  pos[2] = dist * sde;


  double dt = epoch1 - epoch0,
	 pmr = pmra / 365.25,
	 pmd = pmde / 365.25,
	 vel[3];

  vel[0] = (- pmr * sra - pmd * sde * cra ) * dt;
  vel[1] = (  pmr * cra - pmd * sde * sra ) * dt;
  vel[2] = (  pmd * cde ) * dt;


  for (int i = 0; i < 3; i++)
	pos[i] += vel[i];

// Normalize
  double xyproj = sqrt (pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2]);

  for (int i = 0; i < 3; i++)
	pos[i] /= xyproj;

  return;
}


void myepoch_coords(const double ra, const double de, const double pmra, const double pmde,
		const double epoch0, const double epoch1, double *ra1, double *de1) {

  if ( (pmra == 0. && pmde == 0.) || (epoch0 == epoch1) ) {
	*ra1 = ra;
	*de1 = de;
  } else {

	double pos[3];
	radecpm2vecn(ra, de, pmra, pmde, epoch0, epoch1, pos);
	vector2radec(pos, ra1, de1);
  }

  return;
}


double myepoch_coords_ra(const double ra, const double de, const double pmra, const double pmde,
		const double epoch0, const double epoch1) {

  if ( (pmra == 0. && pmde == 0.) || (epoch0 == epoch1) )
	return (ra);

  double pos[3], ra1 = ra;
  radecpm2vecn(ra, de, pmra, pmde, epoch0, epoch1, pos);
  vector2ra(pos, &ra1);

  return (ra1);
}


double myepoch_coords_dec(const double ra, const double de, const double pmra, const double pmde,
		const double epoch0, const double epoch1) {

  if ( (pmra == 0. && pmde == 0.) || (epoch0 == epoch1) )
	return (de);

  double pos[3], de1 = de;
  radecpm2vecn(ra, de, pmra, pmde, epoch0, epoch1, pos);
  vector2dec(pos, &de1);

  return (de1);
}
