#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "prob3.h"

data_t no_fma(){
	data_t A[SIZE], B[SIZE], C[SIZE];
	data_t tmp, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
	clock_t start, end;

	init_matrix(A);
	init_matrix(B);
	for (int i = 0; i < SIZE; i++)
		C[i] = 0;
	
	/* Calculate dot product w/o FMA */
	start = clock();
	for (int i = 0; i < NUM; i++){
		tmp = 0;
		for (int j = 0; j < SIZE; j++)
			tmp = A[j] * B[j] + tmp;
	}
	end = clock();

	printf("Dot product A dot B : %lf\n", tmp);
	printf("Time spent w/o FMA instruction : %lf[s]\n", (double)(end - start) / CLOCKS_PER_SEC);


	start = clock();
	for (int i = 0; i < NUM; i++){
		tmp = tmp1 = tmp2 = tmp3 = tmp4 = tmp5 = tmp6 = tmp7 = 0;
		for (int j = 0; j < SIZE; j+=8){
			tmp = A[j] * B[j] + tmp;
			tmp1 = A[j+1] * B[j+1] + tmp1;
			tmp2 = A[j+2] * B[j+2] + tmp2;
			tmp3 = A[j+3] * B[j+3] + tmp3;
			tmp4 = A[j+4] * B[j+4] + tmp4;
			tmp5 = A[j+5] * B[j+5] + tmp5;
			tmp6 = A[j+6] * B[j+6] + tmp6;
			tmp7 = A[j+7] * B[j+7] + tmp7;
		}
	}
	end = clock();

	data_t sum = tmp + tmp1 + tmp2 + tmp3 + tmp4 + tmp5 + tmp6 + tmp7;

	printf("Dot product A dot B : %lf\n", sum);
	printf("Time spent (8X8 unrolling) : %lf[s]\n", (double)(end - start) / CLOCKS_PER_SEC);

	start = clock();
	for (int i = 0; i < NUM; i++){
		for (int j = 0; j < SIZE; j++)
			C[j] = A[j] * B[j] + C[j];
	}
	end = clock();

	tmp = 0;
	for (int i = 0; i < SIZE; i++)
		tmp += C[i];	
	end = clock();

	printf("Dot product A dot B : %lf\n", tmp / NUM);
	printf("Time spent considering data dependecy : %lf[s]\n", (double)(end - start) / CLOCKS_PER_SEC);


	return tmp / NUM;
}
