/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/

#ifndef _CHECK_AND_SHOW_H_
#define _CHECK_AND_SHOW_H_

#include "fem.h"

// used it in elemcalc.c
void show_elem_stif(int nodeN, Elem_Matr E_matr);

// used it in initial.c
void show_adj(const int* adj_nodn, const int** adj_topo, int total_nodes);

// used it in initial.c
void show_node_eq_index(Equat_Set Equa, int total_nodes, int node_dof);

// used it in initial.c
void show_non_trivial(Equat_Set Equa);

// used it in matrcalc.c
void show_elem(Elem_Info E_info, int elem_nodeN, int mate_varN, int gaus_num);

// used it in matrcalc.c
void show_matr(Equat_Set Equa);

// used it in matrcalc.c
void show_elem_matr(Elem_Matr E_matr, int ematr_size, Matr_Type *M_type);

void show_coor(Coor_Info Coor);

void show_mesh(Node_Mesh Mesh);

void show_elem_tag(Elem_Tag E_ID);

void show_material(const Field_info *Field, int field_SN);

void show_mesh_mate(Mesh_Mate Emate);

void show_gaussain(Gaus_Info G_info);

#endif