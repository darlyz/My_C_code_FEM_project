/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/

#ifndef _RESAULT_COMPOSE_H_
#define _RESAULT_COMPOSE_H_

#include "fem.h"
#include "shap_func.h"

void response_result( Equat_Set Equa, Field_info* Field, int total_nodes );

void derivate_result(Field_info* Field, Coor_Info Coor, Node_Mesh Mesh);

void deriv_elem_calc(int elem_i, Gaus_Info G_info, Elem_Info E_info,
                     Derivative_Resault* deriv_res, Shap_Func shap_func, Test_Func test_func);

#endif