/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#ifndef _CLEAR_DATA_H_
#define _CLEAR_DATA_H_

#include "fem.h"

void clear_coor(Coor_Info *Coor);

void clear_mesh(Node_Mesh *Mesh);

void clear_ID(Dof_Tag *ID);

void clear_init(Init_Data *Init);

void clear_E_ID(Elem_Tag *E_ID);

void clear_Emate(Mesh_Mate *Emate);

void clear_mate(Materail *Mate);

void clear_result(Result *Res);

void clear_field(Field_info *Field, int Field_num);

#endif