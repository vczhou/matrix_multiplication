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

int* answer(int a[N][N], int b[N][N]){
    static int c[N][N];
    int sum = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            sum = 0;
            for(int k = 0; k < N; k++){
                sum += a[i][k] * b[k][j];
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

int main(int argc, char* argv[]) {
    double freqGHz = getFrequencyGHz();
    double cycleNS = 1 / freqGHz;

    printf("freq = %fGHz, cycle_time = %fns\n",freqGHz,cycleNS);

    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            int x = rand() % 11;
            int y = rand() % 11;
            ma[i][j] = x;
            mb[i][j] = y;
        }
    }

    int *expected = answer(ma, mb);
 
    int *actual = 0;

    printf("matrix multiplication (per calculation) ...\n");

    signal(SIGALRM,handler);
    uint64_t count = 0;
    done = 0;

    alarm(T);

    while (!done) {
        actual = multiply();
    }

    report((((N * N) - N) / 2) * count, cycleNS);

    int equal = compare(actual, expected);
    if(equal)
        printf("Success!");
    else
        printf("Try again :)");

    return 0;
}
