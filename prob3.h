#include <immintrin.h>

#define NUM 1000000
#define SIZE 512

typedef double data_t;

data_t no_fma();
data_t option_fma();

void init_matrix(data_t *mat);
