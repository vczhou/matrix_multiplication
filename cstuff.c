#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "matrix.h"

//Computes matrix multiplication
int* multiply() {
    static int c[N][N];

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            c[i][j] = 0;
        }
    }

    register int blockSize = 32;
    int sum = 0;
    for(int i = 0; i < N; i += blockSize){
        for(int j = 0; j < N; j += blockSize){
                for(int x = 0; x < N; x++){
                    for(int y = i; y < i + blockSize && y < N; y++){
                            sum = c[x][y];
                        for(int z = j; z < j + blockSize && z < N; z++){
                            sum += (ma[x][z] * mb[z][y]);
                        }
                        c[x][y] = sum;
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
