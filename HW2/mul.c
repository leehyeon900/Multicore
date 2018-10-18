
//====================================================================
// a[N][M] x b[M][N] = c[N][N], reversed??
//====================================================================
#include <immintrin.h>
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



static void *cal_mat_avx(void *arg) {
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
            int a_idx = i * M + k;
            __m256 a_vec = _mm256_broadcast_ss(a + a_idx);
            //float a_mat = a[i * M + k];
            for (int j = 0; j < N; j+=32) {
                int c_idx = i * N + j;
                int b_idx = k * N + j;
                __m256 c_vec = _mm256_loadu_ps(c + c_idx);
                __m256 c_vec1 = _mm256_loadu_ps(c + c_idx + 8);
                __m256 c_vec2 = _mm256_loadu_ps(c + c_idx + 16);
                __m256 c_vec3 = _mm256_loadu_ps(c + c_idx + 24);
                
                __m256 b_vec = _mm256_loadu_ps(b + b_idx);
                __m256 b_vec1 = _mm256_loadu_ps(b + b_idx + 8);
                __m256 b_vec2 = _mm256_loadu_ps(b + b_idx + 16);
                __m256 b_vec3 = _mm256_loadu_ps(b + b_idx + 24);
                
                __m256 tmp = _mm256_mul_ps(a_vec, b_vec);
                __m256 tmp1 = _mm256_mul_ps(a_vec, b_vec1);
                __m256 tmp2 = _mm256_mul_ps(a_vec, b_vec2);
                __m256 tmp3 = _mm256_mul_ps(a_vec, b_vec3);

                c_vec = _mm256_add_ps(c_vec, tmp);
                c_vec1 = _mm256_add_ps(c_vec1, tmp1);
                c_vec2 = _mm256_add_ps(c_vec2, tmp2);
                c_vec3 = _mm256_add_ps(c_vec3, tmp3);

                _mm256_storeu_ps(c + c_idx, c_vec);
                _mm256_storeu_ps(c + c_idx + 8, c_vec1);
                _mm256_storeu_ps(c + c_idx + 16, c_vec2);
                _mm256_storeu_ps(c + c_idx + 24, c_vec3);
                //c[c_idx] += a_mat * b[b_idx];
            }
        }
    }

    pthread_exit(NULL);
}
/*
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
            for (int j = 0; j < N; j+=4) {
                int c_idx = i * N + j;
                int b_idx = k * N + j;
                c[c_idx] += a_mat * b[b_idx];
                c[c_idx+1] += a_mat * b[b_idx+1];
                c[c_idx+2] += a_mat * b[b_idx+2];
                c[c_idx+3] += a_mat * b[b_idx+3];
            }
        }
    }

    pthread_exit(NULL);
}
*/

void mat_mul(float *a, float *b, float *c, int N, int M) {
    thread_ptr tinfo;
    int offset, junk;
    void *status;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    tinfo = (thread_ptr)malloc(NUM_THREADS * sizeof(thread_info));
    if (tinfo == NULL) {
        fprintf(stderr, "mat_mul(): tinfo allocation ERROR\n");
        exit(EXIT_FAILURE);
    }

    offset = N / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        tinfo[i].thread_id = 0;
        tinfo[i].a = a;
        tinfo[i].b = b;
        tinfo[i].c = c;
        tinfo[i].start = i * offset;
        tinfo[i].end = i * offset + offset;
        tinfo[i].N = N;
        tinfo[i].M = M;
        junk = pthread_create(&tinfo[i].thread_id, &attr, 
        &cal_mat_avx, (void *)&tinfo[i]);
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
        float sum = 0.0f;
      for (int k = 0; k < M; k++) {
        sum += a[i * M + k] * b[k * N + j];
      }
      c[i * N + j] = sum;
    }
  }
}
