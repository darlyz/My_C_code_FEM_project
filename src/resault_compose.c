/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "fem.h"
#include "gaussain.h"
#include "resault_compose.h"
#include "matrix_compose.h"
#include "calc_shap.h"
#include "tools.h"

void response_result( Equat_Set Equa, Field_info* field_ptr, int total_nodes ) {

    double *result  = field_ptr->Res.result;
    int     dof_num = field_ptr->Res.dofN;

    for (int i=0; i<total_nodes; i++) {

        for (int j=0; j<dof_num; j++) {

            int id_SN = Equa.dof_idx[i][j];

            if (id_SN > 0)
                result[i*dof_num + j] = Equa.vector[id_SN - 1];
        }
    }

    printf("result composed done!\n");
}

void Init_derivate_result(Derivative_Resault* deriv_res_ptr, int dofN, int nodeN) {
    deriv_res_ptr->dofN = dofN;
    deriv_res_ptr->nodeN = nodeN;
    deriv_res_ptr->result = (double*)calloc(nodeN*dofN, sizeof(double));
    deriv_res_ptr->accum  = (int*   )calloc(nodeN, sizeof(int));
}

static void deriv_elem_calc(int elem_i, Gaus_Info G_info, Elem_Info E_info,
                            Derivative_Resault* deriv_res_ptr, Shap_Func shap_func,
                            Shap_Coup shap_coup, Deriv_Coup deriv_doup);

static void set_derivcoup(Deriv_Coup* deriv_coup_ptr, int nodeN, int g_dim){
    deriv_coup_ptr->u  = (double*)malloc(nodeN*sizeof(double));
    deriv_coup_ptr->ux = (double*)malloc(nodeN*sizeof(double));
    deriv_coup_ptr->uy = (double*)malloc(nodeN*sizeof(double));
    deriv_coup_ptr->uz = (double*)malloc(nodeN*sizeof(double));
    deriv_coup_ptr->real_coup = (double*)malloc(nodeN*(g_dim+1)*sizeof(double));
}

void derivate_result(Field_info* field_ptr, Coor_Info Coor, Node_Mesh Mesh) {

    Init_derivate_result(&(field_ptr->Deriv_Res), Coor.dim, Coor.nodeN);

    Materail  *Mate  = &(field_ptr->Mate);
    Mesh_Mate *Emate = &(field_ptr->Emate);

    for (int type_i = 1; type_i <= Mesh.typeN; type_i ++)
    {
        int elem_nodeN = Mesh.nodeN[type_i-1];
        int mesh_scale = Mesh.scale[type_i-1];

        Gaus_Info G_info;
        set_gaus(&G_info, Mesh.type[type_i-1], 2);

        Elem_Info E_info;
        set_elem(&E_info, Coor.dim, elem_nodeN, Mate->varN, G_info.gausN, &(field_ptr->Res));

        Shap_Func shap_func = get_shap_func(Mesh.type[type_i-1]);
        Shap_Coup shap_coup = get_shap_coef(Mesh.type[type_i-1]);

        Deriv_Coup deriv_coup;
        set_derivcoup(&deriv_coup, E_info.nodeN, E_info.g_dim);

        for (int elem_i=1; elem_i<=mesh_scale; elem_i++)
        {

            memcpy(E_info.topo,
                  &Mesh.topo[type_i-1][(elem_i-1)*elem_nodeN],
                   elem_nodeN*sizeof(int));

            E_info.elemID = Emate->elemID[type_i-1][elem_i-1];

            if (E_info.elemID <= 0)
                continue;

            memcpy(E_info.mate,
                 &(Mate->mate[(E_info.elemID-1)*Mate->varN]),
                   Mate->varN*sizeof(double));

            for (int node_i=1; node_i<=elem_nodeN; node_i++) {

                for (int dim_i=1; dim_i<=E_info.g_dim; dim_i++)
                    E_info.coor[(dim_i-1)*elem_nodeN + node_i-1] = 
                    Coor.coor[(E_info.topo[node_i-1]-1)*E_info.g_dim + dim_i-1];

                for (int dof_i=1; dof_i<=E_info.coup_dofN; dof_i++)
                    E_info.coup[(dof_i-1)*elem_nodeN + node_i-1] =
                    field_ptr->Res.result[(E_info.topo[node_i-1]-1)*E_info.coup_dofN + dof_i-1];
            }

            deriv_elem_calc(elem_i, G_info, E_info, &(field_ptr->Deriv_Res), shap_func, shap_coup, deriv_coup);
        }
    }

    for (int node_i = 1; node_i <= field_ptr->Deriv_Res.nodeN; node_i ++){
        for (int dof_i = 1; dof_i <= field_ptr->Deriv_Res.dofN; dof_i ++)
            field_ptr->Deriv_Res.result[(node_i-1)*field_ptr->Deriv_Res.dofN + dof_i-1] /= field_ptr->Deriv_Res.accum[node_i-1];
    }
}

static double idmatrix[] = {1., 0., 0., 0., 1., 0., 0., 0., 1.};

void deriv_elem_calc(
    int elem_i,
    Gaus_Info  G_info,
    Elem_Info  E_info,
    Derivative_Resault* deriv_res_ptr,
    Shap_Func shap_func,
    Shap_Coup shap_coup,
    Deriv_Coup deriv_doup
){
    static double x,y,z;
    static double rx,ry,rz;
    static double tempvar;

    double * u = deriv_doup.u ;
    double *ux = deriv_doup.ux;
    double *uy = deriv_doup.uy;
    double *real_coup = deriv_doup.real_coup;

    double *coef_u = (double*)malloc(E_info.nodeN*sizeof(double));
    memcpy(coef_u, &(E_info.coup[(1-1)*E_info.nodeN]), E_info.nodeN*sizeof(double));
    //double *coef_v = &(E_info.coup[(2-1)*E_info.nodeN]);

    static double eps,rho;
    eps = E_info.mate[0];
    rho = E_info.mate[1];

    static double real_coor[3];
    static double refr_coor[3];
    static double jacb_matr[9];
    static double invt_jacb[9];

    set_refr_coup(E_info.refr_coup, G_info.gcoor, coef_u, G_info.gausN, E_info.nodeN, E_info.g_dim, shap_coup);

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
        calc_real_coup(E_info.g_dim, E_info.nodeN, E_info.refr_coup[gaus_i-1], real_coup, invt_jacb);

        //for (int i=1; i<=E_info.nodeN; ++i)
        //    u[i-1]  = +real_coup[(i-1)*(E_info.g_dim+1)+1-1];

        for (int i=1; i<=E_info.nodeN; ++i)
            //ux[i-1] = +real_coup[(i-1)*(E_info.g_dim+1)+2-1];
            deriv_res_ptr->result[(E_info.topo[i-1]-1)*deriv_res_ptr->dofN + 0] += real_coup[(i-1)*(E_info.g_dim+1)+2-1] ;

        for (int i=1; i<=E_info.nodeN; ++i)
            //uy[i-1] = +real_coup[(i-1)*(E_info.g_dim+1)+3-1];
            deriv_res_ptr->result[(E_info.topo[i-1]-1)*deriv_res_ptr->dofN + 1] += real_coup[(i-1)*(E_info.g_dim+1)+3-1] ;
    }

    for (int i=1; i<=E_info.nodeN; ++i)
        deriv_res_ptr->accum[E_info.topo[i-1]-1] += 1;
 
}