#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "projet.h"
#include <unistd.h>
#include <string.h>
#include <math.h>



#define NB_METAS 31

extern uint64_t rdtsc ();

//extern void sgemm (int n, float a[n][n], float b[n][n], float c[n][n]);
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
	//uint64_t x = *((uint64_t*)a);
	//uint64_t y = *((uint64_t*)b);
	//if(x>y) return 1;
	//if(x<y) return -1;
	//return 0;
	uint64_t  const *x=a;
	uint64_t  const *y=b;
	return *x-*y;
}
float main_prof (int argc, char ** argv) {
   /* check command line arguments */
   //~ if (argc != 4) {
      //~ fprintf (stderr, "Usage: %s <size> <nb warmup repets> <nb measure repets>\n", argv[0]);
      //~ abort();
   //~ }

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
      //printf("84 %.2f\n", tab[m]);
      //printf ("%.2f cycles/itération\n",(t2 - t1) / ((float) size * repm));

      /* print output */
      //if (m == 0) print_array (n, c);

      /* free arrays */
      free (a);
      free (b);
   }
   qsort(tab,NB_METAS,sizeof(float),compare);
  // printf("95!!!!!!!!!!!\n");
  /* for(m=0; m<NB_METAS; m++)
	printf("97 %.2f\n", tab[m]);*/
	printf("%.2f;%.2f;%.2f\n", tab[0], tab[(NB_METAS-1)/2], tab[NB_METAS-1]);
   sleep(5);

   return tab[(NB_METAS-1)/2];
}
int main(int argc,char *argv[])
{
	 if (argc != 4) {
      fprintf (stderr, "Usage: %s <size> <nb measure repets> <how many repw>\n", argv[0]);
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
	arg[3]=calloc(strlen(argv[2]), sizeof(char));
	strncpy(arg[3],argv[2], strlen(argv[2])*sizeof(char));//repm
	
	arg[2]=calloc(floor(log(nbre))+1, sizeof(char)); //floor(log(nbre))+1=>nbre de chiffres de nbre
	float *new_tab=calloc(nbre,sizeof(float));
	for(i=0;i<nbre;i++)
	{
		//char car=i+'0';
		//itoa(i, arg[2], 10);
		sprintf(arg[2], "%d", i);
		//strcpy(arg[2], &car);
		//printf("on appelle : %s, %s, %s, %s\n", arg[0],arg[1],arg[2],arg[3]);
		new_tab[i]=main_prof(argc, arg);
		//printf("%.2f \n",new_tab[i]);
	}
}
