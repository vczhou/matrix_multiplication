/**
 * Acknowledgements
 * Thanks to Professor Gheith for providing the base code to calculate 
 * the frequency of functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>

#include "matrix.h"

int ma[N][N];
int mb[N][N];

double getFrequencyGHz() {
    /* use lscpu to get the CPU frequency in MHz. Only works on Linux */
    /* writes the frequency in freq.txt */
    int t = 0;
    for (int i=0; i<100000000; i++) {
        t += rand();
    }
    printf("t=%d\n",t);
    /*int rc = system("grep \"model name\" /proc/cpuinfo | sed -e \"s/.*@ //g\" | head -n 1 > freq.txt");*/
    int rc = system("cpupower frequency-info | grep 'available frequency steps: ' | sed -e s'/^.*: //' | sed -e 's/ .*//' > freq.txt");
    if (rc != 0) {
        perror("system");
        exit(-1);
    }

    /* open freq.txt */
    FILE *file = fopen("freq.txt","r");
    if (file == 0) {
        perror("open freq.txt");
        exit(-1);
    }

    /* read frequency from freq.txt */
    double freqGHz = 0;
    rc = fscanf(file,"%lf",&freqGHz);
    if (rc != 1) {
        perror("scanning file");
        exit(-1);
    }
    fclose(file);

    return freqGHz;
}

#define T 6

void report(uint64_t count, double cycleNS) {
    double nsPerThing = (T * 1e9) / count;
    double cyclesPerThing = nsPerThing / cycleNS;

    printf("    latency: %f ns %f cycles\n",nsPerThing,cyclesPerThing);
    printf("    rate: %f/ns %f/cycle\n",1/nsPerThing,1/cyclesPerThing);
}

volatile int done = 0;

void handler() {
    done = 1;
}

int* answer(int *a, int *b){
    static int c[N][N];
    int sum = 0;
    int aVal = 0;
    int bVal = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            sum = 0;
            for(int k = 0; k < N; k++){
                aVal = *(a + (i * N + k));
                bVal = *(b + (k * N + j));
                sum += aVal * bVal;
            }
            c[i][j] = sum;
        }
    }
    return *c;
}

int compare(int* actual, int* expected){
    int equal = 1;
    int actualVal = 0;
    int expectedVal = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            actualVal = *(actual + (i * N + j));
            expectedVal = *(expected + (i * N + j));
            if(actualVal != expectedVal){
                equal = 0;
                break;
            }
        }
    }
    return equal;    
}

void printMatrix(int* m){
    int actualVal = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            actualVal = *(m + (i * N + j));
            printf("%d ", actualVal);
        }
        printf("\n");
    }
}

void printAll(int *a, int *b, int *c){
    printf("Matrix a: \n");
    printMatrix(a);
    printf("Matrix b: \n");
    printMatrix(b);
    printf("Product: \n");
    printMatrix(c);
}

void printResults(double cycleNS, int count, int *actual, int* expected){
    report(((N * N) * count), cycleNS);

    printf("Count: %d\n", count);

    int equal = compare(actual, expected);
    if(equal)
        printf("Success!\n");
    else
        printf("Try again :)\n");
    
    //printAll(*ma, *mb, actual);
    //printf("Expected: \n");
    //printMatrix(expected);
}

void run(char* what, int* (*func)(), double cycleNS, int* expected) {
    uint64_t count = 0;
    done = 0;
    int *actual = 0;

    printf("%s\n",what);
    signal(SIGALRM,handler);
    alarm(T);
    
    while (!done) {
        actual = func();
        count++;
    }

    printResults(cycleNS, count, actual, expected);
}

void fillMatrices(){
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int x = rand() % 11;
            int y = rand() % 11;
            ma[i][j] = x;
            mb[i][j] = y;
        }
    }
}

int main(int argc, char* argv[]) {
    double freqGHz = getFrequencyGHz();
    double cycleNS = 1 / freqGHz;

    printf("freq = %fGHz, cycle_time = %fns\n",freqGHz,cycleNS);

    fillMatrices();

    int *expected = answer(*ma, *mb);
 
    printf("matrix multiplication (per calculation) ...\n");

    run("Naive", naive, cycleNS, expected);
    run("Blocking", multiply, cycleNS, expected);
    run("Multi-threading", pthreadMultiply, cycleNS, expected);
    run("Multi-threading with blocking", pthreadBlockMultiply, cycleNS, expected);
    //printAll(*ma, *mb, actual);
    //printf("Expected: \n");
    //printMatrix(expected);
    return 0;
}
