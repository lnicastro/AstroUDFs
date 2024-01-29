DROP FUNCTION IF EXISTS skysep;
DROP FUNCTION IF EXISTS z2ldist;
DROP FUNCTION IF EXISTS ras2deg;
DROP FUNCTION IF EXISTS decs2deg;
DROP FUNCTION IF EXISTS radec2gl;
DROP FUNCTION IF EXISTS radec2gb;
DROP FUNCTION IF EXISTS radec2glgb;
DROP FUNCTION IF EXISTS radec2el;
DROP FUNCTION IF EXISTS radec2eb;
DROP FUNCTION IF EXISTS radec2eleb;
DROP FUNCTION IF EXISTS radecpm2ra;
DROP FUNCTION IF EXISTS radecpm2dec;
DROP FUNCTION IF EXISTS radecpmnow;
DROP FUNCTION IF EXISTS rahstr;
DROP FUNCTION IF EXISTS decstr;
DROP FUNCTION IF EXISTS rahdecstr;
DROP FUNCTION IF EXISTS mjd2datet;
DROP FUNCTION IF EXISTS mjd2datetf;

CREATE FUNCTION skysep RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION z2ldist RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION ras2deg RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION decs2deg RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION radec2gl RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION radec2gb RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION radec2glgb RETURNS STRING SONAME 'udf_astro.so';
CREATE FUNCTION radec2el RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION radec2eb RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION radec2eleb RETURNS STRING SONAME 'udf_astro.so';
CREATE FUNCTION radecpm2ra RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION radecpm2dec RETURNS REAL SONAME 'udf_astro.so';
CREATE FUNCTION radecpmnow RETURNS STRING SONAME 'udf_astro.so';
CREATE FUNCTION rahstr RETURNS STRING SONAME 'udf_astro.so';
CREATE FUNCTION decstr RETURNS STRING SONAME 'udf_astro.so';
CREATE FUNCTION rahdecstr RETURNS STRING SONAME 'udf_astro.so';
CREATE FUNCTION mjd2datet RETURNS STRING SONAME 'udf_astro.so';
CREATE FUNCTION mjd2datetf RETURNS STRING SONAME 'udf_astro.so';
