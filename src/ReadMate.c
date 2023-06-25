/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "fem.h"

void readmate(Field_info *fields, int field_num, char* mat_file) {

    FILE *mat_file_rdptr;
    if((mat_file_rdptr = fopen(mat_file,"r"))==NULL) {
        printf("Read materials failed!\n");
        return;
    }
    Materail *Mate_ptr;

    int local_field_num;
    fscanf(mat_file_rdptr, "%d", &(local_field_num));

    if (local_field_num != field_num) {
        printf("field number mismatched!\n");
        return;
    }

    for (int i=0; i<local_field_num; i++) {

        Mate_ptr = &(fields[i].Mate);

        fscanf(mat_file_rdptr, "%d", &(Mate_ptr->mateN));
        fscanf(mat_file_rdptr, "%d", &(Mate_ptr->varN));

        Mate_ptr->mate = (double*)malloc(Mate_ptr->mateN * Mate_ptr->varN * sizeof(double));

        for (int mat_i=0; mat_i<Mate_ptr->mateN; mat_i++)
            for (int var_i=0; var_i<Mate_ptr->varN; var_i++)
                fscanf(mat_file_rdptr, "%le", &(Mate_ptr->mate[mat_i * (Mate_ptr->varN) + var_i]));
    }

    fclose(mat_file_rdptr);
    printf("Read materials done!\n");
}