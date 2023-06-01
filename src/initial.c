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
    Field_info *Field,
    Equat_Set   *Equa
){
    int     node_dof =   Field->Res.dofN;
    double *result   =   Field->Res.result;
    Dof_Tag   *ID    = &(Field->ID);
    Elem_Tag  *E_ID  = &(Field->E_ID);
    Mesh_Mate *Emate = &(Field->Emate);

    // ---------------------------------------- initial solution space ----------------------------------------
    Field->Res.result = (double *)calloc(Coor.nodeN*node_dof, sizeof(double));
    
    result = Field->Res.result;
    
    for (int i=0; i<ID->nodeN; i++) {

        int node_SN = ID->nodeSN[i] - 1;

        for (int j=0; j<ID->dofN; j++)

            if (ID->dofID[i*ID->dofN + j] == -1 )

                result[node_SN*ID->dofN + j] = ID->dofval[i*ID->dofN + j];
    }

    // ---------------------------------------- initial material space ---------------------------------------
    Emate->typeN = Mesh.typeN;

    Emate->scale  = (int* )malloc(Emate->typeN*sizeof(int));

    memcpy(Emate->scale, Mesh.scale, Emate->typeN*sizeof(int));

    Emate->elemID = (int**)malloc(Emate->typeN*sizeof(int*));

    for (int i = 0; i < Emate->typeN; i++)
        Emate->elemID[i] = (int*)malloc(Emate->scale[i]*sizeof(int));

    for (int i = 0; i < Mesh.typeN; i++) {

        memset(Emate->elemID[i], 0, Emate->scale[i]*sizeof(int));

        for (int j = 0; j < E_ID->typeN; j++) {

            if (Mesh.type[i] == E_ID->type[j]) {
                
                for (int k = 0; k < E_ID->elemN[j]; k++)
                    Emate->elemID[i][E_ID->elemSN[j][k]-1] = E_ID->elemID[j][k];

                break;
            }
        }
    }
    //show_mesh_mate(*Emate);

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
    //for (int i=0; i<ID->tag_nodn; i++)
    //    for (int j=0; j<ID->dofN; j++)
    //        if (ID->dofID[i*ID->dofN + j] <= 0)
    //            constraint_count ++ ;

    //Equa->equaN = Coor.nodeN * node_dof - constraint_count;
    //printf("constraint_count = %d\n",constraint_count);

    int min_int32 = -2147483648;

    Equa->dof_idx = (int**)malloc(Coor.nodeN * sizeof(int*));
    for (int i=0; i<Coor.nodeN; i++) {
        Equa->dof_idx[i] = (int*)malloc(node_dof*sizeof(int));
        memcpy(Equa->dof_idx[i], &min_int32, node_dof*sizeof(int));
    }

    for (int i=0; i<ID->nodeN; i++) 
        for (int j=0; j<ID->dofN; j++)
            Equa->dof_idx[ID->nodeSN[i]-1][j] = ID->dofID[i*ID->dofN + j];

    int equation_index = 0;
    for (int i=0; i<Coor.nodeN; i++)
        for (int j=0; j<node_dof; j++)
            if (Equa->dof_idx[i][j] == min_int32)
                Equa->dof_idx[i][j] = (++equation_index);

    Equa->equaN = equation_index;
    //show_node_eq_index(*Equa, Coor.nodeN, node_dof);
    //printf("equation_index = %d\n",equation_index);
    
    Equa->row_nZN = (int* )malloc(Equa->equaN * sizeof(int));
    Equa->clm_idx = (int**)malloc(Equa->equaN * sizeof(int*));
    Equa->matrix  = (double**)malloc(Equa->equaN * sizeof(double*));
    Equa->vector  = (double* )calloc(Equa->equaN , sizeof(double));

    for (int i=0; i<Coor.nodeN; i++)
        for (int j=0; j<node_dof; j++) {

            int row_index   = Equa->dof_idx[i][j];

            if (row_index > 0) {
                Equa->clm_idx[row_index-1] = (int*   )malloc((adj_nodn[i]+1)*node_dof * sizeof(int));
                Equa->matrix [row_index-1] = (double*)malloc((adj_nodn[i]+1)*node_dof * sizeof(double));
            }
        }


    // --------------------------------------- convert graph to matrix ---------------------------------------
    Equa->nZeroN = 0;
    for (int i=0; i<Coor.nodeN; i++)

        for (int j=0; j<node_dof; j++) {

            int non_trivial = 0;
            int row_index   = Equa->dof_idx[i][j];

            if (row_index > 0) {

                for (int jj=0; jj<node_dof; jj++) {

                    int clm_index = Equa->dof_idx[i][jj];

                    if (clm_index >0)
                        Equa->clm_idx[row_index-1][non_trivial++] = clm_index;
                    
                    else
                        continue;
                }

                for (int ii=0; ii<adj_nodn[i]; ii++)

                    for (int jj=0; jj<node_dof; jj++) {

                        int clm_index = Equa->dof_idx[ adj_topo[i][ii] -1 ][ jj ];

                        if (clm_index > 0)
                            Equa->clm_idx[row_index-1][non_trivial++] = clm_index;

                        else
                            continue;
                    }
            }

            else
                continue;
            
            Equa->row_nZN[row_index-1] = non_trivial;
            Equa->nZeroN += non_trivial;
        }


    for (int i=0; i<Equa->equaN; i++)
        int_qsort(Equa->clm_idx[i], Equa->row_nZN[i]);

    //show_non_trivial(*Equa);

    free_adj(Coor.nodeN, adj_nodn, adj_topo);

    printf("Initial done!\n");

    result = NULL;
    ID     = NULL;
    E_ID   = NULL;
    Emate  = NULL;
}
