#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <pthread.h>
#include "matrix.h"

//addressing for a normal array like it's 2d
#define array(arr, i, j) arr[(int) N * (int) i + (int) j]

static int *mc;

void fill_matrix(int *matrix);
int *matrix_page(int *matrix, unsigned long m_size);
void matrix_unmap(int *matrix, unsigned long m_size);
void *row_multiply(void *row_args);

typedef struct thread_args{
    int *a;
    int *b;
    int *c;
    int row;
} thread_args;

int *matrix_page(int *matrix, unsigned long m_size){
    matrix = mmap(0, m_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if(matrix == (void *) -1){
        exit(EXIT_FAILURE);
    }
    memset((void *) matrix, 0, m_size);
    return matrix;
}

void matrix_unmap(int *matrix, unsigned long m_size){
    if(munmap(matrix, m_size)==-1){
        exit(EXIT_FAILURE);
    }
}

void* row_multiply(void *row_args){
    thread_args *args = (thread_args *) row_args;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            int add = array(args->a, args->row, j) * array(args->b, j, i);
            array(args->c, args->row, i) +=add;
        }
    }
    pthread_exit(0);
}

int *threadFastMultiply(void){
    unsigned long m_size = sizeof(int) * (unsigned long) (N*N);
    //int numThreads = 2;
    mc = matrix_page(mc, m_size);
    
    pthread_t *thrs;
    thread_args *args;
    if((thrs = malloc(sizeof(pthread_t) * (unsigned long) N))==NULL || 
            (args = malloc(sizeof(thread_args) * (unsigned long) N)) == NULL){
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < N; i++){
        args[i] = (thread_args){
            .a = *ma,
            .b = *mb,
            .c = mc,
            .row = i
        };
        pthread_create(&thrs[i], NULL, row_multiply, (void *) &args[i]);
    }

    for(int j = 0; j < N; j++){
        pthread_join(thrs[j], NULL);
    }

    if (thrs != NULL)
    {
        free(thrs);
        thrs = NULL;
    }
    if (args != NULL)
    {
        free(args);
        args = NULL;
    }
    //matrix_unmap(mc, m_size);

    return mc;
}
