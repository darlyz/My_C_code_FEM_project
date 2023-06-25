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

void response_result( Equat_Set Equa, Field_info* field_ptr, int total_nodes );

void derivate_result(Field_info* field_ptr, Coor_Info Coor, Node_Mesh Mesh);

#endif