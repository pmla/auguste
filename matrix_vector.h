#ifndef MATRIX_VECTOR_H
#define MATRIX_VECTOR_H

#include <cstdint>


void swap(double* a, double* b);
double vector_dot(int n, double* a, double* b);
double vector_norm(int n, double* x);
void normalize_vector(int n, double* x);
void matvec(int n, double* A, double* x, double* b);
void matveci(int n, double* A, int* x, double* b);
void transpose(int n, double* A);
void transposei(int n, int* A);
double frobenius_inner_product(double* A, double* B);
void matmul(int n, double* A, double* x, double* b);
void matmuli(int n, int* A, int* x, int* b);
void matmul_int8(int n, int* A, int8_t* x, int* b);
void matmul_di(int n, double* A, int* x, double* b);
void matmul_id(int n, int* A, double* x, double* b);
double determinant_3x3(double* m);
void unimodular_inverse_3x3i(int* A, int* B);
void flip_matrix(int n, double* m);

#endif

