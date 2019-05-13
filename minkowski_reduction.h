#ifndef MINKOWSKI_REDUCTION_H
#define MINKOWSKI_REDUCTION_H

#ifdef __cplusplus
extern "C" {
#endif

int minkowski_basis(double (*B)[3], double (*reduced_basis)[3], int (*path)[3]);

#ifdef __cplusplus
}
#endif


#endif

