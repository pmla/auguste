#ifndef LUP_H
#define LUP_H

bool lup_decompose(int n, double* A, int* P);
void lup_solve(int n, double* A, int* P, double* b, double* x);

#endif

