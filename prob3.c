#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "prob3.h"

int main(){
	srand(time(NULL));

	no_fma();

	option_fma();

	return 0;
}


void init_matrix(data_t *mat){
	for (int i = 0; i < SIZE; i++)
		mat[i] = (data_t)rand() / RAND_MAX;
}

