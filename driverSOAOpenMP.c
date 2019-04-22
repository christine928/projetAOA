#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>
#include "projetInv.h"

#define NB_METAS 31

extern uint64_t rdtsc ();

extern void baseline (int n , complex_t a );

static void init_array (int n, complex_t a) {
	int i;
	omp_set_num_threads(4);
	
	#pragma omp parallel for
	for (i=0; i<n; i++)
	{
		a.re[i]= (float) rand() / RAND_MAX;
		a.im[i]= (float) rand() / RAND_MAX;
	}
         
}

static void print_array (int n, complex_t a) {
   int i;

   for (i=0; i<n; i++)
         printf ("re : %f\tim : %f\n", a.re[i], a.im[i]);
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
	omp_set_num_threads(4);

   for (m=0; m<NB_METAS; m++) {
      /* allocate arrays */
      complex_t a;
      a.re = malloc (size * sizeof (float));
      a.im = malloc (size * sizeof (float));

      
      /* init arrays */
      srand(0);
      init_array (size, a);

      /* warmup (repw repetitions in first meta, 1 repet in next metas) */
      if (m == 0) {
         for (i=0; i<repw; i++)
            baseline (size , a);
      } else {
         baseline (size , a);
      }

      /* measure repm repetitions */
      uint64_t t1 = rdtsc();
      #pragma omp parallel for
      for (i=0; i<repm; i++)
         baseline (size , a);
      uint64_t t2 = rdtsc();

      /* print performance */
      printf ("%.2f\n", (t2 - t1) / ((float) size * repm));

  

      /* free arrays */
      free (a.re);
      free(a.im);
   }

   return EXIT_SUCCESS;
}
