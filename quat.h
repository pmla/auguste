#ifndef QUAT_H
#define QUAT_H

double quat_dot(double* a, double* b);
double quat_size(double* q);
void normalize_quaternion(double* q);
void quaternion_to_rotation_matrix(double* q, double* U);
void rotation_matrix_to_quaternion(double* u, double* q);

#endif


