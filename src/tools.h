/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/

#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <stdlib.h>
#include <math.h>
#include <string.h>

// return sdet
double det(double* matr, int dim);

// return *inv_matr, |inv_matr|
double inv(double* inv_matr, double* matr, int dim);

int compare_int(const void *value1, const void *value2);

void int_qsort(int* array, int array_len);

// return -1 if found, return insert index if not found
int Binary_Search(int* dest, int dest_len, int key);

// return -1 if not found, return matched index if found
int Binary_Search_(int* dest, int dest_len, int key);

// return -1 if found, return insert index if not found
int traversal_search(int* dest, int dest_len, int key);

// return -1 if not found, return matched index if found
int traversal_search_(int* dest, int dest_len, int key);

// insert insert_SN into dest[] and length++
void insert_node(int** dest, int* length, int init_length, int insert_SN);

// insert insert_SN into adj_topo[node_SN-1][] and adj_nodn[node_SN-1]++
void insert_node_(int node_SN, int insert_SN, int* adj_nodn, int** adj_topo);

#endif