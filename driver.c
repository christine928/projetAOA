#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 #include "projet.h"

#define NB_METAS 31

extern uint64_t rdtsc ();

//extern void sgemm (int n, float a[n][n], float b[n][n], float c[n][n]);
extern void baseline (int n , complex_t a [ n ] , complex_t b [ n ]);

static void init_array (int n, complex_t * a) {
	int i;

	for (i=0; i<n; i++)
	{
		a[i].re= (float) rand() / RAND_MAX;
		a[i].im= (float) rand() / RAND_MAX;
	}
         
}

static void print_array (int n, complex_t * a) {
   int i;

   for (i=0; i<n; i++)
         printf ("re : %f\tim : %f\n", a[i].re, a[i].im);
}

int main (int argc, char *argv[]) {
   /* check command line arguments */
   if (argc != 4) {
      fprintf (stderr, "Usage: %s <size> <nb warmup repets> <nb measure repets>\n", argv[0]);
      abort();
   }

   int i, m;

   /* get command line arguments */
   int size = atoi (argv[1]); /* table size */
   int repw = atoi (argv[2]); /* repetition number */
   int repm = atoi (argv[3]); /* repetition number */

   for (m=0; m<NB_METAS; m++) {
      /* allocate arrays */
      complex_t *a = malloc (size * sizeof a[0]);
      complex_t *b = malloc (size * sizeof b[0]);
      
      /* init arrays */
      srand(0);
      init_array (size, a);
      init_array (size, b);

      /* warmup (repw repetitions in first meta, 1 repet in next metas) */
      if (m == 0) {
         for (i=0; i<repw; i++)
            //sgemm (size, a, b, c);
            baseline (size , a , b );
      } else {
         baseline (size , a , b );
         //sgemm (size, a, b, c);
      }

      /* measure repm repetitions */
      uint64_t t1 = rdtsc();
      for (i=0; i<repm; i++)
         baseline (size , a , b );
         //sgemm (size, a, b, c);
      uint64_t t2 = rdtsc();

      /* print performance */
      printf ("%.2f cycles/FMA\n",
              (t2 - t1) / ((float) size * repm));

      /* print output */
      //if (m == 0) print_array (n, c);

      /* free arrays */
      free (a);
      free (b);
   }

   return EXIT_SUCCESS;
}
