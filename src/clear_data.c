/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "fem.h"

#define show_tag 1
#define showFtag 0

void clear_coor(Coor_Info *Coor) {
    free(Coor->coor);
    if (show_tag)
        printf("Coor cleared.\n");
}

void clear_mesh(Node_Mesh *Mesh) {
    for (int i = 0; i < Mesh->typeN; i++) {
        free(Mesh->topo[i]);
    }
    free(Mesh->topo );
    free(Mesh->scale);
    free(Mesh->nodeN);
    free(Mesh->l_dim);
    free(Mesh->type );
    if (show_tag)
        printf("Mesh cleared.\n");
}

void clear_ID(Dof_Tag *ID) {
    if  (ID->nodeN == 0)
        return;
    free(ID->nodeSN);
    free(ID->dofID );
    free(ID->dofval);
    if (show_tag&&showFtag)
        printf("ID cleared.\n");
}

void clear_init(Init_Data *Init) {
    if (Init->order == 0 || Init->nodeN == 0) { return; }
    if (Init->order >  0) { free(Init->init_0); }
    if (Init->order >  1) { free(Init->init_1); }
    if (Init->order >  2) { free(Init->init_2); }
    if (show_tag&&showFtag)
        printf("Initial data cleared.\n");
}

void clear_E_ID(Elem_Tag *E_ID) {
    if (E_ID->typeN == 0)
        return;
    for (int i = 0; i < E_ID->typeN; i++) {
        if (E_ID->elemN[E_ID->typeN-1] == 0)
            continue;
        free(E_ID->elemID[i]);
        free(E_ID->elemSN[i]);
    }
    free(E_ID->elemID);
    free(E_ID->elemSN);
    free(E_ID->elemN );
    free(E_ID->type  );
    if (show_tag&&showFtag)
        printf("Element material tag cleared.\n");
}

void clear_Emate(Mesh_Mate *Emate) {
    if (Emate->typeN == 0)
        return;
    for (int i = 0; i < Emate->typeN; i++) {
        if (Emate->scale == 0)
            continue;
        free(Emate->elemID[i]);
    }
    free(Emate->elemID);
    free(Emate->scale );
    if (show_tag&&showFtag)
        printf("Mesh material tag cleared.\n");
}

void clear_mate(Materail *Mate) {
    free(Mate->mate);
    if (show_tag&&showFtag)
        printf("Materail cleared.\n");
}

void clear_result(Result *Res) {
    free(Res->result);
    if (show_tag&&showFtag)
        printf("Result cleared.\n");
}

void clear_field(Field_info *Field, int Field_num) {
    for (int i = 0; i < Field_num; i++) {
        clear_ID     (&(Field[i].ID   )); if (show_tag) printf("Field %d ID     cleared.\n",  i+1);
        clear_init   (&(Field[i].Init )); if (show_tag) printf("Field %d Init   cleared.\n",  i+1);
        clear_E_ID   (&(Field[i].E_ID )); if (show_tag) printf("Field %d E_ID   cleared.\n",  i+1);
        clear_Emate  (&(Field[i].Emate)); if (show_tag) printf("Field %d Emate  cleared.\n",  i+1);
        clear_mate   (&(Field[i].Mate )); if (show_tag) printf("Field %d Mate   cleared.\n",  i+1);
        clear_result (&(Field[i].Res  )); if (show_tag) printf("Field %d result cleared.\n",  i+1);
    }
}