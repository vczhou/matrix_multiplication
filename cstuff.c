#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "matrix.h"

//Computes matrix multiplication
int* multiply() {
    static int c[N][N];
    int sum = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            sum = 0;
            for(int k = 0; k < N; k++){
                sum += ma[i][k] * mb[k][j];
            }
            c[i][j] = sum;
        }
    }
    return *c;
}
