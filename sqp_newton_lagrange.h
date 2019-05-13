#ifndef SQP_NEWTON_LAGRANGE_H
#define SQP_NEWTON_LAGRANGE_H

double newton_lagrange_step(int n, double* args, double (*key)[4][4], double* step);

#endif

