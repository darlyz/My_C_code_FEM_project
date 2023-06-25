/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "fem.h"
#include "ReadMesh.h"
#include "ReadMate.h"
#include "initial.h"
#include "matrix_compose.h"
#include "matrix_solve.h"
#include "resault_compose.h"
#include "clear_data.h"
#include "write_resault.h"
//#include "check_and_show.h"
//#include "export_mesh.h"

int main(int argc, char* argv[])
{
    char *prj = argv[1];
    char data_file[255]; strcpy(data_file, "/mnt/d/Allo_Test/test.fem/My_C_code_FEM_project/mesh/"); strcat(data_file, prj); strcat(data_file, ".gid/"); strcat(data_file, prj); strcat(data_file, ".dat");
    char mate_file[255]; strcpy(mate_file, "/mnt/d/Allo_Test/test.fem/My_C_code_FEM_project/mate/"); strcat(mate_file, prj); strcat(mate_file, ".mat");
    char mesh_file[255]; strcpy(mesh_file, "/mnt/d/Allo_Test/test.fem/My_C_code_FEM_project/mesh/"); strcat(mesh_file, prj); strcat(mesh_file, ".gid/"); strcat(mesh_file, prj); strcat(mesh_file, ".post.msh");
    char resl_file[255]; strcpy(resl_file, "/mnt/d/Allo_Test/test.fem/My_C_code_FEM_project/mesh/"); strcat(resl_file, prj); strcat(resl_file, ".gid/"); strcat(resl_file, prj); strcat(resl_file, ".post.res");

    //char data_file[]="./mesh/exam2.gid/exam2.dat";
    //char mate_file[]="./mate/exam2.mat";
    //char mesh_file[]="./mesh/exam2.gid/exam2.post.msh";
    //char resl_file[]="./mesh/exam2.gid/exam2.post.res";

    Coor_Info Coor;
    Node_Mesh Mesh;

    //Field_info *fields = (Field_info*)malloc(5*sizeof(Field_info));
    Field_info fields[5];

    int field_SN = 0;

    readmesh( &Coor, &Mesh, fields, &field_SN, data_file );
    //char msh_file[255]; strcpy(msh_file, "./mesh/"); strcat(msh_file, prj);
    //write_gmsh_mesh( &Coor, &Mesh, &(fields[0].E_ID), msh_file);
    readmate( fields, field_SN, mate_file );

    //show_coor(Coor);
    //show_mesh(Mesh);
    //show_material(fields, field_SN);
    //for (int i=0; i<field_SN; i++) show_elem_tag(fields[i].E_ID);
    //Mesh.typeN = 1;

    Equat_Set Equa;

    Field_info *Field_ES = (fields + 0);
    Field_ES->Res.nodeN = Coor.nodeN;
    Field_ES->Res.dofN  = 1;
    strcpy(Field_ES->Tag, "ES");
    strcpy(Field_ES->Name, "ElectroStatic");
    initial ( Coor, Mesh, Field_ES, &Equa );

    //for (int i=0; i<Coor.nodeN; i++) printf("%le\n",Field_ES->Res.result[i]);
    //for (int i=0; i<field_SN; i++) show_mesh_mate(fields[i].Emate);

    matrix_compose( Coor, Mesh, Field_ES, &Equa, 0 );
    matrsolv( &Equa );
    response_result( Equa, Field_ES, Coor.nodeN );
    //derivate_result( Field_ES, Coor, Mesh);
    write_result( Coor, Mesh, *Field_ES, mesh_file, resl_file, prj );

    clear_coor( &Coor );
    clear_mesh( &Mesh );
    clear_field( fields, field_SN );

    return 1;
}