/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "fem.h"
#include "calc_shap.h"
#include "tools.h"
#include "matrix_compose.h"
#include "check_and_show.h"

void elemcalc (
    int elem_n,
    Gaus_Info  G_info,
    Elem_Info  E_info,
    Elem_Matr *E_matr,
    Shap_Func  shap_func,
    Test_Func  test_func
){
    static double x,y,z;
    static double rx,ry,rz;
    static double tempvar;

    double * u = test_func.u ;
    double *ux = test_func.ux;
    double *uy = test_func.uy;
    double *real_shap = test_func.real_shap;

    static double eps,rho;
    eps = E_info.mate[0];
    rho = E_info.mate[1];

    static double real_coor[3];
    static double refr_coor[3];
    static double jacb_matr[9];
    static double invt_jacb[9];

    for (int gaus_i = 1; gaus_i <= G_info.gausN; gaus_i ++)
    {
        for (int i=1;i<=E_info.g_dim;i++)
        {
            refr_coor[i-1] = G_info.gcoor[(i-1)*G_info.gausN + gaus_i-1];
            real_coor[i-1] = 0.;
            for (int j=1;j<=E_info.g_dim;j++)
                jacb_matr[(i-1)*E_info.g_dim+j-1]=0.;
        }

        double det = transe_coor(real_coor, refr_coor, E_info.coor, jacb_matr, E_info.g_dim, E_info.nodeN, shap_func);

        double invt_det = inv(invt_jacb, jacb_matr, E_info.g_dim);

        x  = real_coor[0];
        y  = real_coor[1];

        rx = refr_coor[0];
        ry = refr_coor[1];

        double weight = det*G_info.gweig[gaus_i-1];
        calc_real_shap(E_info.g_dim, E_info.nodeN, E_info.refr[gaus_i-1], real_shap, invt_jacb);

        for (int i=1; i<=E_info.nodeN; ++i)
            u[i-1]  = +real_shap[(i-1)*(E_info.g_dim+1)+1-1];

        for (int i=1; i<=E_info.nodeN; ++i)
            ux[i-1] = +real_shap[(i-1)*(E_info.g_dim+1)+2-1];

        for (int i=1; i<=E_info.nodeN; ++i)
            uy[i-1] = +real_shap[(i-1)*(E_info.g_dim+1)+3-1];

        // stiffness calculation
        for (int i=1; i<=E_info.nodeN; ++i)
            for (int j=1; j<=E_info.nodeN; ++j)
            {
                tempvar = ux[i-1]*ux[j-1]*eps
                        + uy[i-1]*uy[j-1]*eps;
                E_matr->matr_0[(i-1)*E_info.nodeN + j-1] += tempvar*weight;
            }

        //for (int i=1; i<=E_info.nodeN; ++i)
        //    for (int j=1; j<=E_info.nodeN; ++j)
        //    {
        //        tempvar = u[i-1]*u[j-1];
        //        E_matr->matr_0[(i-1)*E_info.nodeN + j-1] += tempvar*weight;
        //    }

        // loadvector calculation
        for (int i = 1; i <= E_info.nodeN; ++ i)
        {
            tempvar = u[i-1]*rho;
            E_matr->righ_vect[i-1] += tempvar*weight;
        }
    }

    //show_elem_stif(E_info.nodeN, *E_matr);

}