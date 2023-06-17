/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/

#ifndef _MATRIX_COMPOSE_H_
#define _MATRIX_COMPOSE_H_

#include "fem.h"

void matrix_compose(
    Coor_Info  Coor,
    Node_Mesh  Mesh,
    Field_info *Field,
	Equat_Set* Equa,
    int Field_i
);

void set_matr(Elem_Matr* E_matr, int elem_dof, Matr_Type *M_type);

void reset_matr(Elem_Matr* E_matr, int elem_dof, Matr_Type *M_type);

void clear_matr(Elem_Matr* E_matr);

void set_elem(Elem_Info *E_info, int dim, int elem_nodeN, int varN, int gausN);

void clear_elem(Elem_Info *E_info, int gausN);

void set_testfunc(Test_Func *test_func, int nodeN, int g_dim);

void clear_testfunc(Test_Func *test_func);


#endif