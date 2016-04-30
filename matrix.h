#ifndef _MATRIX_H_
#define _MATRIX_H_

#define N 4

//extern int* ma;
//extern int* mb;
extern int ma[N][N];
extern int mb[N][N];

extern int* multiply();
extern int* threadMultiply();
extern int* threadFastMultiply(void);
extern int* naive(void);

#endif
