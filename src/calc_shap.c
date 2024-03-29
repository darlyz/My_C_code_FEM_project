/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "fem.h"
#include "shap_func.h"

double det(double*,int);

// return *jacb_matr and |jacb_matr|
static double calc_jacobi(
    const double* node_coor,
    const double* refr_coor,
    double* jacb_matr,
    int dim,
    int shap_nodn,
    Type_Lagrange_Deriva_Shapfunc_Ptr lagrange_deriva_shapfunc_ptr
){
    for (int i=1; i<=dim; ++i)
        for (int j=1; j<=dim; ++j)
            for (int node_i=1; node_i<=shap_nodn; ++node_i)
                jacb_matr[(j-1)*dim+i-1] += node_coor[(j-1)*shap_nodn + node_i-1] * (*lagrange_deriva_shapfunc_ptr)(refr_coor, node_i, i);
    return det(jacb_matr, dim);
}

// return *real_coor, *jacb_matr and |jacb_matr|
double transe_coor(
    double* real_coor,
    const double* refr_coor,
    const double* node_coor,
    double* jacb_matr,
    int dim,
    int shap_nodn,
    Shap_Func shap_func
){
    for (int i=1; i<=dim; i++)
        for (int node_i=1; node_i<=shap_nodn; node_i++)
            real_coor[i-1] += node_coor[(i-1)*shap_nodn + node_i-1] * (*shap_func.lagrange_shapfunc_ptr)(refr_coor, node_i);
    return calc_jacobi(node_coor, refr_coor, jacb_matr, dim, shap_nodn, shap_func.lagrange_deriva_shapfunc_ptr);
}

// return *refr_shap
static void calc_refr_shap(
    double* refr_shap,
    const double* refr_coor,
    int node_cont,
    int dim,
    Shap_Func shap_func
){
    for (int node_i=1; node_i<=node_cont; node_i++)
    {
        refr_shap[(node_i-1)*(dim+1)] = (*shap_func.lagrange_shapfunc_ptr)(refr_coor, node_i);
        for (int dim_i=1; dim_i<=(dim+1); dim_i++)
            refr_shap[(node_i-1)*(dim+1) + dim_i] = (*shap_func.lagrange_deriva_shapfunc_ptr)(refr_coor, node_i, dim_i);
    }
}

// return **refr_shap
void set_refr_shap(
    double** refr_shap,
    const double* gaus_coor,
    int gaus_num,
    int node_cont,
    int dim,
    Shap_Func shap_func
){
    double refr_coor[]={0.0, 0.0, 0.0};
    for (int gaus_i = 1; gaus_i <= gaus_num; gaus_i ++)
    {
        for (int i = 1; i <= dim; i ++)
            refr_coor[i-1] = gaus_coor[(i-1)*gaus_num + gaus_i-1];
        calc_refr_shap( refr_shap[gaus_i-1], refr_coor, node_cont, dim, shap_func);
    }
}

// return *real_shap
void calc_real_shap(
    int dim,
    int node_cont,
    const double* refr_shap,
    double* real_shap,
    const double* invt_jacb
){
    for (int node_i=1; node_i<=node_cont; node_i++)
    {
        real_shap[(node_i-1)*(dim+1)] = refr_shap[(node_i-1)*(dim+1)];
        for (int dim_i=1; dim_i<=dim; dim_i++)
        {
            double temp = 0.0;
            for (int k=1; k<=dim; ++k)
                temp += invt_jacb[(k-1)*dim + dim_i-1] * refr_shap[(node_i-1)*(dim+1) + k];
            real_shap[(node_i-1)*(dim+1) + dim_i] = temp;
        }
    }
}

// return *refr_coup
static void calc_refr_coup(
    double* refr_coup,
    const double* refr_coor,
    const double* coup,
    int node_cont,
    int dim,
    Shap_Coup shap_coup
){
    for (int node_i=1; node_i<=node_cont; node_i++)
    {
        refr_coup[(node_i-1)*(dim+1)] = (*shap_coup.lagrange_shapcoup_ptr)(refr_coor, coup, node_i);
        for (int dim_i=1; dim_i<=(dim+1); dim_i++)
            refr_coup[(node_i-1)*(dim+1) + dim_i] = (*shap_coup.lagrange_deriva_shapcoup_ptr)(refr_coor, coup, node_i, dim_i);
    }
}

// return **refr_coup
void set_refr_coup(
    double** refr_coup,
    const double* gaus_coor,
    const double* coup,
    int gaus_num,
    int node_cont,
    int dim,
    Shap_Coup shap_coup
){
    double refr_coor[]={0.0, 0.0, 0.0};
    for (int gaus_i = 1; gaus_i <= gaus_num; gaus_i ++)
    {
        for (int i = 1; i <= dim; i ++)
            refr_coor[i-1] = gaus_coor[(i-1)*gaus_num + gaus_i-1];
        calc_refr_coup( refr_coup[gaus_i-1], refr_coor, coup, node_cont, dim, shap_coup);
    }
}

// return *real_coup
void calc_real_coup(
    int dim,
    int node_cont,
    const double* refr_coup,
    double* real_coup,
    const double* invt_jacb
){
    for (int node_i=1; node_i<=node_cont; node_i++)
    {
        real_coup[(node_i-1)*(dim+1)] = refr_coup[(node_i-1)*(dim+1)];
        for (int dim_i=1; dim_i<=dim; dim_i++)
        {
            double temp = 0.0;
            for (int k=1; k<=dim; ++k)
                temp += invt_jacb[(k-1)*dim + dim_i-1] * refr_coup[(node_i-1)*(dim+1) + k];
            real_coup[(node_i-1)*(dim+1) + dim_i] = temp;
        }
    }
}