/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "shap_func.h"

double lagrange_shapfunc_t3(double *shap_coor, int node_i) //triangle
{
	double u,v;
	double shap_val = 0.0;
	u = shap_coor[0];
	v = shap_coor[1];
	switch (node_i)
    {
    case 1:  shap_val = +u;
        break;
    case 2:  shap_val = +v;
        break;
    case 3:  shap_val = +(+1.-u-v);
        break;
	}
	return shap_val;
}
double lagrange_deriva_shapfunc_t3(double *shap_coor, int node_i, int coor_i) // triangle
{
	double u,v;
	double shap_val = 0.0;
	u = shap_coor[0];
	v = shap_coor[1];
	switch (node_i)
    {
    case 1: 
		switch(coor_i){
			case 1: shap_val = 1.; break;
			case 2: shap_val = 0.; break;
		}
        break;
    case 2: 
		switch(coor_i){
			case 1: shap_val = 0.; break;
			case 2: shap_val = 1.; break;
		}
        break;
    case 3: 
		switch(coor_i){
			case 1: shap_val = -1.; break;
			case 2: shap_val = -1.; break;
		}
        break;
	}
	return shap_val;
}
double lagrange_shapfunc_q4(double *shap_coor, int node_i) //quadrilateral
{
	double u,v;
	double shap_val = 0.0;
	u = shap_coor[0];
	v = shap_coor[1];
	switch (node_i)
    {
    case 1:  shap_val = +(+1.-u)*(+1.-v)/4.;
        break;
    case 2:  shap_val = +(+1.+u)*(+1.-v)/4.;
        break;
    case 3:  shap_val = +(+1.+u)*(+1.+v)/4.;
        break;
    case 4:  shap_val = +(+1.-u)*(+1.+v)/4.;
        break;
	}
	return shap_val;
}
double lagrange_deriva_shapfunc_q4(double *shap_coor, int node_i, int coor_i) // quadrilateral
{
	double u,v;
	double shap_val = 0.0;
	u = shap_coor[0];
	v = shap_coor[1];
	switch (node_i)
    {
    case 1: 
		switch(coor_i){
			case 1: shap_val = -(+1.-v)/4.; break;
			case 2: shap_val = -(+1.-u)/4.; break;
		}
        break;
    case 2: 
		switch(coor_i){
			case 1: shap_val = +(+1.-v)/4.; break;
			case 2: shap_val = -(+1.+u)/4.; break;
		}
        break;
    case 3: 
		switch(coor_i){
			case 1: shap_val = +(+1.+v)/4.; break;
			case 2: shap_val = +(+1.+u)/4.; break;
		}
        break;
    case 4:
		switch(coor_i){
			case 1: shap_val = -(+1.+v)/4.; break;
			case 2: shap_val = +(+1.-u)/4.; break;
		}
        break;
	}
	return shap_val;
}

Shap_Func get_shap_func(Mesh_Type MType){

	Shap_Func shap_func;

	switch (MType)
    {
    case T3: 
		shap_func.lagrange_shapfunc_ptr = lagrange_shapfunc_t3;
		shap_func.lagrange_deriva_shapfunc_ptr = lagrange_deriva_shapfunc_t3;
		break;
    case Q4:
		shap_func.lagrange_shapfunc_ptr = lagrange_shapfunc_q4;
		shap_func.lagrange_deriva_shapfunc_ptr = lagrange_deriva_shapfunc_q4;
		break;
    default:
        break;
    }
	return shap_func;
}
