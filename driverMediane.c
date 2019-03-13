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

float * calcul_mediane(float ** tab, int taille)
{
	int i, j, k;
	double tmp;
	float * tabMed=calloc(taille, sizeof(float));
	//~ printf("avant : \n");
	//~ for(k=0; k<taille; k++)
	//~ {
		//~ for(i=0; i<NB_METAS; i++)
			//~ printf("%f;", tab[k][i]);
		//~ printf("\n");
	//~ }
	
	//~ printf("\naprès : \n");
	for(k=0; k<taille; k++)
	{
		//tri du tableau : il faut avoir un tableau trie des resultats des 31 metarepetitions faites pour la k eme fois de l'execution de baseline
		for(i=NB_METAS; i>1; i--)
		{
			for(j=0; j<i-1; j++)
			{
				if(tab[k][j+1]<tab[k][j])
				{
					tmp=tab[k][j];
					tab[k][j]=tab[k][j+1];
					tab[k][j+1]=tmp;
				}
			}
		}
		//~ for(i=0; i<NB_METAS; i++)
			//~ printf("%f;", tab[k][i]);
		//~ printf("\n");
	
	
		//determinisation de la mediane
		if(NB_METAS%2==0)
		{
			tabMed[k]=(tab[k][NB_METAS/2-1]+tab[k][NB_METAS/2])/2;//moyenne des deux valeurs encadrant la mediane
		}
		else
			tabMed[k]=tab[k][(NB_METAS-1)/2];
	}
	return tabMed;
	
}

float calcul_mediane_total(float * tab, int taille)
{
	int i, j;
	float rep, tmp;
	//tri du tableau
	for(i=taille; i>1; i--)
	{
		for(j=0; j<i-1; j++)
		{
			if(tab[j+1]<tab[j])
			{
				tmp=tab[j];
				tab[j]=tab[j+1];
				tab[j+1]=tmp;
			}
		}
	}
	if(NB_METAS%2==0)
	{
		rep=(tab[taille/2-1]+tab[taille/2])/2;//moyenne des deux valeurs encadrant la mediane
	}
	else
		rep=tab[(taille-1)/2];
		
	return rep;
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

   float ** tabMediane=calloc(repw+repm, sizeof(float*));
   for(i=0; i<repw+repm; i++)
		tabMediane[i]=calloc(NB_METAS, sizeof(float));


   for (m=0; m<NB_METAS; m++) {
      /* allocate arrays */
      complex_t *a = malloc (size * sizeof a[0]);
      complex_t *b = malloc (size * sizeof b[0]);
      
      /* init arrays */
      srand(0);
      init_array (size, a);
      init_array (size, b);

      /* warmup (repw repetitions in first meta, 1 repet in next metas) */
     // if (m == 0) {
     uint64_t t1=0, t2=0; 
         for (i=0; i<repw; i++)
         {
			  t1 = rdtsc();
            //sgemm (size, a, b, c);
            baseline (size , a , b );
             t2 = rdtsc();
         tabMediane[i][m]=(t2 - t1) / ((float) size * (repw+repm));
		
		}
      /* measure repm repetitions */
      for (i=0; i<repm; i++)
      {
		  t1 = rdtsc();
         baseline (size , a , b );
         t2 = rdtsc();
         tabMediane[repw+i][m]=(t2 - t1) / ((float) size * (repw+repm));
		
	 }
      
      /* print performance */
      printf ("%.2f cycles/FMA\n",(t2 - t1) / ((float) size * repm));
     
	
      /* print output */
      //if (m == 0) print_array (n, c);

      /* free arrays */
      free (a);
      free (b);
   }
	float * tab=calcul_mediane(tabMediane, repw+repm);
	printf("taille mediane : \n");
	for(i=0; i<repw+repm; i++)
	{
		printf("%f\n", tab[i]);
	}
	float med=calcul_mediane_total(tab, repw+repm);
	
	printf("mediane des medianes : %f\n min des medianes=%f\n", med, tab[0]);
   return EXIT_SUCCESS;
}
