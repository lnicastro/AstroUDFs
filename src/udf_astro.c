/*
  LN @ INAF-OAS, Dec 2009.  Last changed: 05/03/2025
  
To install:

CREATE FUNCTION skysep RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION z2lang RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION z2ldist RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION z2ascl RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION ras2deg RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION decs2deg RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION radec2gl RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION radec2gb RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION radec2glgb RETURNS STRING SONAME 'udf_astro.so';
CREATE FUNCTION radec2el RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION radec2eb RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION radec2eleb RETURNS STRING SONAME 'udf_astro.so';
CREATE FUNCTION glgb2ra RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION glgb2dec RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION glgb2radec RETURNS STRING SONAME 'udf_astro.so';
CREATE FUNCTION radecpm2ra RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION radecpm2dec RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION radecpmnow RETURNS STRING SONAME 'udf_astro.so';
CREATE FUNCTION rahstr RETURNS STRING SONAME 'udf_astro.so';
CREATE FUNCTION decstr RETURNS STRING SONAME 'udf_astro.so';
CREATE FUNCTION rahdecstr RETURNS STRING SONAME 'udf_astro.so';
CREATE FUNCTION mjd2datet RETURNS STRING SONAME 'udf_astro.so';
CREATE FUNCTION mjd2datetf RETURNS STRING SONAME 'udf_astro.so';

See ../sql/udfinstall.sql
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include <mysql.h>

#if MYSQL_VERSION_ID >= 80000 && MYSQL_VERSION_ID < 100000
typedef bool   my_bool;
#endif

#include "udf_astro.h"

//#ifdef HAVE_DLOPEN
//extern "C" {


// Local functions
DEFINE_FUNCTION(double, skysep);
DEFINE_FUNCTION(double, z2adist);
DEFINE_FUNCTION(double, z2ldist);
DEFINE_FUNCTION(double, z2ascl);
DEFINE_FUNCTION(double, radec2gl);
DEFINE_FUNCTION(double, radec2gb);
DEFINE_FUNCTION(double, radec2el);
DEFINE_FUNCTION(double, radec2eb);
DEFINE_FUNCTION(double, glgb2ra);
DEFINE_FUNCTION(double, glgb2dec);
DEFINE_FUNCTION(double, radecpm2ra);
DEFINE_FUNCTION(double, radecpm2de);
DEFINE_FUNCTION(double, ras2deg);
DEFINE_FUNCTION(double, decs2deg);
DEFINE_FUNCTION_CHAR(char*, radec2glgb);
DEFINE_FUNCTION_CHAR(char*, radec2eleb);
DEFINE_FUNCTION_CHAR(char*, glgb2radec);
DEFINE_FUNCTION_CHAR(char*, radecpmnow);
DEFINE_FUNCTION_CHAR(char*, radecstr);
DEFINE_FUNCTION_CHAR(char*, rahdecstr);
DEFINE_FUNCTION_CHAR(char*, rahstr);
DEFINE_FUNCTION_CHAR(char*, decstr);
DEFINE_FUNCTION_CHAR(char*, mjd2datet);
DEFINE_FUNCTION_CHAR(char*, mjd2datetf);

//}


/* Angular distance on the sphere (in "arcmin") */

my_bool skysep_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "skysep(RA1_deg DOUBLE, Dec1_deg DOUBLE, RA2_deg DOUBLE, Dec2_deg DOUBLE)";

  CHECK_ARG_NUM(4);
  CHECK_ARG_NOT_TYPE(0, STRING_RESULT);
  CHECK_ARG_NOT_TYPE(1, STRING_RESULT);
  CHECK_ARG_NOT_TYPE(2, STRING_RESULT);
  CHECK_ARG_NOT_TYPE(3, STRING_RESULT);

  return 0;
}


double skysep(UDF_INIT *init, UDF_ARGS *args,
                char *is_null, char* error)
{
  return skysep_h(DARGS(0), DARGS(1), DARGS(2), DARGS(3));
}


void skysep_deinit(UDF_INIT *init)
{}


/* From redshift z to angular size distance - H_0 = 67.4, Omega_M = 0.315, Omega_vac = 0.685 (Planck 2018) */

my_bool z2adist_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "z2adist(z DOUBLE)";

  CHECK_ARG_NUM(1);
  CHECK_ARG_NOT_TYPE(0, STRING_RESULT);

  init->ptr = NULL;

  return 0;
}


double z2adist(UDF_INIT *init, UDF_ARGS *args,
                char *is_null, char* error)
{
  return z2ad(DARGS(0));
}


void z2adist_deinit(UDF_INIT *init)
{}



/* From redshift z to luminosity distance */

my_bool z2ldist_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "z2ldist(z DOUBLE)";

  CHECK_ARG_NUM(1);
  CHECK_ARG_NOT_TYPE(0, STRING_RESULT);

  init->ptr = NULL;

  return 0;
}


double z2ldist(UDF_INIT *init, UDF_ARGS *args,
                char *is_null, char* error)
{
  return z2ld(DARGS(0));
}


void z2ldist_deinit(UDF_INIT *init)
{}



/* From redshift z to angular scale (arcsec/kpc) */

my_bool z2ascl_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "z2ascl(z DOUBLE)";

  CHECK_ARG_NUM(1);
  CHECK_ARG_NOT_TYPE(0, STRING_RESULT);

  init->ptr = NULL;

  return 0;
}


double z2ascl(UDF_INIT *init, UDF_ARGS *args,
                char *is_null, char* error)
{
  return z2ascale(DARGS(0));
}


void z2ascl_deinit(UDF_INIT *init)
{}



/* Decode right ascension and declination from sexagesimal string to decimal degrees */

my_bool ras2deg_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "ras2deg(RA_hms STRING)";

  CHECK_ARG_NUM(1);
  CHECK_ARG_TYPE(1, STRING_RESULT);

  init->ptr = NULL;

  return 0;
}


double ras2deg(UDF_INIT *init, UDF_ARGS *args,
                char *is_null, char* error)
{
  return deg_ra(CARGS(0));
}


void ras2deg_deinit(UDF_INIT *init)
{}


my_bool decs2deg_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "ras2deg(DEC_ddmmss STRING)";

  CHECK_ARG_NUM(1);
  CHECK_ARG_TYPE(1, STRING_RESULT);

  init->ptr = NULL;

  return 0;
}


double decs2deg(UDF_INIT *init, UDF_ARGS *args,
                char *is_null, char* error)
{
  return deg_dec(CARGS(0));
}


void decs2deg_deinit(UDF_INIT *init)
{}


/* Equatorial 2000 to Galactic 2000 */

my_bool radec2glgb_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "radec2glgb(RA_deg DOUBLE, Dec_deg DOUBLE)";

  CHECK_ARG_NUM(2);
  CHECK_ARG_NOT_TYPE(0, STRING_RESULT);
  CHECK_ARG_NOT_TYPE(1, STRING_RESULT);

  init->maybe_null = 0;
  init->const_item = 0;

  init->ptr = NULL;

  return 0;
}


char* radec2glgb(UDF_INIT *init, UDF_ARGS *args,
                char *result, unsigned long *length,
                char *is_null, char* error)
{
  double gl, gb;

  radec2GlGb(DARGS(0), DARGS(1), &gl, &gb);

  sprintf(result, "%.16g, %.16g", gl, gb);
  *length = (unsigned long) strlen(result);

  init->ptr = result;
  return result;
}


void radec2glgb_deinit(UDF_INIT *init)
{}


my_bool radec2gl_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "radec2gl(RA_deg DOUBLE, Dec_deg DOUBLE)";

  CHECK_ARG_NUM(2);
  CHECK_ARG_NOT_TYPE(0, STRING_RESULT);
  CHECK_ARG_NOT_TYPE(1, STRING_RESULT);

  init->ptr = NULL;

  return 0;
}


double radec2gl(UDF_INIT *init, UDF_ARGS *args,
                char *is_null, char* error)
{
  return radec2Gl(DARGS(0), DARGS(1));
}


void radec2gl_deinit(UDF_INIT *init)
{}


my_bool radec2gb_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "radec2gb(RA_deg DOUBLE, Dec_deg DOUBLE)";

  CHECK_ARG_NUM(2);
  CHECK_ARG_NOT_TYPE(0, STRING_RESULT);
  CHECK_ARG_NOT_TYPE(1, STRING_RESULT);

  init->ptr = NULL;

  return 0;
}


double radec2gb(UDF_INIT *init, UDF_ARGS *args,
                char *is_null, char* error)
{
  return radec2Gb(DARGS(0), DARGS(1));
}


void radec2gb_deinit(UDF_INIT *init)
{}



/* Equatorial 2000 to Ecliptic 2000 */

my_bool radec2eleb_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "radec2eleb(RA_deg DOUBLE, Dec_deg DOUBLE)";

  CHECK_ARG_NUM(2);
  CHECK_ARG_NOT_TYPE(0, STRING_RESULT);
  CHECK_ARG_NOT_TYPE(1, STRING_RESULT);

  init->maybe_null = 0;
  init->const_item = 0;

  init->ptr = NULL;

  return 0;
}


char* radec2eleb(UDF_INIT *init, UDF_ARGS *args,
                char *result, unsigned long *length,
                char *is_null, char* error)
{
  double el, eb;

  radec2ElEb(DARGS(0), DARGS(1), &el, &eb);

  sprintf(result, "%.16g, %.16g", el, eb);
  *length = (unsigned long) strlen(result);

  init->ptr = result;
  return result;
}


void radec2eleb_deinit(UDF_INIT *init)
{}


my_bool radec2el_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "radec2el(RA_deg DOUBLE, Dec_deg DOUBLE)";

  CHECK_ARG_NUM(2);
  CHECK_ARG_NOT_TYPE(0, STRING_RESULT);
  CHECK_ARG_NOT_TYPE(1, STRING_RESULT);

  init->ptr = NULL;

  return 0;
}


double radec2el(UDF_INIT *init, UDF_ARGS *args,
                char *is_null, char* error)
{
  return radec2El(DARGS(0), DARGS(1));
}


void radec2el_deinit(UDF_INIT *init)
{}


my_bool radec2eb_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "radec2eb(RA_deg DOUBLE, Dec_deg DOUBLE)";

  CHECK_ARG_NUM(2);
  CHECK_ARG_NOT_TYPE(0, STRING_RESULT);
  CHECK_ARG_NOT_TYPE(1, STRING_RESULT);

  init->ptr = NULL;

  return 0;
}


double radec2eb(UDF_INIT *init, UDF_ARGS *args,
                char *is_null, char* error)
{
  return radec2Eb(DARGS(0), DARGS(1));
}


void radec2eb_deinit(UDF_INIT *init)
{}



/* Galactic to Equatorial 2000 */

my_bool glgb2radec_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "glgb2radec(Gl_deg DOUBLE, Gb_deg DOUBLE)";

  CHECK_ARG_NUM(2);
  CHECK_ARG_NOT_TYPE(0, STRING_RESULT);
  CHECK_ARG_NOT_TYPE(1, STRING_RESULT);

  init->maybe_null = 0;
  init->const_item = 0;

  init->ptr = NULL;

  return 0;
}


char* glgb2radec(UDF_INIT *init, UDF_ARGS *args,
                char *result, unsigned long *length,
                char *is_null, char* error)
{
  double gl, gb;

  GlGb2radec(DARGS(0), DARGS(1), &gl, &gb);

  sprintf(result, "%.16g, %.16g", gl, gb);
  *length = (unsigned long) strlen(result);

  init->ptr = result;
  return result;
}


void glbg2radec_deinit(UDF_INIT *init)
{}


my_bool glgb2ra_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "glgb2ra(Gl_deg DOUBLE, Gb_deg DOUBLE)";

  CHECK_ARG_NUM(2);
  CHECK_ARG_NOT_TYPE(0, STRING_RESULT);
  CHECK_ARG_NOT_TYPE(1, STRING_RESULT);

  init->ptr = NULL;

  return 0;
}


double glgb2ra(UDF_INIT *init, UDF_ARGS *args,
                char *is_null, char* error)
{
  return GlGb2ra(DARGS(0), DARGS(1));
}


void glgb2ra_deinit(UDF_INIT *init)
{}


my_bool glgb2dec_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "glgb2dec(Gl_deg DOUBLE, Gb_deg DOUBLE)";

  CHECK_ARG_NUM(2);
  CHECK_ARG_NOT_TYPE(0, STRING_RESULT);
  CHECK_ARG_NOT_TYPE(1, STRING_RESULT);

  init->ptr = NULL;

  return 0;
}


double glgb2dec(UDF_INIT *init, UDF_ARGS *args,
                char *is_null, char* error)
{
  return GlGb2dec(DARGS(0), DARGS(1));
}


void glgb2dec_deinit(UDF_INIT *init)
{}



/* PM updated coords at given epoch */

my_bool radecpm2ra_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "radecpm2ra(RA_deg DOUBLE, Dec_deg DOUBLE, pmRA DOUBLE, pmDec DOUBLE, catEpoch DOUBLE, reqEpoch DOUBLE)";

  CHECK_ARG_NUM(6);
  for (int i = 0; i < 6; i++)
	CHECK_ARG_NOT_TYPE(i, STRING_RESULT);

  init->ptr = NULL;

  return 0;
}


double radecpm2ra(UDF_INIT *init, UDF_ARGS *args,
                char *is_null, char* error)
{
  return myepoch_coords_ra(DARGS(0), DARGS(1), DARGS(2), DARGS(3), DARGS(4), DARGS(5));
}


void radecpm2ra_deinit(UDF_INIT *init)
{}


my_bool radecpm2dec_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "radecpm2dec(RA_deg DOUBLE, Dec_deg DOUBLE, pmRA DOUBLE, pmDec DOUBLE, catEpoch DOUBLE, reqEpoch DOUBLE)";

  CHECK_ARG_NUM(6);
  for (int i = 0; i < 6; i++)
	CHECK_ARG_NOT_TYPE(i, STRING_RESULT);

  init->ptr = NULL;

  return 0;
}


double radecpm2dec(UDF_INIT *init, UDF_ARGS *args,
                char *is_null, char* error)
{
  return myepoch_coords_dec(DARGS(0), DARGS(1), DARGS(2), DARGS(3), DARGS(4), DARGS(5));
}


void radecpm2dec_deinit(UDF_INIT *init)
{}


my_bool radecpmnow_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "radecpmnow(RA_deg DOUBLE, Dec_deg DOUBLE, pmRA DOUBLE, pmDec DOUBLE, catEpoch DOUBLE, reqEpoch DOUBLE)";

  CHECK_ARG_NUM(6);
  for (int i = 0; i < 6; i++)
	CHECK_ARG_NOT_TYPE(i, STRING_RESULT);

  init->max_length = 128;
  init->maybe_null = 0;
  init->const_item = 0;

  init->ptr = NULL;

  return 0;
}


char* radecpmnow(UDF_INIT *init, UDF_ARGS *args,
		char *result, unsigned long *length,
                char *is_null, char* error)
{
  
  double rapmnow, depmnow;

  myepoch_coords(DARGS(0), DARGS(1), DARGS(2), DARGS(3), DARGS(4), DARGS(5), &rapmnow, &depmnow);

  sprintf(result, "%.16g, %.16g", rapmnow, depmnow);
  *length = (unsigned long) strlen(result);

  init->ptr = result;
  return result;
}


void radecpmnow_deinit(UDF_INIT *init)
{}



/* Encode into strings RA and Dec */

my_bool radecstr_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "radecstr(RA_deg DOUBLE, DEC_deg, separator STRING)";

  switch (args->arg_count) {
    case 3:
	CHECK_ARG_TYPE(2, STRING_RESULT);
    case 2:
	CHECK_ARG_NOT_TYPE(0, STRING_RESULT);
	CHECK_ARG_NOT_TYPE(1, STRING_RESULT);
	break;
    default:
	CHECK_ARG_NUM(3);
  }

  init->maybe_null = 0;
  init->const_item = 0;

  init->ptr = NULL;

  return 0;
}


char* radecstr(UDF_INIT *init, UDF_ARGS *args,
		char *result, unsigned long *length,
		char *is_null, char* error)
{

  double radeg = DARGS(0)/15.,
	 decdeg = DARGS(1);
  char *sep = {":"};

  if (args->arg_count == 3)
	sep = CARGS(2);

  sprintf(result, "%s, %s", enc_str_rah(radeg, sep), enc_str_decdeg(decdeg, sep));
  *length = (unsigned long) strlen(result);

  init->ptr = result;
  return result;
}


void radecstr_deinit(UDF_INIT *init)
{}



my_bool rahdecstr_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "rahdecstr(RA_hours DOUBLE, DEC_deg DOUBLE, separator STRING)";

  switch (args->arg_count) {
    case 3:
	CHECK_ARG_TYPE(2, STRING_RESULT);
    case 2:
	CHECK_ARG_NOT_TYPE(0, STRING_RESULT);
	CHECK_ARG_NOT_TYPE(1, STRING_RESULT);
	break;
    default:
	CHECK_ARG_NUM(3);
  }

  init->maybe_null = 0;
  init->const_item = 0;

  init->ptr = NULL;

  return 0;
}


char* rahdecstr(UDF_INIT *init, UDF_ARGS *args,
		char *result, unsigned long *length,
		char *is_null, char* error)
{

  double rahr = DARGS(0),
	 decdeg = DARGS(1);
  char *sep = {":"};

  if (args->arg_count == 3)
	sep = CARGS(2);

  sprintf(result, "%s, %s", enc_str_rah(rahr, sep), enc_str_decdeg(decdeg, sep));
  *length = (unsigned long) strlen(result);

  init->ptr = result;
  return result;
}


void rahdecstr_deinit(UDF_INIT *init)
{}


my_bool rahstr_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "rahstr(RA_hours DOUBLE, separator STRING)";

  switch (args->arg_count) {
    case 2:
	CHECK_ARG_TYPE(1, STRING_RESULT);
    case 1:
	CHECK_ARG_NOT_TYPE(0, STRING_RESULT);
	break;
    default:
	CHECK_ARG_NUM(2);
  }

  init->maybe_null = 0;
  init->const_item = 0;

  init->ptr = NULL;

  return 0;
}


char* rahstr(UDF_INIT *init, UDF_ARGS *args,
		char *result, unsigned long *length,
		char *is_null, char* error)
{

  double rahr = DARGS(0);
  char *sep = {":"};

  if (args->arg_count == 2)
	sep = CARGS(1);

  sprintf(result, "%s", enc_str_rah(rahr, sep));
  *length = (unsigned long) strlen(result);

  init->ptr = result;
  return result;
}


void rahstr_deinit(UDF_INIT *init)
{}


my_bool decstr_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "rahstr(DEC_deg DOUBLE, separator STRING)";

  switch (args->arg_count) {
    case 2:
	CHECK_ARG_TYPE(1, STRING_RESULT);
    case 1:
	CHECK_ARG_NOT_TYPE(0, STRING_RESULT);
	break;
    default:
	CHECK_ARG_NUM(2);
  }

  init->maybe_null = 0;
  init->const_item = 0;

  init->ptr = NULL;

  return 0;
}


char* decstr(UDF_INIT *init, UDF_ARGS *args,
		char *result, unsigned long *length,
		char *is_null, char* error)
{

  double decdeg = DARGS(0);
  char *sep = {":"};

  if (args->arg_count == 2)
	sep = CARGS(1);

  sprintf(result, "%s", enc_str_decdeg(decdeg, sep));
  *length = (unsigned long) strlen(result);

  init->ptr = result;
  return result;
}


void decstr_deinit(UDF_INIT *init)
{}


my_bool mjd2datet_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "mjd2date(MJD DOUBLE)";

  CHECK_ARG_NUM(1);
  CHECK_ARG_NOT_TYPE(0, STRING_RESULT);

  init->maybe_null = 0;
  init->const_item = 0;

  init->ptr = NULL;

  return 0;
}


char* mjd2datet(UDF_INIT *init, UDF_ARGS *args,
		char *result, unsigned long *length,
		char *is_null, char* error)
{

  double mjd = DARGS(0);

  sprintf(result, "%s", mjd2date(mjd));
  *length = (unsigned long) strlen(result);

  init->ptr = result;
  return result;
}


void mjd2datet_deinit(UDF_INIT *init)
{}



my_bool mjd2datetf_init(UDF_INIT* init, UDF_ARGS *args, char *message)
{
  const char* argerr = "mjd2date(MJD DOUBLE)";

  CHECK_ARG_NUM(1);
  CHECK_ARG_NOT_TYPE(0, STRING_RESULT);

  init->maybe_null = 0;
  init->const_item = 0;

  init->ptr = NULL;

  return 0;
}


char* mjd2datetf(UDF_INIT *init, UDF_ARGS *args,
		char *result, unsigned long *length,
		char *is_null, char* error)
{

  double mjd = DARGS(0);

  sprintf(result, "%s", mjd2datef(mjd));
  *length = (unsigned long) strlen(result);

  init->ptr = result;
  return result;
}


void mjd2datetf_deinit(UDF_INIT *init)
{}

//#endif /* HAVE_DLOPEN */
