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
    Field_info *field_ptr,
    Equat_Set  *Equa_ptr,
    int Field_i
);

void set_elem(Elem_Info *E_info_ptr, int dim, int elem_nodeN, int varN, int gausN, const Result* res_ptr);

void clear_elem(Elem_Info *E_info_ptr, int gausN);

void set_testfunc(Test_Func *test_func_ptr, int nodeN, int g_dim);

void clear_testfunc(Test_Func *test_func_ptr);

#endif