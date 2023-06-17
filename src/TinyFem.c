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
#include "check_and_show.h"
//#include "export_mesh.h"

int main(int argc, char* argv[])
{
    char *prj = argv[1];
    char data_file[255]; strcpy(data_file, "./mesh/"); strcat(data_file, prj); strcat(data_file, ".gid/"); strcat(data_file, prj); strcat(data_file, ".dat");
    char mate_file[255]; strcpy(mate_file, "./mate/"); strcat(mate_file, prj); strcat(mate_file, ".mat");
    char mesh_file[255]; strcpy(mesh_file, "./mesh/"); strcat(mesh_file, prj); strcat(mesh_file, ".gid/"); strcat(mesh_file, prj); strcat(mesh_file, ".post.msh");
    char resl_file[255]; strcpy(resl_file, "./mesh/"); strcat(resl_file, prj); strcat(resl_file, ".gid/"); strcat(resl_file, prj); strcat(resl_file, ".post.res");

    //char data_file[]="./mesh/exam2.gid/exam2.dat";
    //char mate_file[]="./mate/exam2.mat";
    //char mesh_file[]="./mesh/exam2.gid/exam2.post.msh";
    //char resl_file[]="./mesh/exam2.gid/exam2.post.res";

    Coor_Info Coor;
    Node_Mesh Mesh;

    //Field_info *Field = (Field_info*)malloc(5*sizeof(Field_info));
    Field_info Field[5];

    int field_SN = 0;

    readmesh( &Coor, &Mesh, Field, &field_SN, data_file );
    //char msh_file[255]; strcpy(msh_file, "./mesh/"); strcat(msh_file, prj);
    //write_gmsh_mesh( &Coor, &Mesh, &(Field[0].E_ID), msh_file);
    readmate( Field, field_SN, mate_file );

    //show_coor(Coor);
    //show_mesh(Mesh);
    //show_material(Field, field_SN);
    //for (int i=0; i<field_SN; i++) show_elem_tag(Field[i].E_ID);
    //Mesh.typeN = 1;

    Equat_Set Equa;

    Field_info *Field_A = (Field + 0);
    Field_A->Res.nodeN = Coor.nodeN;
    Field_A->Res.dofN  = 1;
    strcpy(Field_A->Tag, "ES");
    strcpy(Field_A->Name, "ElectroStatic");
    initial ( Coor, Mesh, Field_A, &Equa );

    //for (int i=0; i<Coor.nodeN; i++) printf("%le\n",Field_A->Res.result[i]);
    //for (int i=0; i<field_SN; i++) show_mesh_mate(Field[i].Emate);

    matrix_compose( Coor, Mesh, Field_A, &Equa, 0 );
    matrsolv( &Equa );
    result_compose( Equa, *Field_A, Coor.nodeN );
    write_result( Coor, Mesh, *Field_A, mesh_file, resl_file, prj );

    clear_coor( &Coor );
    clear_mesh( &Mesh );
    clear_field( Field, field_SN );

    return 1;
}