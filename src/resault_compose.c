/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "fem.h"

void result_compose( Equat_Set Equa, Field_info Field, int total_nodes ) {

    double *result  = Field.Res.result;
    int     dof_num = Field.Res.dofN;

    for (int i=0; i<total_nodes; i++) {

        for (int j=0; j<dof_num; j++) {

            int id_SN = Equa.dof_idx[i][j];

            if (id_SN > 0)
                result[i*dof_num + j] = Equa.vector[id_SN - 1];
        }
    }

    printf("result composed done!\n");
}