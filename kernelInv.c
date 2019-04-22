#include "projetInv.h"

void baseline (int n , complex_t a) {
	int i ;
	for ( i =0; i < n ; i ++) {
		a.re[i] = a.re[i] + a.im[i] ;
		a.im[i] = a.im[i] * a.im [i];
	}
}

