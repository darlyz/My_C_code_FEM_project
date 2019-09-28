/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "fem.h"

void set_gaus(Gaus_Info* G_info)
{
	G_info->gausN = 4;
	G_info->l_dim = 2;
	G_info->gcoor = (double*)malloc(G_info->gausN*G_info->l_dim*sizeof(double));
	G_info->gweig = (double*)malloc(G_info->gausN*sizeof(double));
	
	G_info->gcoor[(1-1)*(4)+1-1] =  5.773502692e-001;
    G_info->gcoor[(2-1)*(4)+1-1] =  5.773502692e-001;
	
    G_info->gweig[1-1]=1.000000000e+000;
	
    G_info->gcoor[(1-1)*(4)+2-1] =  5.773502692e-001;
    G_info->gcoor[(2-1)*(4)+2-1] = -5.773502692e-001;
	
    G_info->gweig[2-1]=1.000000000e+000;
	
    G_info->gcoor[(1-1)*(4)+3-1] = -5.773502692e-001;
    G_info->gcoor[(2-1)*(4)+3-1] =  5.773502692e-001;
	
    G_info->gweig[3-1]=1.000000000e+000;
	
    G_info->gcoor[(1-1)*(4)+4-1] = -5.773502692e-001;
    G_info->gcoor[(2-1)*(4)+4-1] = -5.773502692e-001;
	
    G_info->gweig[4-1]=1.000000000e+000;
}