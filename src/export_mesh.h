/*
 Copyright: Copyright (c) 2018
 Created: 2023-6-14
 Author: Zhang_Licheng
 Title: complex tensor expression in dummy index summation
 All rights reserved
*/

#ifndef _EXPORT_MESH_H_
#define _EXPORT_MESH_H_

#include "fem.h"

void write_gmsh_mesh(
    Coor_Info   *Coor,
    Node_Mesh   *Mesh,
    Elem_Tag    *E_ID,
    char        *mesh_file
);

#endif