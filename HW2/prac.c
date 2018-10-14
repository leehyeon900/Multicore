#include <stdio.h>
#define VBYTES 32
typedef float data_t;
#define VSIZE VBYTES/sizeof(data_t)
typedef data_t vec_t __attribute__ ((vector_size(VBYTES)));
#define IDENT 0

int main() {
    vec_t a, b, c;
    data_t res = IDENT;
    for (int i = 0; i < VSIZE; i++) {
        a[i] = 1;
        b[i] = 2;
        c[i] = 0;
    }

    for (int i = 0; i < VSIZE; i++)
        printf("a[i] : %.2f\n", a[i]);

    c = a * b;
    for (int i = 0; i < VSIZE; i++)
        res += c[i];

    printf("res : %.2f\n", res);
    return 0;
}
