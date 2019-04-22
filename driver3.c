#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "projet.h"
#include <unistd.h>
#include <string.h>
#include <math.h>

#define NB_METAS 31

extern uint64_t rdtsc ();

extern void baseline (int n , complex_t a [ n ] , complex_t b [ n ]);

static void init_array (int n, complex_t a[n]) {
   int i;

   for (i=0; i<n; i++)
         a[i].re = (float) rand() / RAND_MAX;
         a[i].im = (float) rand() / RAND_MAX;
}


static void print_array (int n, complex_t a[n]) {
   int i;

   for (i=0; i<n; i++)
         printf ("le re est %f et im est %f\n", a[i].re,a[i].im);
}

static int compare(const void *a, const void *b)
{
	uint64_t  const *x=a;
	uint64_t  const *y=b;
	return *x-*y;
}
float main_prof (int argc, char ** argv) {
 
   int i, m;

   /* get command line arguments */
   int size = atoi (argv[1]); /* matrix size */
   int repw = atoi (argv[2]); /* repetition number */
   int repm = atoi (argv[3]); /* repetition number */
	float*tab=calloc(NB_METAS,sizeof(float));
   for (m=0; m<NB_METAS; m++) {
      /* allocate arrays */
      complex_t *a = malloc (size  * sizeof(complex_t));
      complex_t *b = malloc (size  * sizeof(complex_t));

      /* init arrays */
      srand(0);
      init_array (size, a);
      init_array (size, b);

      /* warmup (repw repetitions in first meta, 1 repet in next metas) */
      if (m == 0) {
         for (i=0; i<repw; i++)
            baseline (size, a, b);
      } else {
         baseline (size, a, b);
      }

      /* measure repm repetitions */
      uint64_t t1 = rdtsc();
      for (i=0; i<repm; i++)
         baseline (size, a, b);
      uint64_t t2 = rdtsc();

      /* print performance */
      tab[m]=(t2 - t1) / ((float) size * repm);
      //printf ("%.2f cycles/itération\n",(t2 - t1) / ((float) size * repm));

      /* print output */
      //if (m == 0) print_array (n, c);
		//printf("tab=%.2f\n", tab[m]);
      /* free arrays */
      free (a);
      free (b);
   }
   qsort(tab,NB_METAS,sizeof(float),compare);
   
   printf("%.2f;%.2f;%.2f\n", tab[0], tab[(NB_METAS-1)/2], tab[NB_METAS-1]);
   
   sleep(5);

   return tab[(NB_METAS-1)/2];
}
int main(int argc,char *argv[])
{
	 if (argc != 4) {
      fprintf (stderr, "Usage: %s <size>  <repw> <how many measure repets>\n", argv[0]);
      abort();
   }
	int i=0;
	int nbre=atoi(argv[3]);
	char **arg;
	arg=calloc(4*sizeof(char*),1);
	arg[0]=calloc(strlen(argv[0]), sizeof(char));
	strncpy(arg[0],argv[0], strlen(argv[0])*sizeof(char));
	arg[1]=calloc(strlen(argv[1]), sizeof(char));
	strncpy(arg[1],argv[1], strlen(argv[1])*sizeof(char));
	arg[2]=calloc(strlen(argv[2]), sizeof(char));
	strncpy(arg[2],argv[2], strlen(argv[2])*sizeof(char));//repm
	
	arg[3]=calloc(floor(log(nbre))+1, sizeof(char)); //floor(log(nbre))+1=>nbre de chiffres de nbre
	float *new_tab=calloc(nbre,sizeof(float));
	for(i=0;i<nbre;i++)
	{
		sprintf(arg[3], "%d", i+1);//i+1 pour ne pas avoir de inf lors de la premiere execution
		new_tab[i]=main_prof(argc, arg);
		printf("%.2f \n",new_tab[i]);
	}
}
