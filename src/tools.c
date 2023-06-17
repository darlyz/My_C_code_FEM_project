/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define init_adj_num 20

// return sdet
double det(const double* matr, int dim) {
    if      (dim == 1) return matr[0];
    else if (dim == 2) return matr[(1-1)*dim + 1-1]*matr[(2-1)*dim + 2-1] - matr[(1-1)*dim + 2-1]*matr[(2-1)*dim + 1-1];
    else
    {
        double  sdet  = 0.0;
        static double sub_matr[9];
        for (int i=1; i<=dim; i++)
        {
            int sign = (i%2==0)?-1:1;
            for (int r=2,rr=1; r<=dim; r++,rr++)
                for (int c=1,cc=0; c<=dim; c++)
                {
                    if (c == i) continue;
                    sub_matr[(rr-1)*(dim-1) + (cc++)] = matr[(r-1)*dim + c-1];
                }
            sdet += sign*matr[i-1]*det(sub_matr,dim-1);
        }
        return sdet;
    }
}

// return *inv_matr, |inv_matr|
double inv(double* inv_matr, const double* matr, int dim) {
    double sdet = det(matr,dim);
    if (fabs(sdet) < 1e-30) return 1e30;
    static double sub_matr[9];
    for (int i=1; i<=dim; i++)
        for (int j=1; j<=dim; j++)
        {
            int sign = ((i+j)%2==0)?1:-1;
            for (int r=1,rr=0; r<=dim; r++)
            {
                if(r == i) continue;
                rr++;
                for (int c=1,cc=0; c<=dim; c++)
                {
                    if(c == j) continue;
                    cc++;
                    sub_matr[(rr-1)*(dim-1) + cc-1] = matr[(r-1)*dim + c-1];
                }
            }
            inv_matr[(j-1)*dim + i-1] = sign*det(sub_matr,dim-1)/sdet;
        }
    return det(inv_matr, dim);
}

int compare_int(const void *value1, const void *value2) {
    return *(int*)value1 - *(int*)value2;
}

void int_qsort(int* array, int array_len) {
    qsort(array, array_len, sizeof(int), compare_int);
}

// return -1 if found, return insert index if not found
int Binary_Search(const int* dest, int dest_len, int key) {
    int idx_start = 0;
    int idx_end   = dest_len-1;
    int idx_mid   = -1;

    if (dest_len == 0)
        return 0;

    while(1)
    {
        idx_mid = idx_start + (idx_end - idx_start)/2;

        if (key < dest[idx_mid])
            idx_end   = idx_mid - 1;

        else
        if (key > dest[idx_mid])
            idx_start = idx_mid + 1;

        else
            return -1;

        if (idx_start >= idx_end) {

            if (idx_mid == idx_start)
                return idx_mid;

            else if (idx_mid == idx_end)
                return idx_mid + 1;
        }
    }
}

// return -1 if not found, return matched index if found
int Binary_Search_(const int* dest, int dest_len, int key) {
    int idx_start = 0;
    int idx_end   = dest_len-1;
    int idx_mid   = -1;

    if (dest_len == 0)
        return -1;

    while(idx_start <= idx_end)
    {
        idx_mid = idx_start + (idx_end - idx_start)/2;

        if (key < dest[idx_mid])
            idx_end   = idx_mid - 1;

        else
        if (key > dest[idx_mid])
            idx_start = idx_mid + 1;

        else
            return idx_mid;
    }
    return -1;
}

// return -1 if found, return insert index if not found
int traversal_search(const int* dest, int dest_len, int key) {
    for (int i=0; i<dest_len; i++) {
        if ( dest[0] > key )
            return 0;

        else if ( dest[dest_len-1] < key )
            return dest_len;

        else if ( dest[i] < key && dest[i+1] > key)
            return i+1;
    }
    return -1;
}

// return -1 if not found, return matched index if found
int traversal_search_(const int* dest, int dest_len, int key) {
    for (int i=0; i<dest_len; i++) {
        if ( dest[0] > key )
            return -1;

        else if ( dest[i] == key )
            return i;
    }
    return -1;
}

// insert insert_SN into dest[] and length++
void insert_node(int** dest, int* length, int init_length, int insert_SN) {

    if ((*length) == 0) {
        (*dest)[0] = insert_SN;
        (*length) ++;
    }

    else {

        int insert_idx = Binary_Search((*dest), (*length), insert_SN);

        if (insert_idx != -1) {

            if ((*length) % init_length == 0)
                (*dest) = (int*)realloc((*dest), ((*length) / init_length + 1) * init_length * sizeof(int));

            memmove((*dest) + insert_idx + 1, (*dest) + insert_idx, ((*length) - insert_idx) * sizeof(int));
            (*dest)[insert_idx] = insert_SN;
            (*length) ++;
        }
    }
}

// insert insert_SN into adj_topo[node_SN-1][] and adj_nodn[node_SN-1]++
void insert_node_(int node_SN, int insert_SN, int* adj_nodn, int** adj_topo) {

    node_SN --;

    if (adj_nodn[node_SN] == 0) {
        adj_topo[node_SN][0] = insert_SN;
        adj_nodn[node_SN] ++;
    }

    else {

        int insert_idx = Binary_Search(adj_topo[node_SN], adj_nodn[node_SN], insert_SN);

        if (insert_idx != -1) {

            if (adj_nodn[node_SN] % init_adj_num == 0)
                adj_topo[node_SN] = (int*)realloc(adj_topo[node_SN], (adj_nodn[node_SN]/init_adj_num + 1)*init_adj_num * sizeof(int));

            memmove(adj_topo[node_SN] + insert_idx + 1, adj_topo[node_SN] + insert_idx, (adj_nodn[node_SN] - insert_idx) * sizeof(int));
            adj_topo[node_SN][insert_idx] = insert_SN;
            adj_nodn[node_SN] ++;
        }
    }
}