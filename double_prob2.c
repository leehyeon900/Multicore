#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "prob2.h"

typedef double data_t;

void double_op() {
	data_t num;
	data_t val1, val2;
	clock_t start, end;

	val1 = (data_t)rand() / RAND_MAX;
	val2 = (data_t)rand() / RAND_MAX;

	start = clock();
	for (int i = 0; i < NUM; i++){
		num = val1 OP1 val2;
	}
	end = clock();
	printf("%.1lf", num);
	PRT_TIME(Addition);

	start = clock();
	for (int i = 0; i < NUM; i++){
		num = -val1 OP2 val2;
	}
	end = clock();
	printf("%.1lf", num);
	PRT_TIME(Multiplication);
}
