#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "matrix.h"

int thread_num;
static int c[N][N];

void* block_helper(void * slice) {
    int s = *(int *) slice;
    int from = (s * N)/thread_num;
    int to = ((s + 1) *  N)/thread_num;
    int i , j , k, x, y, z;
    register int blockSize = 8;

    for(i = from ; i < to; i += blockSize){
        for(j = 0; j < N; j += blockSize){
            for(k = 0; k < N; k += blockSize){
                for(x = i; (x < to) && (x < i + blockSize); x++){
                    for(y = j; (y < N) && (y < j + blockSize); y++){
                        for(z = k; (z < N) && (z < k + blockSize); z++){
                            c[x][y] += ma[x][z] * mb[z][y];
                        }
                    }
                }
            }
        }
    }

    pthread_exit(0);
}

int* pthreadBlockMultiply(void) {
    pthread_t thread[thread_num];
    int param[thread_num];
    thread_num = 4;

    memset(*c, 0, (sizeof(int) * N * N));

    for (int i = 0; i < thread_num; i++) {
        param[i] = i;
        pthread_create (&thread[i], NULL, block_helper, (void *) &param[i]);
    }

    for (int i = 0; i < thread_num; i++) {
        pthread_join(thread[i], NULL);
    }

    return *c;
}

void* mult_helper(void * slice) {
    int s = *(int *) slice;
    int from = (s * N)/thread_num;
    int to = ((s + 1) *  N)/thread_num;
    int i , j , k;
    for (i = from; i < to; i++) {
        for (j= 0; j < N; j++) {
            //c[i][j]= 0;
            for (k = 0; k < N; k++) {
                c[i][j] += ma[i][k]*mb[k][j];
            }
        }
    }

    pthread_exit(0);
}

int* pthreadMultiply(void) {
    pthread_t thread[thread_num];
    int param[thread_num];
    thread_num = 4;

    memset(*c, 0, (sizeof(int) * N * N));

    for (int i = 0; i < thread_num; i++) {
        param[i] = i;
        pthread_create (&thread[i], NULL, mult_helper, (void *) &param[i]);
    }

    for (int i = 0; i < thread_num; i++) {
        pthread_join(thread[i], NULL);
    }

    return *c;
}
