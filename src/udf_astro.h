//Argument to  *_init functions
#define INIT_ARGS UDF_INIT*, UDF_ARGS*, char*

//Argument to main functions
#define MAIN_ARGS UDF_INIT*, UDF_ARGS*, char*, char*

//Argument to main functions: char
#define MAIN_ARGS_CHAR UDF_INIT*, UDF_ARGS*, char*, unsigned long*, char*, char*

//Argument to *_deinit functions
#define DEINIT_ARGS UDF_INIT*


#define DEFINE_FUNCTION(RETTYPE, NAME) \
my_bool  NAME ## _init  (INIT_ARGS);       \
RETTYPE  NAME           (MAIN_ARGS);       \
void     NAME ## _deinit(DEINIT_ARGS);

#define DEFINE_FUNCTION_CHAR(RETTYPE, NAME) \
my_bool  NAME ## _init  (INIT_ARGS);        \
RETTYPE  NAME           (MAIN_ARGS_CHAR);   \
void     NAME ## _deinit(DEINIT_ARGS);


#define CHECK_ARG_NUM(NUM)         \
  if (args->arg_count != NUM) {    \
    strcpy(message, argerr);       \
    return 1;                      \
  }


#define CHECK_ARG_TYPE(NUM, TYPE)     \
  if (args->arg_type[NUM] != TYPE) {  \
    strcpy(message, argerr);          \
    return 1;                         \
  }


#define CHECK_ARG_NOT_TYPE(NUM, TYPE) \
  if (args->arg_type[NUM] == TYPE) {  \
    strcpy(message, argerr);          \
    return 1;                         \
  }


#define ISNULL(A)  (!((bool) args->args[A]))
#define CHECK_AND_RETURN_NULL(A) if (! args->args[A]) { *is_null = 1; return 0;  }

#define IARGS(A) Conv_int(args->args[A], args->arg_type[A])
#define DARGS(A) Conv_double(args->args[A], args->arg_type[A])
#define CARGS(A) ((char*) args->args[A])

long long int Conv_int(void* p, enum Item_result type)
{
  long long int ret;

  switch (type) {
  case STRING_RESULT:
  case DECIMAL_RESULT:
    if (sscanf((char*) p, "%lld", &ret) != 1)
	return 0;
    break;
  case INT_RESULT:
    ret = (*((long long*) p));
    break;
  case REAL_RESULT:
    ret = (long long int) rint(*((double*) p));
    break;
  default:
    return 0;
  }

  return ret;
}


double Conv_double(void* p, enum Item_result type)
{
  double ret;

  switch (type) {
  case STRING_RESULT:
  case DECIMAL_RESULT:
    if (sscanf((char*) p, "%lf", &ret) != 1)
	return 0;
    break;
  case INT_RESULT:
    ret = (*((long long*) p));
    break;
  case REAL_RESULT:
    ret = (*((double*) p));
    break;
  default:
    return 0;
  }

  return ret;
}



#define CLEAR *is_null = 0; *error = 0;


// External functions
extern double skysep_h( double theta1, double phi1, double theta2, double phi2 );
extern int radec2GlGb(const double ra, const double dec, double *gl, double *gb);
extern double radec2Gl( const double ra, const double dec );
extern double radec2Gb( const double ra, const double dec );
extern int radec2ElEb(const double ra, const double dec, double *el, double *eb);
extern double radec2El( const double ra, const double dec );
extern double radec2Eb( const double ra, const double dec );
extern double myepoch_coords_ra( const double ra, const double de, const double pmra, const double pmde,
	const double epoch0, const double epoch1 );
extern double myepoch_coords_dec( const double ra, const double de, const double pmra, const double pmde,
	const double epoch0, const double epoch1 );
extern void myepoch_coords( const double ra, const double de, const double pmra, const double pmde,
	const double epoch0, const double epoch1, double *ra1, double *de1 );
extern char *enc_str_rah(double rahr, const char* separator);
extern char *enc_str_decdeg(double decdeg, const char* separator);
extern char *mjd2date(double mjd);
extern char *mjd2datef(double mjd);
extern double deg_ra(const char *ra_str);
extern double deg_dec(const char *dec_str);
