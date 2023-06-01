/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/

#ifndef _CALC_SHAP_H_
#define _CALC_SHAP_H_

#include "shap_func.h"

// return *jacb_matr and |jacb_matr|
double calc_jacobi(
	double* node_coor,
	double* refr_coor,
	double* jacb_matr,
	int dim,
	int shap_nodn,
	Type_Lagrange_Deriva_Shapfunc_Ptr lagrange_deriva_shapfunc_ptr
);

// return *real_coor, *jacb_matr and |jacb_matr|
double transe_coor(
	double* real_coor,
	double* refr_coor,
	double* node_coor,
	double* jacb_matr,
	int dim,
	int shap_nodn,
	Shap_Func shap_func
);

// return *refr_shap
void calc_refr_shap(
	double* refr_shap,
	double* refr_coor,
	int node_cont,
	int dim,
	Shap_Func shap_func
);

// return **refr_shap
void set_refr_shap(
	double** refr_shap,
	double* gaus_coor,
	int gaus_num,
	int node_cont,
	int dim,
	Shap_Func shap_func
);

// return *real_shap
void calc_real_shap(int dim, int node_cont, double* refr_shap, double* real_shap, double* invt_jacb);

#endif