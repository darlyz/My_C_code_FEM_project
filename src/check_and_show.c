/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "fem.h"

// used it in elemcalc.c
void show_elem_stif(int nodeN, Elem_Matr E_matr) {
	for (int i=0; i<nodeN*nodeN; i++)
	    printf("%e\n",E_matr.matr_0[i]);
    
	for (int i=0; i<nodeN; i++)
	    printf("%e\n",E_matr.righ_vect[i]);

	printf("\n");
}

// used it in initial.c
void show_adj(int* adj_nodn, int** adj_topo, int total_nodes) {
    for (int i=0; i<total_nodes; i++) {
        printf("%d, %d : ",i+1,adj_nodn[i]);
        for (int j=0; j<adj_nodn[i]; j++)
            printf("%d ",adj_topo[i][j]);
        printf("\n");
    }
}

// used it in initial.c
void show_node_eq_index(Equat_Set Equa, int total_nodes, int node_dof) {
    printf("equaN = %d\n",Equa.equaN);
    for (int i=0; i<total_nodes; i++) {
        for (int j=0; j<node_dof; j++)
            printf("\t%d",Equa.dof_idx[i][j]);
        printf("\n");
    }
}

// used it in initial.c
void show_non_trivial(Equat_Set Equa) {
    printf("non triaval: %d\n",Equa.nZeroN);
    for (int i=0; i<Equa.equaN; i++) {
        printf("%d, ",i+1);
        for (int j=0; j<Equa.row_nZN[i]; j++)
            printf("%d ",Equa.clm_idx[i][j]);
        printf("\n");
    }
}

// used it in matrcalc.c
void show_elem(Elem_Info E_info, int elem_nodeN, int mate_varN, int gaus_num) {

    int dim = E_info.l_dim;

	printf("dim: %d\n",E_info.l_dim);
	printf("nodeN: %d\n",E_info.nodeN);

	printf("topo:\n");
	for (int i=0; i<elem_nodeN; i++)
		printf("%d ",E_info.topo[i]);
	printf("\n");

	printf("coor:\n");
	for (int i=0; i<E_info.nodeN; i++){
		for (int j=0; j<dim; j++)
			printf("%e ",E_info.coor[j*E_info.nodeN+i]);
		printf("\n");
	}

	printf("mate:\n");
	for (int i=0; i<mate_varN; i++)
		printf("%e ",E_info.mate[i]);
	printf("\n");

	printf("refr:\n");
	for (int i=0; i<gaus_num; i++){
		for (int j=0; j<E_info.nodeN*(dim+1); j++)
			printf("%e ",E_info.refr[i][j]);
		printf("\n");
	}
}

// used it in matrcalc.c
void show_matr(Equat_Set Equa) {
    for (int i=0; i<Equa.equaN; i++){
        printf("%d: ",i+1);
        for (int j=0; j<Equa.row_nZN[i]; j++)
            printf("%d,%e ",Equa.clm_idx[i][j],Equa.matrix[i][j]);
        printf("-->%e\n",Equa.vector[i]);
    }
}

// used it in matrcalc.c
void show_elem_matr(Elem_Matr E_matr, int ematr_size, Matr_Type *M_type) {
    int size[4];
    for (int i=0; i<4; i++){
        if     (M_type[i] == lump) size[i] = ematr_size;
        else if(M_type[i] == dist) size[i] = ematr_size*ematr_size;
    }

    printf("\nmatr0:\n");
    for (int i=0; i<size[0]; i++)
        printf("%e\n",E_matr.matr_0[i]);

    printf("\nforc:\n");
    for (int i=0; i<ematr_size; i++)
        printf("%e\n",E_matr.righ_vect[i]);
}

void show_coor(Coor_Info Coor) {
    printf("total nodes:%d, dim:%d\n",Coor.nodeN,Coor.dim);
    for (int i=0; i<Coor.nodeN; i++) {
        for (int j=0; j<Coor.dim; j++)
            printf("%le ",Coor.coor[i*Coor.dim + j]);
        printf("\n");
    }
}

void show_mesh(Node_Mesh Mesh) {
    for (int i=0; i<Mesh.typeN; i++) {
        printf("type_i=%d\n",i+1);
        printf("type = %d\n",Mesh.type[i]);
        printf("%d elems, %d nodes per elem\n",Mesh.scale[i], Mesh.nodeN[i]);
        for(int j=0; j<Mesh.scale[i];j++) {
            printf("elem_i=%d: ",j+1);
            for (int k=0; k<Mesh.nodeN[i];k++)
                printf("%d ",Mesh.topo[i][j*Mesh.nodeN[i]+k]);
            printf("\n");
        }
        printf("\n");
    }
}

void show_elem_tag(Elem_Tag E_ID) {
    for (int i=0; i<E_ID.typeN; i++) {
        printf("type=%d\n",E_ID.type[i]);
        for (int j=0; j<E_ID.elemN[i]; j++) {
            printf("%d %d\n", E_ID.elemSN[i][j], E_ID.elemID[i][j]);
        }
        printf("\n");
    }
}

void show_material(Field_info *Field, int field_SN) {
    for (int i = 0; i <field_SN; i++){
        printf("%d %d\n", Field[i].Mate.mateN, Field[i].Mate.varN);
        for (int j = 0; j <Field[i].Mate.mateN; j++) {
            for (int k = 0; k <Field[i].Mate.varN; k++)
                printf("%le ", Field[i].Mate.mate[j*Field[i].Mate.varN+k]);
            printf("\n");
        }
        printf("\n");
    }
}

void show_mesh_mate(Mesh_Mate Emate) {
    printf("\n%d mesh type\n", Emate.typeN);
    for (int i = 0; i < Emate.typeN; i++) {
        printf("type_i=%d\n",i+1);
        for (int j = 0; j < Emate.scale[i]; j++)
            printf("%d ",Emate.elemID[i][j]);
        printf("\n");
    }
}