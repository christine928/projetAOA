#include "projet.h"
#ifdef OPT1


/* Removing of store to load dependency (array ref replaced by scalar) */
/*void sgemm (int n, float a[n][n], float b[n][n], float c[n][n]) {
   int i, j, k;

   for (i=0; i<n; i++)
      for (j=0; j<n; j++) {
         float s = 0.0f;

         for (k=0; k<n; k++)
            s += a[i][k] * b[k][j];

         c[i][j] = s;
      }
}*/

#elif defined OPT2

#include <string.h> // memset

/* ijk -> ikj permutation to make stride 1 the innermost loop */
/*void sgemm (int n, float a[n][n], float b[n][n], float c[n][n]) {
   int i, j, k;

   memset (c, 0, n * n * sizeof c[0][0]);

   for (i=0; i<n; i++)
      for (k=0; k<n; k++)
         for (j=0; j<n; j++)
            c[i][j] += a[i][k] * b[k][j];
}*/

#else

/* original */

void baseline (int n , complex_t a [ n ] , complex_t b [ n ]) {
	int i ;
	for ( i =0; i < n ; i ++) {
		a [ i ]. re = a [ i ]. re + b [ i ]. im ;
		b [ i ]. im = b [ i ]. im * b [ i ]. im ;
	}
}
/*void sgemm (int n, float a[n][n], float b[n][n], float c[n][n]) {
   int i, j, k;

   for (i=0; i<n; i++)
      for (j=0; j<n; j++) {
         c[i][j] = 0.0f;

         for (k=0; k<n; k++)
            c[i][j] += a[i][k] * b[k][j];
      }
}*/
#endif
