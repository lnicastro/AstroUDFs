# AstroUDFs
A set of handy astronomical tools for astronomical catalogues hosted on MySQL / MariaDB.
All implemented as C UDF functions.

Written to be used on Linux and Mac OS.


[![DOI](https://zenodo.org/badge/269748443.svg)](https://zenodo.org/badge/latestdoi/269748443)


More to come. THIS IS WORK IN PROGESS!

## Requirements

1. MySQL / MariaDB development files, mysql_config
2. make / gmake
3. C compiler

## Compile and install

```shell
make
sudo make install
```
On Mac OS, if you have compilation errors, try to use the appropriate path for the GNU `gcc`, e.g., assuming you use MacPorts:
```shell
make CC=/opt/local/bin/gcc
sudo make install
```

## UDFs installation

To import UDFs into MySQL (from the directory `sql`):
```shell
  shell> cd ../sql
  shell> mysql -u root -p

  mysql> source udfinstall.sql
```

## List of functions

 UDF        | Parameters     | Returns  |
----------- | -------------- | -------- |
skysep      | (RA1 ,Dec1, RA2, Dec2) | Double |
z2ldist     | (z) | Double |
ras2deg     | (RA_hhmmss) | String |
decs2deg    | (DEC_ddmmss) | String |
radec2gl    | (RA, Dec) | Double |
radec2gb    | (RA, Dec) | Double |
radec2glgb  | (RA, Dec) | String |
radec2el    | (RA, Dec) | Double |
radec2eb    | (RA, Dec) | Double |
radec2eleb  | (RA, Dec) | String |
radecpm2ra  | (RA_deg, Dec_deg, pmRA, pmDec, catEpoch, reqEpoch) | Double |
radecpm2dec | (RA_deg, Dec_deg, pmRA, pmDec, catEpoch, reqEpoch) | Double |
radecpmnow  | (RA_deg, Dec_deg, pmRA, pmDec, catEpoch, reqEpoch) | String |
rahstr      | (RA_hrs) | String |
decstr      | (Dec_deg) | String |
rahdecstr   | (RA_hra, Dec_deg) | String|
mjd2datet   | (MJD) | String |
mjd2datetf  | (MJD) | String |


## UDFs reference

### [ skysep ]
Compute the angular distance given the coordinates of two points on a
sphere.

**Syntax:**
`skysep(RA1, Dec1, RA2, Dec2)`

*RA1* `DOUBLE` : right ascension (or longitude) of the first point, in degrees;

*Dec1* `DOUBLE` : declination (or latitude) of the first point, in degrees;

*RA2* `DOUBLE` : right ascension (or longitude) of the second point, in degrees;

*Dec2* `DOUBLE` : declination (or latitude) of the second point, in degrees.

**Return value** (`DOUBLE`):
Angular distance between the points, in arcmin.

```sql
select Sphedist(125.6, -37.5, 121.4, -38.1) as d_arcmin;
+-------------------+
| 202.3282326405512 |
+-------------------+
```


### [ z2ldist ]
Compute the luminosity distance given the redshift z.
Cosmology parameters used are: `H_o = 69.6, Omega_M = 0.286, Omega_vac = 0.714`

**Syntax:**
`z2ldist(z)`

*z* `REAL` : redshift z;

**Return value** (`DOUBLE`):
Luminosity distance in Mpc.

```sql
select z2ldist(1.438) as D_L;
+-----------+
| 10523.511 |
+-----------+
```


### [ ras2deg ]
Decode a right ascension sexagesimal string formatted as "hh mm ss.s" or "hh:mm:ss.s" into fractional degrees 

**Syntax:**
`ras2deg(RA_hhmmss)`

*RA_hhmmss* `STRING` : Object right ascension in sexagesimal format;

**Return value** (`DOUBLE`):
Right ascension, in the range [0, 360[ degrees.

**Example:**

```sql
mysql> select ras2deg('00 33 18.34') as RAdeg;
+------------------+
| 8.32641666730245 |
+------------------+
```


### [ decs2deg ]
Decode a declination sexagesimal string formatted as "+/-dd mm ss.s" or "+/-dd:mm:ss.s" into fractional degrees 

**Syntax:**
`decs2deg(DEC_ddmmss)`

*DEC_ddmmss* `STRING` : Object declination in sexagesimal format;

**Return value** (`DOUBLE`):
Declination, in the range [-90, 90] degrees.

**Example:**

```sql
mysql> select decs2deg('+61 27 43.3') as DECdeg;
+-------------------+
| 61.46202777756585 |
+-------------------+
```


### [ radec2gl ]
Galactic longitude from given equatorial J2000 coordinates.

**Syntax:**
`radec2gl(RA, Dec)`

*RA* `DOUBLE` : Object right ascension (J2000) in degrees;

*Dec* `DOUBLE` : Object declination (J2000) in degrees.

**Return value** (`DOUBLE`):
Galactic longitude, in the range [0, 360[ degrees.

**Example:**

```sql
mysql> select radec2gl(320.85, 12.67) as Gl;
+-------+
| 64.32 |
+-------+
```


### [ radec2gb ]
Galactic latitude from given equatorial J2000 coordinates.

**Syntax:**
`radec2gb(RA, Dec)`

*RA* `DOUBLE` : Object right ascension (J2000) in degrees;

*Dec* `DOUBLE` : Object declination (J2000) in degrees.

**Return value** (`DOUBLE`):
Galactic latitude, in the range [-90, 90] degrees.

**Example:**

```sql
mysql> select radec2gb(320.85, 12.67) as Gb;
+--------+
| -25.75 |
+--------+
```


### [ radec2glgb ]
Galactic longitude and latitude from given equatorial J2000 coordinates.

**Syntax:**
`radec2glgb(RA, Dec)`

*RA* `DOUBLE` : Object right ascension (J2000) in degrees;

*Dec* `DOUBLE` : Object declination (J2000) in degrees.

**Return value** (`STRING`):
Galactic longitude and latitude, in the range [0, 360[, [-90, 90] degrees.

**Example:**

```sql
mysql> select radec2glgb(320.85, 12.67) as lon_lat;
+---------------------------------------+
| 64.31518160611735, -25.75108845741275 |
+---------------------------------------+
```


### [ radec2el ]
Ecliptic longitude from given equatorial J2000 coordinates.

**Syntax:**
`radec2el(RA, Dec)`

*RA* `DOUBLE` : Object right ascension (J2000) in degrees;

*Dec* `DOUBLE` : Object declination (J2000) in degrees.

**Return value** (`DOUBLE`):
Ecliptic longitude, in the range [0, 360[ degrees.

**Example:**

```sql
mysql> select radec2el(320.85, 12.67) as El;
+--------+
| 327.73 |
+--------+
```


### [ radec2eb ]
Ecliptic latitude from given equatorial J2000 coordinates.

**Syntax:**
`radec2eb(RA, Dec)`

*RA* `DOUBLE` : Object right ascension (J2000) in degrees;

*Dec* `DOUBLE` : Object declination (J2000) in degrees.

**Return value** (`DOUBLE`):
Ecliptic latitude, in the range [-90, 90] degrees.

**Example:**

```sql
mysql> select radec2eb(320.85, 12.67) as Eb;
+-------+
| 26.51 |
+-------+
```


### [ radec2eleb ]
Ecliptic longitude and latitude from given equatorial J2000 coordinates.

**Syntax:**
`radec2eleb(RA, Dec)`

*RA* `DOUBLE` : Object right ascension (J2000) in degrees;

*Dec* `DOUBLE` : Object declination (J2000) in degrees.

**Return value** (`STRING`):
Ecliptic longitude and latitude, in the range [0, 360[, [-90, 90] degrees.

**Example:**

```sql
mysql> select radec2eleb(320.85, 12.67) as lon_lat;
+-------------------------------------+
| 327.7266930717887, 26.5073679424105 |
+-------------------------------------+
```


### [ radecpm2ra ]
Compute right ascension at a given epoch given reference epoch, coordinates and proper motion.

**Syntax:**
`radecpm2ra(RA, Dec, pmRA, pmDec, catEpoch, reqEpoch)`

*RA* `DOUBLE` : Right ascension at reference epoch, in degrees;

*Dec* `DOUBLE` : Declination at reference epoch, in degrees;

*pmRA* `DOUBLE` : Right ascension proper motion (pmRA x cos(Dec)) at reference epoch, in mas/year;

*pmDec* `DOUBLE` : Declination proper motion reference epoch, in mas/year;

*catEpoch* `DOUBLE` : Reference epoch (e.g. MJD);

*reqEpoch* `DOUBLE` : Requested epoch.

**Return value** (`DOUBLE`):
RA at requested epoch, in degrees.

**Example:**

```sql
mysql> select radecpm2ra(46.34573, -89.36417, 127.3, 349.2, 53371.5, 59006.5) as RA_now;
+----------+
| 46.39478 |
+----------+
```


### [ radecpm2dec ]
Compute declination at a given epoch given reference epoch, coordinates and proper motion.

**Syntax:**
`radecpm2dec(RA, Dec, pmRA, pmDec, catEpoch, reqEpoch)`

*RA* `DOUBLE` : Right ascension at reference epoch, in degrees;

*Dec* `DOUBLE` : Declination at reference epoch, in degrees;

*pmRA* `DOUBLE` : Right ascension proper motion (pmRA x cos(Dec)) at reference epoch, in mas/year;

*pmDec* `DOUBLE` : Declination proper motion reference epoch, in mas/year;

*catEpoch* `DOUBLE` : Reference epoch (e.g. MJD);

*reqEpoch* `DOUBLE` : Requested epoch.

**Return value** (`DOUBLE`):
Dec at requested epoch, in degrees.

**Example:**

```sql
mysql> select radecpm2dec(46.34573, -89.36417, 127.3, 349.2, 53371.5, 59006.5) as Dec_now;
+-----------+
| -89.36267 |
+-----------+
```


### [ radecpmnow ]
Compute right ascension and declination at a given epoch given reference epoch, coordinates and proper motion.

**Syntax:**
`radecpmnow(RA, Dec, pmRA, pmDec, catEpoch, reqEpoch)`

*RA* `DOUBLE` : Right ascension at reference epoch, in degrees;

*Dec* `DOUBLE` : Declination at reference epoch, in degrees;

*pmRA* `DOUBLE` : Right ascension proper motion (pmRA x cos(Dec)) at reference epoch, in mas/year;

*pmDec* `DOUBLE` : Declination proper motion reference epoch, in mas/year;

*catEpoch* `DOUBLE` : Reference epoch (e.g. MJD);

*reqEpoch* `DOUBLE` : Requested epoch.

**Return value** (`STRING`):
RA and Dec at requested epoch, in degrees.

**Example:**

```sql
mysql> select radecpmnow(46.34573, -89.36417, 127.3, 349.2, 53371.5, 59006.5);
+-----------------------------------------------------------------+
| 46.39477549330486, -89.36267327096959                           |
+-----------------------------------------------------------------+
```


### [ rahstr ]
Encode into a `hh:mm:ss.ss` formatted string RA decimal hours. Separator other than the ":" can be given as an optional parameter.

**Syntax:**
`rahstr(RA_hrs, ['Separator'])`

*RA_hrs* `DOUBLE` : Right ascension as fractional hours;

*Separator* `STRING` (optional): Separator to use instead of ":". Use '' or ' ' for blank.

**Return value** (`STRING`):
RA in the format `hh:mm:ss.ss`.

**Example:**

```sql
mysql> select rahstr(12.7891) as RAs;
+-------------+
| 12:47:20.76 |
+-------------+

mysql> select rahstr(12.7891, '') as RAs;
+-------------+
| 12 47 20.76 |
+-------------+
```


### [ decstr ]
Encode into a `+dd:mm:ss.s` formatted string Dec decimal degrees. Separator other than the ":" can be given as an optional parameter.

**Syntax:**
`decstr(Dec_deg, ['Separator'])`

*Dec_deg* `DOUBLE` : Declination as fractional degrees;

*Separator* `STRING` (optional): Separator to use instead of ":". Use '' or ' ' for blank.

**Return value** (`STRING`):
Dec in the format `+dd:mm:ss.s`.

**Example:**

```sql
mysql> select decstr(-37.289) as Decs;
+-------------+
| -37:17:20.4 |
+-------------+

mysql> select decstr(-37.289, '') as Decs;
+-------------+
| -37 17 20.4 |
+-------------+
```


### [ rahdecstr ]
Encode into a `hh:mm:ss.ss, +dd:mm:ss.s` formatted string RA and Dec. Separator other than the ":" can be given as an optional parameter.

**Syntax:**
`rahdecstr(RA_hrs, Dec_deg, ['Separator'])`

*RA_hrs* `DOUBLE` : Right ascension as fractional hours;

*Dec_deg* `DOUBLE` : Declination as fractional degrees;

*Separator* `STRING` (optional): Separator to use instead of ":". Use '' or ' ' for blank.

**Return value** (`STRING`):
RA and Dec in the format `hh:mm:ss.ss, +dd:mm:ss.s`.

**Example:**

```sql
mysql> select rahdecstr(12.7891, -37.289) as RA_Decs;
+--------------------------+
| 12:47:20.76, -37:17:20.4 |
+--------------------------+

mysql> select decstr(12.7891, -37.289, '') as RA_Decs;
+--------------------------+
| 12 47 20.76, -37 17 20.4 |
+--------------------------+
```


### [ mjd2datet ]
Encode MJD into a ISO 8601 formatted string: `yyy-mm-ddThh:mm:ss`.
E.g. `2006-08-07T12:15:11`.

**Syntax:**
`mjd2datet(MJD)`

*MJD* `DOUBLE` : Modified Julian Date;

**Return value** (`STRING`):
ISO 8601 calendar date and time.

**Example:**

```sql
mysql> select mjd2datet(59010.768) as Date_time;
+---------------------+
| 2020-06-10T18:25:55 |
+---------------------+
```


### [ mjd2datetf ]
Encode MJD into a formatted string: `yyy-mm-ddThh:mm:ss.sss`.
E.g. `2006-08-07T12:15:11`.

**Syntax:**
`mjd2datetf(MJD)`

*MJD* `DOUBLE` : Modified Julian Date;

**Return value** (`STRING`):
Calendar date and time with milliseconds.

**Example:**

```sql
mysql> select mjd2datetf(59010.768) as Date_time;
+-------------------------+
| 2020-06-10T18:25:55.200 |
+-------------------------+
```
