/*
 Copyright: Copyright (c) 2018
 Created: 2018-4-19
 Author: Zhang_Licheng
 Title: complex tensor expression in dummy index summation
 All rights reserved
*/

#ifndef _READMESH_H_
#define _READMESH_H_

#include "fem.h"

void readmesh(
    Coor_Info   *Coor,
    Node_Mesh   *Mesh,
    Field_info  *Field,
    int         *field_SN,
    char        *dat_file
);

int assign_mesh_type(char* type_str, Mesh_Type *Mtype, int type_i);

void assign_local_dim(Mesh_Type type, int *l_dim);

#endif