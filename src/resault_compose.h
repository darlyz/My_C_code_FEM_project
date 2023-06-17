/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/

#ifndef _RESAULT_COMPOSE_H_
#define _RESAULT_COMPOSE_H_

#include "fem.h"

typedef struct Derivative_Resault
{
    int dofN;
    int nodeN;
    double *result;
    int *accum;
}Deriv_Res;

void result_compose( Equat_Set Equa, Field_info Field, int total_nodes );

#endif