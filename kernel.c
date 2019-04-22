#include "projet.h"

void baseline (int n , complex_t a [ n ] , complex_t b [ n ]) {
	int i ;
	for ( i =0; i < n ; i ++) {
		a [ i ]. re = a [ i ]. re + b [ i ]. im ;
		b [ i ]. im = b [ i ]. im * b [ i ]. im ;
	}
}
