/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "matrix_compose.h"
#include "elemcalc.h"
#include "shap_func.h"
#include "calc_shap.h"
#include "gaussain.h"
#include "tools.h"
#include "check_and_show.h"

static void set_matr  (Elem_Matr* E_matr_ptr, int elem_dof, const Matr_Type *M_types);
static void reset_matr(Elem_Matr* E_matr_ptr, int elem_dof, const Matr_Type *M_types);
static void clear_matr(Elem_Matr* E_matr_ptr);

void matrix_compose(
    Coor_Info  Coor,
    Node_Mesh  Mesh,
    Field_info *field_ptr,
    Equat_Set  *Equa_ptr,
    int Field_i
){

    int node_dof = field_ptr->Res.dofN;
    Materail  *Mate_ptr  = &(field_ptr->Mate);
    Mesh_Mate *Emate_ptr = &(field_ptr->Emate);
    double    *result    =   field_ptr->Res.result;

    for (int type_i=1; type_i<=Mesh.typeN; type_i++)
    {
        int elem_nodeN = Mesh.nodeN[type_i-1];
        int mesh_scale = Mesh.scale[type_i-1];
        int ematr_size = node_dof*elem_nodeN;

        // to set
        Matr_Type M_type[4]={dist,lump,lump,lump};

        Gaus_Info G_info;
        set_gaus(&G_info, Mesh.type[type_i-1], 2);

        Elem_Info E_info;
        set_elem(&E_info, Coor.dim, elem_nodeN, Mate_ptr->varN, G_info.gausN, NULL);

        Elem_Matr E_matr;
        set_matr(&E_matr, ematr_size, M_type);

        Shap_Func shap_func = get_shap_func(Mesh.type[type_i-1]);

        Test_Func test_func;
        set_testfunc(&test_func, E_info.nodeN, E_info.g_dim);

        set_refr_shap(E_info.refr, G_info.gcoor, G_info.gausN, E_info.nodeN, E_info.g_dim, shap_func);

        for (int elem_i=1; elem_i<=mesh_scale; elem_i++)
        {

            memcpy(E_info.topo,
                  &Mesh.topo[type_i-1][(elem_i-1)*elem_nodeN],
                   elem_nodeN*sizeof(int));

            E_info.elemID = Emate_ptr->elemID[type_i-1][elem_i-1];

            if (E_info.elemID <= 0)
                continue;

            memcpy(E_info.mate,
                 &(Mate_ptr->mate[(E_info.elemID-1)*Mate_ptr->varN]),
                   Mate_ptr->varN*sizeof(double));

            for (int node_i=1; node_i<=elem_nodeN; node_i++) {
                for (int dim_i=1; dim_i<=E_info.g_dim; dim_i++)

                    E_info.coor[(dim_i-1)*elem_nodeN + node_i-1] = 
                    Coor.coor[(E_info.topo[node_i-1]-1)*E_info.g_dim + dim_i-1];
            }

            reset_matr(&E_matr, ematr_size, M_type);

            elemcalc(elem_i, G_info, E_info, &E_matr, shap_func, test_func);

            //show_elem(E_info, elem_nodeN, Mate_ptr->varN, G_info.gausN);
            //show_elem_stif(E_info.nodeN, E_matr);
            //show_elem_matr(E_matr, ematr_size, M_type);

            // construct element left matrix
            for (int dof_i=0; dof_i<ematr_size; dof_i++) {

                for (int dof_j=0; dof_j<ematr_size; dof_j++) {
                    // distributed matrix
                    E_matr.left_matr[dof_i*ematr_size + dof_j] += E_matr.matr_0[dof_i*ematr_size + dof_j];
                }
                // lumped matrix
                //E_matr.left_matr[dof_i*ematr_size + dof_i] += E_matr.matr_1[dof_i]*dt/2
                //                                           +  E_matr.matr_2[dof_i];
            }

            // construct total left matrix and right vector
            for (int nod_i=0; nod_i<elem_nodeN; nod_i++) {

                int nodi_SN = E_info.topo[nod_i];

                for (int dof_i=0; dof_i<node_dof; dof_i++) {

                    int ID_i = Equa_ptr->dof_idx[nodi_SN-1][dof_i];

                    int MD_i = nod_i*node_dof + dof_i;

                    if (ID_i == 0)
                        continue;

                    if (ID_i  > 0) {

                        // lumped matrix for right vector
                        Equa_ptr->vector[ID_i-1] += E_matr.righ_vect[MD_i];
                        //                   +  E_matr.righ_vect[MD_i] * dt*dt/2
                        //                   +  E_matr.matr_2   [MD_i] * u_n[(nodi_SN-1)*node_dof + dof_i]
                        //                   +  E_matr.matr_2   [MD_i] * v_n[(nodi_SN-1)*node_dof + dof_i] * dt
                        //                   +  E_matr.matr_1   [MD_i] * u_n[(nodi_SN-1)*node_dof + dof_i] * dt/2;
                    }

                    int idx = 0;

                    for (int nod_j=0; nod_j<elem_nodeN; nod_j++) {

                        int nodj_SN = E_info.topo[nod_j];

                        for (int dof_j=0; dof_j<node_dof; dof_j++) {

                            int ID_j = Equa_ptr->dof_idx[nodj_SN-1][dof_j];

                            int MD_j = nod_j*node_dof + dof_j;

                            if (ID_j == 0)
                                continue;

                            if (ID_j  > 0) {

                                // distributed matrix for right vector
                                //Equa_ptr->vector[ID_j-1] += -E_matr.matr_0[MD_i*ematr_size + MD_j] * u_n[(nodi_SN-1)*node_dof + dof_i] *dt*dt/4
                                //                     +   E_matr.matr_0[MD_i*ematr_size + MD_j] * v_n[(nodi_SN-1)*node_dof + dof_i] *dt*dt/2;

                                // deal with direclet boundary conditions
                                if (ID_i < 0)
                                    Equa_ptr->vector[ID_j-1] -= E_matr.left_matr[MD_i*ematr_size + MD_j] * result[(nodi_SN-1)*node_dof+dof_i];

                                // compose whole matrix
                                else if (ID_i > 0) {

                                    idx = Binary_Search_(Equa_ptr->clm_idx[ID_i-1], Equa_ptr->row_nZN[ID_i-1], ID_j);

                                    if (idx < 0)
                                        continue;

                                    Equa_ptr->matrix[ID_i-1][idx] += E_matr.left_matr[MD_i*ematr_size + MD_j];
                                    //idx ++;
                                }
                            }
                        }
                    }
                }
            }
        }

        //clear_matr(&E_matr);               // ! mem fault if not comm it
        //clear_elem(&E_info, G_info.gausN); // ! mem fault if not comm it
        //clear_testfunc(&test_func);
    }

    //show_matr(*Equa_ptr);
    printf("Compose whole matrix done!\n");
}

void set_matr  (Elem_Matr* E_matr_ptr, int elem_dof, const Matr_Type *M_types)
{
    static int size[4];
    for (int i=0; i<4; i++) {
        if     (M_types[i] == lump) size[i] = elem_dof;
        else if(M_types[i] == dist) size[i] = elem_dof*elem_dof;
    }
    E_matr_ptr->matr_0    = (double*)malloc(size[0]          *sizeof(double));
    E_matr_ptr->matr_1    = (double*)malloc(size[1]          *sizeof(double));
    E_matr_ptr->matr_2    = (double*)malloc(size[2]          *sizeof(double));
    E_matr_ptr->matr_3    = (double*)malloc(size[3]          *sizeof(double));
    E_matr_ptr->left_matr = (double*)malloc(elem_dof*elem_dof*sizeof(double));
    E_matr_ptr->righ_vect = (double*)malloc(elem_dof         *sizeof(double));
}

void reset_matr(Elem_Matr* E_matr_ptr, int elem_dof, const Matr_Type *M_types)
{
    static int size[4];
    for (int i=0; i<4; i++){
        if     (M_types[i] == lump) size[i] = elem_dof;
        else if(M_types[i] == dist) size[i] = elem_dof*elem_dof;
    }
    memset(E_matr_ptr->matr_0    , 0, size[0]          *sizeof(double));
    memset(E_matr_ptr->matr_1    , 0, size[1]          *sizeof(double));
    memset(E_matr_ptr->matr_2    , 0, size[2]          *sizeof(double));
    memset(E_matr_ptr->matr_3    , 0, size[3]          *sizeof(double));
    memset(E_matr_ptr->left_matr , 0, elem_dof*elem_dof*sizeof(double));
    memset(E_matr_ptr->righ_vect , 0, elem_dof         *sizeof(double));
}

void clear_matr(Elem_Matr* E_matr_ptr)
{
    free(E_matr_ptr->matr_0);    E_matr_ptr->matr_0 = NULL;
    free(E_matr_ptr->matr_1);    E_matr_ptr->matr_1 = NULL;
    free(E_matr_ptr->matr_2);    E_matr_ptr->matr_2 = NULL;
    free(E_matr_ptr->matr_3);    E_matr_ptr->matr_3 = NULL;
    free(E_matr_ptr->left_matr); E_matr_ptr->left_matr = NULL;
    free(E_matr_ptr->righ_vect); E_matr_ptr->righ_vect = NULL;
}

void set_elem(Elem_Info *E_info_ptr, int dim, int elem_nodeN, int varN, int gausN, const Result* res_ptr)
{
    E_info_ptr->g_dim = dim;
    E_info_ptr->nodeN = elem_nodeN;

    E_info_ptr->topo = (int*    )malloc(E_info_ptr->nodeN * sizeof(int));
    E_info_ptr->coor = (double* )malloc(E_info_ptr->nodeN * E_info_ptr->g_dim * sizeof(double));
    E_info_ptr->mate = (double* )malloc(varN  * sizeof(double));
    E_info_ptr->refr = (double**)malloc(gausN * sizeof(double*));
    for (int gaus_i=1; gaus_i<=gausN; gaus_i++)
        E_info_ptr->refr[gaus_i-1] = (double*)malloc(E_info_ptr->nodeN * (E_info_ptr->g_dim+1) * sizeof(double));
    
    if (res_ptr != NULL){
        E_info_ptr->coup_dofN = res_ptr->dofN;
        E_info_ptr->coup = (double*)malloc(E_info_ptr->nodeN * E_info_ptr->coup_dofN * sizeof(double));
        E_info_ptr->refr_coup = (double**)malloc(gausN * sizeof(double*));
        for (int gaus_i=1; gaus_i<=gausN; gaus_i++)
            E_info_ptr->refr_coup[gaus_i-1] = (double*)malloc(E_info_ptr->nodeN * (E_info_ptr->g_dim+1) * sizeof(double));
    }
}

void clear_elem(Elem_Info *E_info_ptr, int gausN)
{
    free(E_info_ptr->topo); E_info_ptr->topo = NULL;
    free(E_info_ptr->coor); E_info_ptr->coor = NULL;
    //free(E_info_ptr->coup);
    free(E_info_ptr->mate); E_info_ptr->mate = NULL;
    for (int gaus_i=1; gaus_i<=gausN; gaus_i++)
        free(E_info_ptr->refr[gaus_i-1]);
    free(E_info_ptr->refr); E_info_ptr->refr = NULL;
}

void set_testfunc(Test_Func *test_func_ptr, int nodeN, int g_dim){
    test_func_ptr->u  = (double*)malloc(nodeN*sizeof(double));
    test_func_ptr->ux = (double*)malloc(nodeN*sizeof(double));
    test_func_ptr->uy = (double*)malloc(nodeN*sizeof(double));
    test_func_ptr->uz = (double*)malloc(nodeN*sizeof(double));
    test_func_ptr->real_shap = (double*)malloc(nodeN*(g_dim+1)*sizeof(double));
}

void clear_testfunc(Test_Func *test_func_ptr){
    free(test_func_ptr->u );        test_func_ptr->u  = NULL;
    free(test_func_ptr->ux);        test_func_ptr->ux = NULL;
    free(test_func_ptr->uy);        test_func_ptr->uy = NULL;
    free(test_func_ptr->uz);        test_func_ptr->uz = NULL;
    free(test_func_ptr->real_shap); test_func_ptr->real_shap = NULL;
}
