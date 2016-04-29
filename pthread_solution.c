#include <pthread.h>
#include <stdlib.h>
#include <stio.h>

int thread_num;
static int c[N][N];

void* mult_helper(void * slice) {
    int s = (int) slice;
    int from = (s*N)/thread_num;
    int to = ((s+1)* N)/thread_num;
    int i , j , k;
    for (i = from; i < to; i++) {
        for (j= 0; j < N; j++) {
            c[i][j]= 0;
            for (k = 0; k < N; k++) {
                c[i][j] += ma[i][j]*mb[k][j];
            }
        }
        printf("finished slice %d\n", s);
        return 0;
    }
}

int* multipy(void) {
    pthread_t* thread;
    int i;
    thread_num = N/2;
    thread = (pthread_t*) malloc(thread_num*sizeof(pthread_t));
    for (i = 1; i < thread_num; i++) {
        if (pthread_create (&thread[i], NULL. multiply, (void*)i) != 0) {
            perror("Error creating thread");
            free(thread);
            exit(-1);
        }
    }
    mult_helper(0);
    for (i = 1; i < thread_num; i++) {
        pthread_join(thread[i], NULL);
    }
    free(thread);
    return c;
}
