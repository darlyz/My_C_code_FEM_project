/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
double lagrange_shapfunc(double *shap_coor, int node_i) //quadrilateral
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
double lagrange_deriva_shapfunc(double *shap_coor, int node_i, int coor_i) // quadrilateral
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