/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "shap_func.h"

double lagrange_shapfunc_t3(const double *shap_coor, int node_i) // Triangle
{
    double u,v;
    double shap_val = 0.0;
    u = shap_coor[0];
    v = shap_coor[1];
    switch (node_i)
    {
    case 1: shap_val = +u;         break;
    case 2: shap_val = +v;         break;
    case 3: shap_val = +(+1.-u-v); break;
    default: break;
    }
    return shap_val;
}
double lagrange_deriva_shapfunc_t3(const double *shap_coor, int node_i, int coor_i) // Triangle
{
    //double u,v;
    double shap_val = 0.0;
    //u = shap_coor[0];
    //v = shap_coor[1];
    switch (node_i)
    {
    case 1: 
        switch(coor_i){
            case 1: shap_val = 1.; break;
            case 2: shap_val = 0.; break;
            default: break;
        }
        break;
    case 2: 
        switch(coor_i){
            case 1: shap_val = 0.; break;
            case 2: shap_val = 1.; break;
            default: break;
        }
        break;
    case 3: 
        switch(coor_i){
            case 1: shap_val = -1.; break;
            case 2: shap_val = -1.; break;
            default: break;
        }
        break;
    default: break;
    }
    return shap_val;
}
double lagrange_shapfunc_q4(const double *shap_coor, int node_i) // Quadrilateral
{
    double u,v;
    double shap_val = 0.0;
    u = shap_coor[0];
    v = shap_coor[1];
    switch (node_i)
    {
    case 1: shap_val = +(+1.-u) * (+1.-v) * 0.25; break;
    case 2: shap_val = +(+1.+u) * (+1.-v) * 0.25; break;
    case 3: shap_val = +(+1.+u) * (+1.+v) * 0.25; break;
    case 4: shap_val = +(+1.-u) * (+1.+v) * 0.25; break;
    default: break;
    }
    return shap_val;
}
double lagrange_deriva_shapfunc_q4(const double *shap_coor, int node_i, int coor_i) // Quadrilateral
{
    double u,v;
    double shap_val = 0.0;
    u = shap_coor[0];
    v = shap_coor[1];
    switch (node_i)
    {
    case 1: 
        switch(coor_i){
            case 1: shap_val = -(+1.-v) * 0.25; break;
            case 2: shap_val = -(+1.-u) * 0.25; break;
            default: break;
        }
        break;
    case 2: 
        switch(coor_i){
            case 1: shap_val = +(+1.-v) * 0.25; break;
            case 2: shap_val = -(+1.+u) * 0.25; break;
            default: break;
        }
        break;
    case 3: 
        switch(coor_i){
            case 1: shap_val = +(+1.+v) * 0.25; break;
            case 2: shap_val = +(+1.+u) * 0.25; break;
            default: break;
        }
        break;
    case 4:
        switch(coor_i){
            case 1: shap_val = -(+1.+v) * 0.25; break;
            case 2: shap_val = +(+1.-u) * 0.25; break;
            default: break;
        }
        break;
    default: break;
    }
    return shap_val;
}
double lagrange_shapfunc_w4(const double *shap_coor, int node_i) // Tetrahedron
{
    double u,v,w;
    double shap_val = 0.0;
    u = shap_coor[0];
    v = shap_coor[1];
    w = shap_coor[2];
    switch (node_i)
    {
    case 1:  shap_val = +u;           break;
    case 2:  shap_val = +v;           break;
    case 3:  shap_val = +w;           break;
    case 4:  shap_val = +(+1.-u-v-w); break;
    default: break;
    }
    return shap_val;
}
double lagrange_deriva_shapfunc_w4(const double *shap_coor, int node_i, int coor_i) // Tetrahedron
{
    //double u,v,w;
    double shap_val = 0.0;
    //u = shap_coor[0];
    //v = shap_coor[1];
    //w = shap_coor[2];
    switch (node_i)
    {
    case 1: 
        switch(coor_i){
            case 1: shap_val = 1.; break;
            case 2: shap_val = 0.; break;
            case 3: shap_val = 0.; break;
            default: break;
        }
        break;
    case 2: 
        switch(coor_i){
            case 1: shap_val = 0.; break;
            case 2: shap_val = 1.; break;
            case 3: shap_val = 0.; break;
            default: break;
        }
        break;
    case 3: 
        switch(coor_i){
            case 1: shap_val = 0.; break;
            case 2: shap_val = 0.; break;
            case 3: shap_val = 1.; break;
            default: break;
        }
        break;
    case 4: 
        switch(coor_i){
            case 1: shap_val = -1.; break;
            case 2: shap_val = -1.; break;
            case 3: shap_val = -1.; break;
            default: break;
        }
        break;
    default: break;
    }
    return shap_val;
}
double lagrange_shapfunc_c8(const double *shap_coor, int node_i) // Hexahedron
{
    double u,v,w;
    double shap_val = 0.0;
    u = shap_coor[0];
    v = shap_coor[1];
    w = shap_coor[2];
    switch (node_i)
    {
    case 1: shap_val = + (1.-u) * (1.-v) * (1.-w) * 0.125; break;
    case 2: shap_val = + (1.+u) * (1.-v) * (1.-w) * 0.125; break;
    case 3: shap_val = + (1.+u) * (1.+v) * (1.-w) * 0.125; break;
    case 4: shap_val = + (1.-u) * (1.+v) * (1.-w) * 0.125; break;
    case 5: shap_val = + (1.-u) * (1.-v) * (1.+w) * 0.125; break;
    case 6: shap_val = + (1.+u) * (1.-v) * (1.+w) * 0.125; break;
    case 7: shap_val = + (1.+u) * (1.+v) * (1.+w) * 0.125; break;
    case 8: shap_val = + (1.-u) * (1.+v) * (1.+w) * 0.125; break;
    default: break;
    }
    return shap_val;
}
double lagrange_deriva_shapfunc_c8(const double *shap_coor, int node_i, int coor_i) // Hexahedron
{
    double u,v,w;
    double shap_val = 0.0;
    u = shap_coor[0];
    v = shap_coor[1];
    w = shap_coor[2];
    switch (node_i)
    {
    case 1: 
        switch(coor_i){
            case 1: shap_val = - (1.-v) * (1.-w) * 0.125; break;
            case 2: shap_val = - (1.-u) * (1.-w) * 0.125; break;
            case 3: shap_val = - (1.-u) * (1.-v) * 0.125; break;
            default: break;
        }
        break;
    case 2: 
        switch(coor_i){
            case 1: shap_val = + (1.-v) * (1.-w) * 0.125; break;
            case 2: shap_val = - (1.+u) * (1.-w) * 0.125; break;
            case 3: shap_val = - (1.+u) * (1.-v) * 0.125; break;
            default: break;
        }
        break;
    case 3: 
        switch(coor_i){
            case 1: shap_val = + (1.+v) * (1.-w) * 0.125; break;
            case 2: shap_val = + (1.+u) * (1.-w) * 0.125; break;
            case 3: shap_val = - (1.+u) * (1.+v) * 0.125; break;
            default: break;
        }
        break;
    case 4: 
        switch(coor_i){
            case 1: shap_val = - (1.+v) * (1.-w) * 0.125; break;
            case 2: shap_val = + (1.-u) * (1.-w) * 0.125; break;
            case 3: shap_val = - (1.-u) * (1.+v) * 0.125; break;
            default: break;
        }
        break;
    case 5: 
        switch(coor_i){
            case 1: shap_val = - (1.-v) * (1.+w) * 0.125; break;
            case 2: shap_val = - (1.-u) * (1.+w) * 0.125; break;
            case 3: shap_val = + (1.-u) * (1.-v) * 0.125; break;
            default: break;
        }
        break;
    case 6: 
        switch(coor_i){
            case 1: shap_val = + (1.-v) * (1.+w) * 0.125; break;
            case 2: shap_val = - (1.+u) * (1.+w) * 0.125; break;
            case 3: shap_val = + (1.+u) * (1.-v) * 0.125; break;
            default: break;
        }
        break;
    case 7: 
        switch(coor_i){
            case 1: shap_val = + (1.+v) * (1.+w) * 0.125; break;
            case 2: shap_val = + (1.+u) * (1.+w) * 0.125; break;
            case 3: shap_val = + (1.+u) * (1.+v) * 0.125; break;
            default: break;
        }
        break;
    case 8: 
        switch(coor_i){
            case 1: shap_val = - (1.+v) * (1.+w) * 0.125; break;
            case 2: shap_val = + (1.-u) * (1.+w) * 0.125; break;
            case 3: shap_val = + (1.-u) * (1.+v) * 0.125; break;
            default: break;
        }
    default: break;
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
    case W4: 
        shap_func.lagrange_shapfunc_ptr = lagrange_shapfunc_w4;
        shap_func.lagrange_deriva_shapfunc_ptr = lagrange_deriva_shapfunc_w4;
        break;
    case C8: 
        shap_func.lagrange_shapfunc_ptr = lagrange_shapfunc_c8;
        shap_func.lagrange_deriva_shapfunc_ptr = lagrange_deriva_shapfunc_c8;
        break;
    default:
        break;
    }
    return shap_func;
}
