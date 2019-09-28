/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define maxsize 32

typedef enum Mesh_Type {P1=1, L2, L3, T3, T6, Q4, Q8, Q9, W4, W10, C8, C20, C27, H6, H15, H18, P5, P13, P14} Mesh_Type;
typedef enum Matr_Type {lump=1, dist} Matr_Type;

// -------------------------- global data -------------------------------
typedef struct Coor_Info
{
    int dim;                        // coordinates dim
    int nodeN;                      // total nodes count
    double *coor;                   // all nodes coordinates                        [node_i*dim + dim_j] = <coor value>
}Coor_Info;

typedef struct Node_Mesh
{
	int   typeN;                    // type count
    int  *l_dim;                    // element local dim of each type               [type_i] = <local dim>
    int  *nodeN;                    // element nodes count of each type             [type_i] = <nodes count>
    int  *scale;                    // element count of each type                   [type_i] = <elems count>
    int **topo;                     // mesh topology of all types                   [type_i][elem_i*elem_nodeN[type_i] + node_j] = <global node S.N.>
    Mesh_Type *type;                // element type name of each type               [type_i] = <Mesh Type>
}Node_Mesh;

// ------------------------- field data ------------------------------------
typedef struct Dof_Tag
{
    int     dofN;                   // dof number a node has
    int     nodeN;                  // total number of nodes has constraint dof
    int    *nodeSN;                 // S.N. of nodes has constraint dof             [index]    = <node S.N.>
    int    *dofID;                  // ID of dof                                    [i*dofn+j] = <ID S.N.  >
    double *dofval;                 // value of dof                                 [i*dofn+j] = <ID value >
}Dof_Tag;

typedef struct Init_Data
{
    int dofN;                       // dof number a node has
    int nodeN;                      // total nodes count
    int order;                      // time derivatives order
    double *init_0;                 // value of initial displacement
    double *init_1;                 // value of initial velocity
    double *init_2;                 // value of initial acceleration
}Init_Data;

typedef struct Elem_Tag
{
    int   typeN;
    int  *elemN;
    int **elemSN;
    int **elemID;
    Mesh_Type *type;
}Elem_Tag;

typedef struct Materail
{
	int mateN;                      // material count
    int varN;                       // material parameter count
	double* mate;                   // materail parameter values                    [mate_i*mate_cont + para_i] = <mate parameter value>
}Materail;

typedef struct Mesh_Mate
{
    int   typeN;
    int  *scale;
    int **elemID;
}Mesh_Mate;

typedef struct Result
{
    int dofN;                       //
    int nodeN;                      //
    double *result;                 //
}Result;

typedef struct Field_info
{
    Result    Res;
    Dof_Tag   ID;
    Init_Data Init;
    Elem_Tag  E_ID;
    Materail  Mate;
    Mesh_Mate Emate;
}Field_info;

// ------------------
typedef struct Elem_Matr
{
    double *matr_0;     // stif
    double *matr_1;     // damp
    double *matr_2;     // mass
    double *matr_3;     // high order mass
    double *left_matr;
    double *righ_vect;
}Elem_Matr;

typedef struct Gaus_Info
{
    int gausN;          // gaussian node number
    int l_dim;          // gaussian dim
    double *gcoor;      // gaussian node coordinates
    double *gweig;      // gaussian node weight
}Gaus_Info;

typedef struct Elem_Info
{
    int  l_dim;         // element local dimension
    int  g_dim;         // element global dimension
    int  nodeN;         // element node count
    int  elemID;        // element material S.N.
	int *topo;          // element nodes S.N.
    double  *coor;      // element nodes coordinates
    double  *coup;      // element XX coupled value
    double  *mate;      // element materail parameter values
    double **refr;      // element materail reference shape values
}Elem_Info;

typedef struct Equation_Set
{
	int   equaN;        // total number of equations
    int   nZeroN;       // total number of non-trivials
    int  *row_nZN;      // counts of non-trivials per row        [eq   S.N.]           = <count>
    int **clm_idx;      // non-trivials column index per row     [eq   S.N.][index]    = <clm S.N.>
    int **dof_idx;      // equations S.N. of dofs on every node  [node S.N.][dof S.N.] = <eq  S.N.>
	double **matrix;
    double  *vector;
}Equat_Set;
