/*
 Copyright: Copyright (c) 2018
 Created: 2023-6-14
 Author: Zhang_Licheng
 Title: complex tensor expression in dummy index summation
 All rights reserved
*/

#include "write_mesh.h"

void write_gmsh_mesh(
    Coor_Info   *Coor,
    Node_Mesh   *Mesh,
    Elem_Tag    *E_ID,
    char        *mesh_file
){
    char gmsh_file[255]; strcpy(gmsh_file, mesh_file); strcat(gmsh_file, ".msh");

    typedef enum Gmsh_Type {
        G_L2 = 1, G_T3, G_Q4, G_W4, G_C8, G_H6, G_P5, G_L3, G_T6, G_Q9, G_W10, G_C27,
        G_H18, G_P14, G_P1, G_Q8, G_C20, G_H15, G_P13
    } Gmsh_Type;

    FILE *WriteMesh;
    if((WriteMesh = fopen(gmsh_file, "w")) == NULL) {
        printf("Write mesh failed!\n");
        return;
    }

    fprintf(WriteMesh, "$MeshFormat\n2.2 0 8\n$EndMeshFormat\n");

    fprintf(WriteMesh, "$Nodes\n");

    fprintf(WriteMesh, "%d\n", Coor->nodeN);

    for (int i = 0; i < Coor->nodeN; i ++){
        fprintf(WriteMesh, "%d", i+1);
        for (int j = 0; j < Coor->dim; j ++)
            fprintf(WriteMesh, " %e", Coor->coor[i*Coor->dim + j]);
        if (Coor->dim == 1)
            fprintf(WriteMesh, " 0 0\n");
        else if (Coor->dim == 2)
            fprintf(WriteMesh, " 0\n");
        else
            fprintf(WriteMesh, "\n");
    }

    fprintf(WriteMesh, "$EndNodes\n");

    fprintf(WriteMesh, "$Elements\n");

    int elem_idx = 0;
    for (int i = 0; i < Mesh->typeN; i ++)
        elem_idx += Mesh->scale[i];
    
    fprintf(WriteMesh, "%d\n", elem_idx + 1);

    elem_idx = 0;
    for (int i = 0; i < Mesh->typeN; i ++){
        int gmsh_elem_type;
        switch (Mesh->type[i])
        {
            case P1 : gmsh_elem_type = G_P1 ; break;
            case L2 : gmsh_elem_type = G_L2 ; break;
            case L3 : gmsh_elem_type = G_L3 ; break;
            case T3 : gmsh_elem_type = G_T3 ; break;
            case T6 : gmsh_elem_type = G_T6 ; break;
            case Q4 : gmsh_elem_type = G_Q4 ; break;
            case Q8 : gmsh_elem_type = G_Q8 ; break; 
            case Q9 : gmsh_elem_type = G_Q9 ; break; 
            case W4 : gmsh_elem_type = G_W4 ; break; 
            case W10: gmsh_elem_type = G_W10; break; 
            case C8 : gmsh_elem_type = G_C8 ; break;
            case C20: gmsh_elem_type = G_C20; break; 
            case C27: gmsh_elem_type = G_C27; break; 
            case H6 : gmsh_elem_type = G_H6 ; break;
            case H15: gmsh_elem_type = G_H15; break; 
            case H18: gmsh_elem_type = G_H18; break; 
            case P5 : gmsh_elem_type = G_P5 ; break;
            case P13: gmsh_elem_type = G_P13; break; 
            case P14: gmsh_elem_type = G_P14; break;
            default: break;
        }

        for (int j = 0; j < Mesh->scale[i]; j ++){
            
            elem_idx ++;

            fprintf(WriteMesh, "%d %d 2 %d %d", elem_idx, gmsh_elem_type, E_ID->elemID[i][j], E_ID->elemID[i][j]);

            for (int k = 0; k < Mesh->nodeN[i]; k ++)
                fprintf(WriteMesh, " %d", Mesh->topo[i][j * Mesh->nodeN[i] + k]);
            
            fprintf(WriteMesh, "\n");

        }
    }

    fprintf(WriteMesh, "%d 15 2 2 2 1\n", elem_idx + 1);

    fprintf(WriteMesh, "$EndElements\n");

    fclose(WriteMesh);

}