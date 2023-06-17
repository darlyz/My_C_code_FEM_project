/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/

#ifndef _SHAP_FUNC_H_
#define _SHAP_FUNC_H_

#include "fem.h"

double lagrange_shapfunc_t3(const double *shap_coor, int node_i); // Triangle

double lagrange_deriva_shapfunc_t3(const double *shap_coor, int node_i, int coor_i); // Triangle

double lagrange_shapfunc_q4(const double *shap_coor, int node_i); // Quadrilateral

double lagrange_deriva_shapfunc_q4(const double *shap_coor, int node_i, int coor_i); // Quadrilateral

double lagrange_shapfunc_w4(const double *shap_coor, int node_i); // Tetrahedron

double lagrange_deriva_shapfunc_w4(const double *shap_coor, int node_i, int coor_i); // Tetrahedron

double lagrange_shapfunc_c8(const double *shap_coor, int node_i); // Hexahedron

double lagrange_deriva_shapfunc_c8(const double *shap_coor, int node_i, int coor_i); // Hexahedron

typedef double (*Type_Lagrange_Shapfunc_Ptr)(const double*, int);

typedef double (*Type_Lagrange_Deriva_Shapfunc_Ptr)(const double*, int, int);

typedef struct Shap_Func {
    Type_Lagrange_Shapfunc_Ptr lagrange_shapfunc_ptr;
    Type_Lagrange_Deriva_Shapfunc_Ptr lagrange_deriva_shapfunc_ptr;
} Shap_Func;

Shap_Func get_shap_func(Mesh_Type MType);

#endif