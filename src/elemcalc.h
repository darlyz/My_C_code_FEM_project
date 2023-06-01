/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#ifndef _ELEMCALC_H_
#define _ELEMCALC_H_

#include "fem.h"
#include "shap_func.h"
#include "matrix_compose.h"

void elemcalc (
    int elem_n,
    Gaus_Info  G_info,
	Elem_Info  E_info,
    Elem_Matr *E_matr,
	Shap_Func  shap_func,
	Test_Func  test_func
);

#endif