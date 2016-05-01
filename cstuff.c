#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "matrix.h"

//Computes matrix multiplication
int* multiply(){
    static int c[N][N];
    memset(*c, 0, (sizeof(int) * N * N));
    register int blockSize = 8;

    for(int i = 0; i < N; i += blockSize){
        for(int j = 0; j < N; j += blockSize){
            for(int k = 0; k < N; k += blockSize){
                for(int x = i; (x < N) && (x < i + blockSize); x++){
                    for(int y = j; (y < N) && (y < j + blockSize); y++){
                        for(int z = k; (z < N) && (z < k + blockSize); z++){
                            c[x][y] += ma[x][z] * mb[z][y];
                        }
                    }
                }
            }
        }
    }
    
    return *c;
}

int* naive(void){
    static int c[N][N];

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            c[i][j] = 0;
        }
    }

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            for(int k = 0; k < N; k++){
                c[i][j] = c[i][j] + (ma[i][k] * mb[k][j]);
            }
        }
    }
    return *c;
}
