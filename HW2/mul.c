
//====================================================================
// a[N][M] x b[M][N] = c[N][N], reversed??
//====================================================================
#include <x86intrin.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 16

typedef struct {
    pthread_t thread_id;
    float *a;
    float *b;
    float *c;
    int start;
    int end;
    int N;
    int M;
}thread_info, *thread_ptr;

//static pthread_t threads[NUM_THREADS];
static pthread_mutex_t mutex;

static void *cal_mat(void *arg) {
    thread_ptr tinfo  = (thread_ptr)arg;
    float *a = tinfo->a;
    float *b = tinfo->b;
    float *c = tinfo->c;
    int start = tinfo->start;
    int end = tinfo->end;
    int N = tinfo->N;
    int M = tinfo->M;

    for (int i = start; i < end; i++) {
        for (int k = 0; k < M; k++) {
            float a_mat = a[i * M + k];
            for (int j = 0; j < N; j++) {
                c[i * N + j] += a_mat * b[k * N + j];
            }
        }
    }

    pthread_exit(NULL);
}




void mat_mul(float *a, float *b, float *c, int N, int M) {
    thread_ptr tinfo;
    int offset, junk;
    void *status;
    pthread_attr_t attr;

    /*
     * N, M % 32 == 0?
     */
    if ((N & 0x1f) || (M & 0x1f)) {
        fprintf(stderr, "mat_mul(): N, M are not multiples of 32\n");
        exit(EXIT_FAILURE);
    }


    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_mutex_init(&mutex, NULL);

    tinfo = (thread_ptr)malloc(NUM_THREADS * sizeof(thread_info));
    if (tinfo == NULL) {
        fprintf(stderr, "mat_mul(): tinfo allocation ERROR\n");
        exit(EXIT_FAILURE);
    }


    offset = N / NUM_THREADS;
    /*
     * Initialize data on each threads and create
     */
    for (int i = 0; i < NUM_THREADS; i++) {
        tinfo[i].thread_id = 0;
        tinfo[i].a = a;
        tinfo[i].b = b;
        tinfo[i].c = c;
        tinfo[i].start = i * offset;
        tinfo[i].end = i * offset + offset;
        tinfo[i].N = N;
        tinfo[i].M = M;
        junk = pthread_create(&tinfo[i].thread_id, &attr, &cal_mat, (void *)&tinfo[i]);
        if (junk) {
            fprintf(stderr, "mat_mul(): thread creation failed\n");
            exit(EXIT_FAILURE);
        }
    }
    pthread_attr_destroy(&attr);


    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(tinfo[i].thread_id, &status);
    }

    free(tinfo);
    pthread_mutex_destroy(&mutex);
}

void mat_mul_cache(float *a, float *b, float *c, int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < M; k++) {
            float a_mat = a[i * M + k];
            for (int j = 0; j < N; j++) {
                c[i * N + j] += a_mat * b[k * N + j];
            }
        }
    }
}

void mat_mul_original(float *a, float *b, float *c, int N, int M) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      for (int k = 0; k < M; k++) {
        c[i * N + j] += a[i * M + k] * b[k * N + j];
      }
    }
  }
}
