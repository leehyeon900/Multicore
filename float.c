#include <stdio.h>
#include <stdint.h>

#define ENT (printf("\n"))
#define SIZE 32

typedef union{
	uint32_t u;
	float f;
}data_t;


int main(){
	data_t num, tmp;
	printf("Enter a number: ");
	scanf("%f", &num.f);

	tmp = num;
	for (int i = 0; i < SIZE; i++) {
		printf("%u", (tmp.u >> (SIZE-1)) );
		tmp.u <<= 1;
	}
	ENT;

	printf("%f\n", num.f);

	return 0;
}
