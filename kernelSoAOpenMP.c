#include "projetInv.h"
#include <omp.h>

void baseline (int n , complex_t a) {
	int i ;
	omp_set_num_threads(4);
	#pragma omp parallel for
	for ( i =0; i < n ; i ++) {
		a.re[i] = a.re[i] + a.im[i] ;
		a.im[i] = a.im[i] * a.im [i];
	}
}

