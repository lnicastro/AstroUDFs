/*
 * TESTING z2ld ...
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double DCMT(double DCMR, double WK);
double z2ld(const float z);


int main(int argc, char **argv)
{
  if (argc < 2)
  {
	printf("%s z\n", argv[0]);
	return 0;
  }

  float z;
  sscanf(argv[1], "%f", &z );
  printf("Luminosity distance: %.4lf\n", z2ld(z)); 
}
