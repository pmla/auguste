/*MIT License

Copyright (c) 2019 P. M. Larsen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/


#ifndef MATRIX_VECTOR_H
#define MATRIX_VECTOR_H

#include <stdint.h>


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
void flip_matrix_i(int n, int* m);

#endif

