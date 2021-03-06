/*
 * used these as references to learn about multithreading :
 * https://zhaowuluo.wordpress.com/2011/01/03/pthread-atrix-multiplication-on-unixlinux/
 * https://gist.github.com/rhinoceraptor/33935694dc47608861bc
 * http://www.yolinux.com/TUTORIALS/LinuxTutorialPosixThreads.html
 * */
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "matrix.h"

int thread_num;
static int c[N][N];

/* Multi-threading with blocking */
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

	/* Initialize product matrix to 0 */
    memset(*c, 0, (sizeof(int) * N * N));

	/* Create each thread */
    for (int i = 0; i < thread_num; i++) {
        param[i] = i;
        pthread_create (&thread[i], NULL, block_helper, (void *) &param[i]);
    }

	/* Join the threads together */
    for (int i = 0; i < thread_num; i++) {
        pthread_join(thread[i], NULL);
    }

    return *c;
}

/* Multi-threading no blocking*/
void* mult_helper(void * slice) {
    int s = *(int *) slice;
    int from = (s * N)/thread_num;
    int to = ((s + 1) *  N)/thread_num;
    int i , j , k;

    for (i = from; i < to; i++) {
        for (j= 0; j < N; j++) {
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

	/* Initialize product matrix to 0 */
    memset(*c, 0, (sizeof(int) * N * N));

	/* Create each thread */
    for (int i = 0; i < thread_num; i++) {
        param[i] = i;
        pthread_create (&thread[i], NULL, mult_helper, (void *) &param[i]);
    }

	/* Join the threads together */
    for (int i = 0; i < thread_num; i++) {
        pthread_join(thread[i], NULL);
    }

    return *c;
}
