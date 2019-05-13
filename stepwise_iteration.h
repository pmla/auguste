#ifndef STEPWISE_ITERATION_H
#define STEPWISE_ITERATION_H

double calculate_trace(int n, double* x, double* Ktrans, double* Q, double* P);
double stepwise_iteration(int n, double* x, double* Ktrans, double* Q, double* P);
double optimize_stepwise(int n, double* x, double* Ktrans, double* Q, double* P, int max_it, double tolerance);

#endif


