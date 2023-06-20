/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "fem.h"

void write_result(Coor_Info Coor, Node_Mesh Mesh, Field_info Field, char* mesh_file, char* resl_file, char* prj_name) {

    FILE *WriteMesh;
    if((WriteMesh = fopen(mesh_file, "w")) == NULL) {
        printf("Write mesh failed!\n");
        return;
    }

    char elem_type[64];
    char elem_tag[8];
    char mesh_tag[32];

    for (int type_i=0; type_i<Mesh.typeN; type_i++) {

        switch(Mesh.type[type_i]) {
            case P1 : strcpy(elem_tag, "P1" ); strcpy(elem_type, "Point"); break;
            case L2 : strcpy(elem_tag, "L2" ); strcpy(elem_type, "Line" ); break;
            case L3 : strcpy(elem_tag, "L3" ); strcpy(elem_type, "Line" ); break;
            case T3 : strcpy(elem_tag, "T3" ); strcpy(elem_type, "Triangle"); break;
            case T6 : strcpy(elem_tag, "T6" ); strcpy(elem_type, "Triangle"); break;
            case Q4 : strcpy(elem_tag, "Q4" ); strcpy(elem_type, "Quadrilateral"); break;
            case Q8 : strcpy(elem_tag, "Q8" ); strcpy(elem_type, "Quadrilateral"); break;
            case Q9 : strcpy(elem_tag, "Q9" ); strcpy(elem_type, "Quadrilateral"); break;
            case W4 : strcpy(elem_tag, "W4" ); strcpy(elem_type, "Tetrahedra"); break;
            case W10: strcpy(elem_tag, "W10"); strcpy(elem_type, "Tetrahedra"); break;
            case C8 : strcpy(elem_tag, "C8" ); strcpy(elem_type, "Hexahedra"); break;
            case C20: strcpy(elem_tag, "C20"); strcpy(elem_type, "Hexahedra"); break;
            case C27: strcpy(elem_tag, "C27"); strcpy(elem_type, "Hexahedra"); break;
            case H6 : strcpy(elem_tag, "H6" ); strcpy(elem_type, "Prism"); break;
            case H15: strcpy(elem_tag, "H15"); strcpy(elem_type, "Prism"); break;
            case H18: strcpy(elem_tag, "H18"); strcpy(elem_type, "Prism"); break;
            case P5 : strcpy(elem_tag, "P5" ); strcpy(elem_type, "Pyramid"); break;
            case P13: strcpy(elem_tag, "P13"); strcpy(elem_type, "Pyramid"); break;
            case P14: strcpy(elem_tag, "P14"); strcpy(elem_type, "Pyramid"); break;
        }

        strcpy(mesh_tag, Field.Tag); strcat(mesh_tag, " - "); strcat(mesh_tag, elem_tag);

        fprintf(WriteMesh,"Mesh \"%s\" Dimension %d Elemtype %s Nnode %d\n",
                          mesh_tag, Coor.dim, elem_type, Mesh.nodeN[type_i]);

        if (type_i == 0) {

            fprintf(WriteMesh,"Coordinates\n");

            for (int i=0; i<Coor.nodeN; i++) {

                fprintf(WriteMesh, "%d", i+1);

                for (int j=0; j<Coor.dim; j++)
                    fprintf(WriteMesh, " %le", Coor.coor[i*Coor.dim+j]);

                fprintf(WriteMesh, "\n");
            }

            fprintf(WriteMesh,"End coordinates\n");
        }

        fprintf(WriteMesh, "Elements\n");

        for (int i=0; i<Mesh.scale[type_i]; i++) {

            fprintf(WriteMesh, "%d", i+1);

            for (int j=0; j<Mesh.nodeN[type_i]; j++)
                fprintf(WriteMesh, " %d", Mesh.topo[type_i][i*Mesh.nodeN[type_i] + j]);

            fprintf(WriteMesh, "\n");
        }

        fprintf(WriteMesh, "End elements\n");
    }

    fclose(WriteMesh);

    FILE *WriteResl;
    if((WriteResl = fopen(resl_file, "w")) == NULL) {
        printf("Write result failed!\n");
        return;
    }

    fprintf(WriteResl, "GID Post Results File 1.0\n");

    char var_type[255];

    double *result  = Field.Res.result;
    int     dof_num = Field.Res.dofN;

    if (dof_num == 1)
        strcpy(var_type, "Scalar");

    else
        strcpy(var_type, "Vector");

    fprintf(WriteMesh, "Result \"%s\" \"Load Analysis\"  %d %s OnNodes\n", prj_name, dof_num, var_type);

    fprintf(WriteMesh, "ComponentNames \"u\"\n");

    fprintf(WriteMesh, "Values\n");

    for (int i = 0; i <Coor.nodeN; i++) {

        fprintf(WriteMesh, "%d", i+1);

        for (int j = 0; j <dof_num; j++)
            fprintf(WriteMesh," %le", result[i*dof_num+j]);

        fprintf(WriteMesh, "\n");
    }

    fprintf(WriteMesh, "End Values\n");

    result = Field.Deriv_Res.result;
    dof_num = Field.Deriv_Res.dofN;

    if (dof_num == 1)
        strcpy(var_type, "Scalar");

    else
        strcpy(var_type, "Vector");

    fprintf(WriteMesh, "Result \"%s-derive\" \"Load Analysis\" 1 %s OnNodes\n", prj_name, var_type);

    fprintf(WriteMesh, "ComponentNames \"ux\" \"uy\"\n");

    fprintf(WriteMesh, "Values\n");

    for (int i = 0; i <Coor.nodeN; i++) {

        fprintf(WriteMesh, "%d", i+1);

        for (int j = 0; j <dof_num; j++)
            fprintf(WriteMesh," %le", result[i*dof_num+j]);

        fprintf(WriteMesh, "\n");
    }

    fprintf(WriteMesh, "End Values\n");

    fclose(WriteMesh);

    printf("Write results done!\n");
}