/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/

#ifndef _CALC_SHAP_H_
#define _CALC_SHAP_H_

#include "shap_func.h"

// return *real_coor, *jacb_matr and |jacb_matr|
double transe_coor(
    double* real_coor,
    const double* refr_coor,
    const double* node_coor,
    double* jacb_matr,
    int dim,
    int shap_nodn,
    Shap_Func shap_func
);

// return **refr_shap
void set_refr_shap(
    double** refr_shap,
    const double* gaus_coor,
    int gaus_num,
    int node_cont,
    int dim,
    Shap_Func shap_func
);

// return *real_shap
void calc_real_shap(
    int dim,
    int node_cont,
    const double* refr_shap,
    double* real_shap,
    const double* invt_jacb
);

// return **refr_coup
void set_refr_coup(
    double** refr_coup,
    const double* gaus_coor,
    const double* coup,
    int gaus_num,
    int node_cont,
    int dim,
    Shap_Coup shap_coup
);

// return *real_coup
void calc_real_coup(
    int dim,
    int node_cont,
    const double* refr_coup,
    double* real_coup,
    const double* invt_jacb
);
#endif