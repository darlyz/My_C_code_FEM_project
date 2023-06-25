/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/

#ifndef _INITIAL_H_
#define _INITIAL_H_

#include "fem.h"

void initial(
    Coor_Info   Coor,
    Node_Mesh   Mesh,
    Field_info *field_ptr,
    Equat_Set  *Equa_ptr
);

#endif