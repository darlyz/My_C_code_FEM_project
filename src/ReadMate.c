/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "fem.h"

void readmate(Field_info *Field, int field_num, char* mat_file) {
    
    FILE *ReadMate;
    if((ReadMate = fopen(mat_file,"r"))==NULL) {
        printf("Read materials failed!\n");
        return;
    }
    Materail *Mate;

    int local_field_num;
    fscanf(ReadMate, "%d", &(local_field_num));

    if (local_field_num != field_num) {
        printf("field number mismatched!\n");
        return;
    }

    for (int i=0; i<local_field_num; i++) {

        Mate = &(Field[i].Mate);

        fscanf(ReadMate, "%d", &(Mate->mateN));
        fscanf(ReadMate, "%d", &(Mate->varN));

        Mate->mate = (double*)malloc(Mate->mateN * Mate->varN * sizeof(double));

        for (int mat_i=0; mat_i<Mate->mateN; mat_i++)
            for (int var_i=0; var_i<Mate->varN; var_i++)
                fscanf(ReadMate, "%le", &(Mate->mate[mat_i * (Mate->varN) + var_i]));
    }

    fclose(ReadMate);
    printf("Read materials done!\n");
}