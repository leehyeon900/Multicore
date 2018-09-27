#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "prob2.h"

void float_op();
void double_op();

int main() {
	srand(time(NULL));
	
	printf("float case\n");
	float_op();
	
	printf("\ndouble case\n");
	double_op();
	
	return 0;
}
