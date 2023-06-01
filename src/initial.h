/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/

#ifndef _INITIAL_H_
#define _INITIAL_H_

#include "fem.h"

void free_adj(int total_nodes, int* adj_nodn, int** adj_topo);

void initial(
    Coor_Info   Coor,
    Node_Mesh   Mesh,
    Field_info *Field,
    Equat_Set   *Equa
);

#endif