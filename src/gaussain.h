/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/

#ifndef _GAUSSAIN_H_
#define _GAUSSAIN_H_

#include "fem.h"

void set_gaus_t3(Gaus_Info* G_info);

void set_gaus_q4(Gaus_Info* G_info);

void set_gaus(Gaus_Info* G_info, Mesh_Type MType);

#endif