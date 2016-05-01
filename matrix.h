#ifndef _MATRIX_H_
#define _MATRIX_H_

#define N 2000

//extern int* ma;
//extern int* mb;
extern int ma[N][N];
extern int mb[N][N];

extern int* multiply();
extern int* threadMultiply();
extern int* pthreadMultiply();
extern int* pthreadBlockMultiply();
extern int* threadFastMultiply(void);
extern int* naive(void);
extern void printMatrix(int*);

#endif
