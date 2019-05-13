#ifndef SYMMETRIZATION_H
#define SYMMETRIZATION_H

#ifdef __cplusplus
extern "C" {
#endif

int optimize(	char* name,
		double* input_B,	//lattice basis in column-vector format
		bool search_correspondences,
		double* symmetrized,
		double* p_strain);

#ifdef __cplusplus
}
#endif

#endif

