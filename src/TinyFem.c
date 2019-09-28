/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "fem.h"

void readmesh();
void readmate();
void initial ();
void matrix_compose();
void matrsolv();
void show_coor();
void show_mesh();
void show_material();
void show_elem_tag();
void show_mesh_mate();
void result_compose();
void write_result();
void clear_coor();
void clear_mesh();
void clear_field();

int main(int argc, char* argv[])
{

    char data_file[]="./mesh/exam2.gid/exam2.dat";
    char mate_file[]="./mate/exam2.mat";
    char mesh_file[]="./mesh/exam2.gid/exam2.post.msh";
    char resl_file[]="./mesh/exam2.gid/exam2.post.res";

    Coor_Info Coor;
    Node_Mesh Mesh;

    //Field_info *Field = (Field_info*)malloc(5*sizeof(Field_info));
    Field_info Field[5];

    Equat_Set Equa;

    int field_SN = 0;

    readmesh( &Coor, &Mesh, Field, &field_SN, data_file );

    readmate( Field, field_SN, mate_file );
    
    //show_coor(Coor);
    //show_mesh(Mesh);
    //show_material(Field, field_SN);
    //for (int i=0; i<field_SN; i++) show_elem_tag(Field[i].E_ID);
    //Mesh.typeN = 1;

    Field_info *Field_A = (Field + 0);
    Field_A->Res.nodeN = Coor.nodeN;
    Field_A->Res.dofN  = 1;

    initial ( Coor, Mesh, Field_A, &Equa );

    //for (int i=0; i<Coor.nodeN; i++) printf("%le\n",Field_A->Res.result[i]);
    //for (int i=0; i<field_SN; i++) show_mesh_mate(Field[i].Emate);
    
    matrix_compose( Coor, Mesh, Field_A, &Equa );

    matrsolv( &Equa );

    result_compose( Equa, *Field_A, Coor.nodeN );

    write_result( Coor, Mesh, *Field_A, mesh_file, resl_file );

    clear_coor( &Coor );
    clear_mesh( &Mesh );
    clear_field( Field, field_SN );
}