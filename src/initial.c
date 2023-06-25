/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "fem.h"
#include "tools.h"
#include "check_and_show.h"

#define init_adj_num 20
#define minsize_int32 -2147483648

void free_adj(int total_nodes, int* adj_nodn, int** adj_topo) {
    for (int i=0; i<total_nodes; i++)
        free(adj_topo[i]);
    free(adj_topo);
    free(adj_nodn);
}

void initial(
    Coor_Info   Coor,
    Node_Mesh   Mesh,
    Field_info *field_ptr,
    Equat_Set  *Equa_ptr
){
    int node_dof = field_ptr->Res.dofN;
    Dof_Tag   *ID_ptr    = &(field_ptr->ID);
    Elem_Tag  *E_ID_ptr  = &(field_ptr->E_ID);
    Mesh_Mate *Emate_ptr = &(field_ptr->Emate);

    // ---------------------------------------- initial solution space ----------------------------------------
    field_ptr->Res.result = (double *)calloc(Coor.nodeN*node_dof, sizeof(double));

    for (int i=0; i<ID_ptr->nodeN; i++) {

        int node_SN = ID_ptr->nodeSN[i] - 1;

        for (int j=0; j<ID_ptr->dofN; j++)

            if (ID_ptr->dofID[i*ID_ptr->dofN + j] == -1 )

                field_ptr->Res.result[node_SN*ID_ptr->dofN + j] = ID_ptr->dofval[i*ID_ptr->dofN + j];
    }

    // ---------------------------------------- initial material space ---------------------------------------
    Emate_ptr->typeN = Mesh.typeN;

    Emate_ptr->scale  = (int* )malloc(Emate_ptr->typeN*sizeof(int));

    memcpy(Emate_ptr->scale, Mesh.scale, Emate_ptr->typeN*sizeof(int));

    Emate_ptr->elemID = (int**)malloc(Emate_ptr->typeN*sizeof(int*));

    for (int i = 0; i < Emate_ptr->typeN; i++)
        Emate_ptr->elemID[i] = (int*)malloc(Emate_ptr->scale[i]*sizeof(int));

    for (int i = 0; i < Mesh.typeN; i++) {

        memset(Emate_ptr->elemID[i], 0, Emate_ptr->scale[i]*sizeof(int));

        for (int j = 0; j < E_ID_ptr->typeN; j++) {

            if (Mesh.type[i] == E_ID_ptr->type[j]) {

                for (int k = 0; k < E_ID_ptr->elemN[j]; k++)
                    Emate_ptr->elemID[i][E_ID_ptr->elemSN[j][k]-1] = E_ID_ptr->elemID[j][k];

                break;
            }
        }
    }
    //show_mesh_mate(*Emate_ptr);

    // ---------------------------------------- convert mesh to graph ----------------------------------------
    int   nodeN;
    int  *node;
    int  *adj_nodn;
    int **adj_topo;

    adj_nodn = (int* )calloc(Coor.nodeN,sizeof(int));
    adj_topo = (int**)malloc(Coor.nodeN*sizeof(int*));

    for (int i=0; i<Coor.nodeN; i++)
        adj_topo[i] = (int*)malloc(init_adj_num*sizeof(int));

    for (int type_i=0; type_i<Mesh.typeN; type_i++) {

        nodeN = Mesh.nodeN[type_i];

        for (int elem_i=0; elem_i<Mesh.scale[type_i]; elem_i++) {

            node = &(Mesh.topo[type_i][elem_i*nodeN]);

            for (int node_i=0; node_i<nodeN; node_i++) {

                for (int node_j=0; node_j<nodeN; node_j++) {

                    if (node_i == node_j)
                        continue;

                    insert_node(adj_topo + node[node_i] - 1,
                                adj_nodn + node[node_i] - 1,
                                init_adj_num,
                                node[node_j] );
                    //insert_node_(node[node_i], node[node_j], adj_nodn, adj_topo);
                }
            }
        }
    }

    //show_adj(adj_nodn, adj_topo, Coor.nodeN);

    // --------------------------------------- initial Equation Set ---------------------------------------
    //int constraint_count = 0;
    //for (int i=0; i<ID_ptr->tag_nodn; i++)
    //    for (int j=0; j<ID_ptr->dofN; j++)
    //        if (ID_ptr->dofID[i*ID_ptr->dofN + j] <= 0)
    //            constraint_count ++ ;

    //Equa_ptr->equaN = Coor.nodeN * node_dof - constraint_count;
    //printf("constraint_count = %d\n",constraint_count);

    int min_int32 = -2147483648;

    Equa_ptr->dof_idx = (int**)malloc(Coor.nodeN * sizeof(int*));
    for (int i=0; i<Coor.nodeN; i++) {
        Equa_ptr->dof_idx[i] = (int*)malloc(node_dof*sizeof(int));
        memcpy(Equa_ptr->dof_idx[i], &min_int32, node_dof*sizeof(int));
    }

    for (int i=0; i<ID_ptr->nodeN; i++) 
        for (int j=0; j<ID_ptr->dofN; j++)
            Equa_ptr->dof_idx[ID_ptr->nodeSN[i]-1][j] = ID_ptr->dofID[i*ID_ptr->dofN + j];

    int equation_index = 0;
    for (int i=0; i<Coor.nodeN; i++)
        for (int j=0; j<node_dof; j++)
            if (Equa_ptr->dof_idx[i][j] == min_int32)
                Equa_ptr->dof_idx[i][j] = (++equation_index);

    Equa_ptr->equaN = equation_index;
    //show_node_eq_index(*Equa_ptr, Coor.nodeN, node_dof);
    //printf("equation_index = %d\n",equation_index);

    Equa_ptr->row_nZN = (int* )malloc(Equa_ptr->equaN * sizeof(int));
    Equa_ptr->clm_idx = (int**)malloc(Equa_ptr->equaN * sizeof(int*));
    Equa_ptr->matrix  = (double**)malloc(Equa_ptr->equaN * sizeof(double*));
    Equa_ptr->vector  = (double* )calloc(Equa_ptr->equaN , sizeof(double));

    for (int i=0; i<Coor.nodeN; i++)
        for (int j=0; j<node_dof; j++) {

            int row_index   = Equa_ptr->dof_idx[i][j];

            if (row_index > 0) {
                Equa_ptr->clm_idx[row_index-1] = (int*   )malloc((adj_nodn[i]+1)*node_dof * sizeof(int));
                Equa_ptr->matrix [row_index-1] = (double*)malloc((adj_nodn[i]+1)*node_dof * sizeof(double));
            }
        }

    // --------------------------------------- convert graph to matrix ---------------------------------------
    Equa_ptr->nZeroN = 0;
    for (int i=0; i<Coor.nodeN; i++)

        for (int j=0; j<node_dof; j++) {

            int non_trivial = 0;
            int row_index   = Equa_ptr->dof_idx[i][j];

            if (row_index > 0) {

                for (int jj=0; jj<node_dof; jj++) {

                    int clm_index = Equa_ptr->dof_idx[i][jj];

                    if (clm_index >0)
                        Equa_ptr->clm_idx[row_index-1][non_trivial++] = clm_index;

                    else
                        continue;
                }

                for (int ii=0; ii<adj_nodn[i]; ii++)

                    for (int jj=0; jj<node_dof; jj++) {

                        int clm_index = Equa_ptr->dof_idx[ adj_topo[i][ii] -1 ][ jj ];

                        if (clm_index > 0)
                            Equa_ptr->clm_idx[row_index-1][non_trivial++] = clm_index;

                        else
                            continue;
                    }
            }

            else
                continue;

            Equa_ptr->row_nZN[row_index-1] = non_trivial;
            Equa_ptr->nZeroN += non_trivial;
        }


    for (int i=0; i<Equa_ptr->equaN; i++)
        int_qsort(Equa_ptr->clm_idx[i], Equa_ptr->row_nZN[i]);

    //show_non_trivial(*Equa_ptr);

    free_adj(Coor.nodeN, adj_nodn, adj_topo);

    printf("Initial done!\n");

    ID_ptr     = NULL;
    E_ID_ptr   = NULL;
    Emate_ptr  = NULL;
}
