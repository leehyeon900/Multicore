#include <stdio.h>
#include <stdint.h>

#define ENT (printf("\n"))
#define SIZE 64

typedef union{
	uint64_t u;
	double f;
}data_t;


int main(){
	data_t num, tmp;
	printf("Enter a number: ");
	scanf("%lf", &num.f);

	tmp = num;
	for (int i = 0; i < SIZE; i++) {
		printf("%llu", (tmp.u >> (SIZE-1)) );
		tmp.u <<= 1;
	}
	ENT;

	printf("%f\n", num.f);

	return 0;
}
