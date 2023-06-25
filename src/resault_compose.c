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
                            Derivative_Resault* deriv_res, Shap_Func shap_func,
                            Test_Func test_func);

void derivate_result(Field_info* field_ptr, Coor_Info Coor, Node_Mesh Mesh) {

    Init_derivate_result(&(field_ptr->Deriv_Res), Coor.dim, Coor.nodeN);

    Materail  *Mate  = &(field_ptr->Mate);
    Mesh_Mate *Emate = &(field_ptr->Emate);

    for (int type_i = 1; type_i <= Mesh.typeN; type_i ++)
    {
        int elem_nodeN = Mesh.nodeN[type_i-1];
        int mesh_scale = Mesh.scale[type_i-1];

        Gaus_Info G_info;
        set_gaus(&G_info, Mesh.type[type_i-1], 0);

        Elem_Info E_info;
        set_elem(&E_info, Coor.dim, elem_nodeN, Mate->varN, G_info.gausN, &(field_ptr->Res));

        Shap_Func shap_func = get_shap_func(Mesh.type[type_i-1]);

        Test_Func test_func;
        set_testfunc(&test_func, E_info.nodeN, E_info.g_dim);

        set_refr_shap(E_info.refr, G_info.gcoor, G_info.gausN, E_info.nodeN, E_info.g_dim, shap_func);

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

            deriv_elem_calc(elem_i, G_info, E_info, &(field_ptr->Deriv_Res), shap_func, test_func);
        }
    }

    for (int node_i = 1; node_i <= field_ptr->Deriv_Res.nodeN; node_i ++){
        for (int dof_i = 1; dof_i <= field_ptr->Deriv_Res.dofN; dof_i ++)
            field_ptr->Deriv_Res.result[(node_i-1)*field_ptr->Deriv_Res.dofN + dof_i-1] /= field_ptr->Deriv_Res.accum[node_i-1];
    }
}

void deriv_elem_calc(
    int elem_i,
    Gaus_Info  G_info,
    Elem_Info  E_info,
    Derivative_Resault* deriv_res,
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

    for (int node_i = 1; node_i < E_info.nodeN; node_i ++){
        
    }

}