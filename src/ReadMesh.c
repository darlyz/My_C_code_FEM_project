/*
 Copyright: Copyright (c) 2018
 Created: 2018-4-19
 Author: Zhang_Licheng
 Title: complex tensor expression in dummy index summation
 All rights reserved
*/
#include "fem.h"

#define show_tag 0

int  assign_mesh_type(char*, Mesh_Type*, int);
void assign_local_dim(Mesh_Type, int*);

void readmesh(
    Coor_Info   *coor_ptr,
    Node_Mesh   *mesh_ptr,
    Field_info  *fields,
    int         *field_SN_ptr,
    char        *dat_file
){
    char   temp_str[255];
    //char   temp_char;
    //int    temp_int;
    //double temp_double;

    FILE *dat_file_rdptr;
    if((dat_file_rdptr=fopen(dat_file,"r"))==NULL) {
        printf("Read mesh failed!\n");
        return;
    }

    typedef enum{coor = 1, mesh, field, id, value, init, mate} paragh_tag;
    paragh_tag dataparagh;
    int line_count = 0;

    mesh_ptr->typeN = 0;
    mesh_ptr->type  = (Mesh_Type*)malloc(sizeof(Mesh_Type)*maxsize);
    mesh_ptr->l_dim = (int* )malloc(sizeof(int )*maxsize);
    mesh_ptr->nodeN = (int* )malloc(sizeof(int )*maxsize);
    mesh_ptr->scale = (int* )malloc(sizeof(int )*maxsize);
    mesh_ptr->topo  = (int**)malloc(sizeof(int*)*maxsize);

    Dof_Tag   *ID_ptr;
    Init_Data *Init_ptr;
    Elem_Tag  *E_ID_ptr;

    while(fgets(temp_str, 255, dat_file_rdptr) != NULL) {

        line_count ++;

        if (strncmp(temp_str,"-coordinate-" ,4) == 0) {
            dataparagh = coor;
            line_count = 0;
            continue;
        }

        else if (strncmp(temp_str,"-mesh-" ,4) == 0) {
            dataparagh = mesh;
            line_count = 0;
            mesh_ptr->typeN ++ ;
            continue;
        }

        else if (strncmp(temp_str,"-field-", 4) == 0) {
            dataparagh = field;
            (*field_SN_ptr) ++;
            ID_ptr   = &(fields[*field_SN_ptr-1].ID);
            Init_ptr = &(fields[*field_SN_ptr-1].Init);
            E_ID_ptr = &(fields[*field_SN_ptr-1].E_ID);

            Init_ptr->order  = 0;
            E_ID_ptr->typeN  = 0;
            E_ID_ptr->type   = (Mesh_Type*)malloc(sizeof(Mesh_Type)*maxsize);
            E_ID_ptr->elemN  = (int* )malloc(sizeof(int )*maxsize);
            E_ID_ptr->elemID = (int**)malloc(sizeof(int*)*maxsize);
            E_ID_ptr->elemSN = (int**)malloc(sizeof(int*)*maxsize);
        }

        else if (strncmp(temp_str,"-ID-"   ,4) == 0) {
            dataparagh = id;
            line_count = 0;
            continue;
        }

        else if (strncmp(temp_str,"-value-",4) == 0) {
            dataparagh = value;
            line_count = 0;
            continue;
        }

        else if (strncmp(temp_str,"-init-" ,4) == 0) {
            dataparagh = init;
            line_count = 0;
            Init_ptr->order ++ ;
            continue;
        }

        else if (strncmp(temp_str, "-mate-", 4) == 0) {
            dataparagh = mate;
            line_count = 0;
            E_ID_ptr->typeN ++ ;
            continue;
        }

        switch (dataparagh)
        {
            case coor: {

                if (line_count == 1) {

                    sscanf(temp_str, "%d %d", &(coor_ptr->nodeN), &(coor_ptr->dim));

                    coor_ptr->coor = (double*)malloc(coor_ptr->nodeN
                                                *coor_ptr->dim
                                                *sizeof(double));
                }
                else if (line_count < coor_ptr->nodeN + 2) {

                    int  word_count = 0;
                    int  word_begin = 0;
                    bool word_find  = true;

                    for (int i=0; temp_str[i] != '\0'; i++) {

                        if ( isspace(temp_str[i]) && word_find ) {

                            word_find = false;
                            temp_str[i] = '\0';

                            sscanf(temp_str + word_begin, "%lg",
                                 &(coor_ptr->coor[(line_count-2)*coor_ptr->dim + word_count]));

                            word_count ++;
                        }

                        else if ( !isspace(temp_str[i]) && !word_find ) {
                            word_find  = true;
                            word_begin = i;
                        }
                    }
                }

                if (show_tag && (line_count == coor_ptr->nodeN + 2) ) printf("Read coor end.\n");
            } break;

            case mesh: {

                if (line_count == 1) {
                    if ( assign_mesh_type(temp_str, mesh_ptr->type, mesh_ptr->typeN-1) )
                        return;
                    assign_local_dim(mesh_ptr->type[mesh_ptr->typeN-1], &(mesh_ptr->l_dim[mesh_ptr->typeN-1]));
                }

                else if (line_count == 2) {

                    sscanf(temp_str, "%d %d", &(mesh_ptr->scale[mesh_ptr->typeN-1]),
                                              &(mesh_ptr->nodeN[mesh_ptr->typeN-1]));

                    mesh_ptr->topo[mesh_ptr->typeN-1]  = (int*)malloc(sizeof(int)*
                                               mesh_ptr->scale[mesh_ptr->typeN-1]*
                                               mesh_ptr->nodeN[mesh_ptr->typeN-1]);
                }

                else if (line_count < mesh_ptr->scale[mesh_ptr->typeN-1] + 3) {

                    int  word_count = 0;
                    int  word_begin = 0;
                    bool word_find  = true;

                    for (int i=0; temp_str[i] != '\0'; i++) {

                        if ( isspace(temp_str[i]) && word_find ) {

                            word_find = false;
                            temp_str[i] = '\0';

                            sscanf(temp_str + word_begin, "%d",
                                 &(mesh_ptr->topo[mesh_ptr->typeN-1][(line_count-3)*mesh_ptr->nodeN[mesh_ptr->typeN-1] + word_count]) );

                            word_count ++;
                        }

                        else if ( !isspace(temp_str[i]) && !word_find ) {
                            word_find  = true;
                            word_begin = i;
                        }
                    }
                }

                if (show_tag && (line_count == mesh_ptr->scale[mesh_ptr->typeN-1] + 3) ) printf("Read mesh end.\n");
            } break;

            case id: {

                if (line_count == 1) {

                    sscanf(temp_str, "%d %d", &(ID_ptr->nodeN), &(ID_ptr->dofN));

                    ID_ptr->nodeSN = (int*   )malloc(ID_ptr->nodeN * sizeof(int));
                    ID_ptr->dofID  = (int*   )malloc(ID_ptr->nodeN * ID_ptr->dofN * sizeof(int));
                    ID_ptr->dofval = (double*)malloc(ID_ptr->nodeN * ID_ptr->dofN * sizeof(double));
                }
                else if (line_count < ID_ptr->nodeN + 2) {

                    int  word_count = 0;
                    int  word_begin = 0;
                    bool word_find  = true;

                    for (int i=0; temp_str[i] != '\0'; i++) {

                        if ( isspace(temp_str[i]) && word_find ) {

                            word_find = false;
                            temp_str[i] = '\0';

                            if (word_count == 0)
                                sscanf(temp_str + word_begin, "%d", &(ID_ptr->nodeSN[line_count-2]));

                            else
                                sscanf(temp_str + word_begin, "%d",
                                     &(ID_ptr->dofID[(line_count-2)*ID_ptr->dofN + word_count-1]));

                            word_count ++;
                        }

                        else if ( !isspace(temp_str[i]) && !word_find ) {
                            word_find  = true;
                            word_begin = i;
                        }
                    }
                }
            } break;

            case value: {

                if (line_count == 1) {
                    continue;
                }

                else if (line_count < ID_ptr->nodeN + 2) {

                    int  word_count = 0;
                    int  word_begin = 0;
                    bool word_find  = true;

                    for (int i=0; temp_str[i] != '\0'; i++) {

                        if ( isspace(temp_str[i]) && word_find ) {

                            word_find = false;
                            temp_str[i] = '\0';

                            if (word_count != 0)
                                sscanf(temp_str + word_begin, "%lg",
                                     &(ID_ptr->dofval[(line_count-2)*ID_ptr->dofN + word_count-1]));

                            word_count ++;
                        }

                        else if ( !isspace(temp_str[i]) && !word_find ) {
                            word_find  = true;
                            word_begin = i;
                        }
                    }
                }

                if (show_tag && (line_count == ID_ptr->nodeN + 2) ) printf("Read constraint end.\n");
            } break;

            case init: {

                if (line_count == 1) {

                    switch(Init_ptr->order) {
                        case 1:
                            sscanf(temp_str, "%d %d", &Init_ptr->nodeN, &Init_ptr->dofN);
                            Init_ptr->init_0 = (double*)malloc(Init_ptr->nodeN*Init_ptr->dofN*sizeof(double));
                            break;
                        case 2:
                            Init_ptr->init_1 = (double*)malloc(Init_ptr->nodeN*Init_ptr->dofN*sizeof(double));
                            break;
                        case 3:
                            Init_ptr->init_2 = (double*)malloc(Init_ptr->nodeN*Init_ptr->dofN*sizeof(double));
                            break;
                    }

                }

                else if (line_count < Init_ptr->nodeN + 2) {

                    int  word_count = 0;
                    int  word_begin = 0;
                    bool word_find  = true;

                    for (int i=0; temp_str[i] != '\0'; i++) {

                        if ( isspace(temp_str[i]) && word_find ) {

                            word_find = false;
                            temp_str[i] = '\0';

                            if (word_count != 0) {

                                switch(Init_ptr->order) {
                                    case 0:
                                        sscanf(temp_str + word_begin, "%lg",
                                             &(Init_ptr->init_0[(line_count-2)*Init_ptr->dofN + word_count-1]));
                                        break;
                                    case 1:
                                        sscanf(temp_str + word_begin, "%lg",
                                             &(Init_ptr->init_1[(line_count-2)*Init_ptr->dofN + word_count-1]));
                                        break;
                                    case 2:
                                        sscanf(temp_str + word_begin, "%lg",
                                             &(Init_ptr->init_2[(line_count-2)*Init_ptr->dofN + word_count-1]));
                                        break;
                                }
                            }

                            word_count ++;
                        }

                        else if ( !isspace(temp_str[i]) && !word_find ) {
                            word_find  = true;
                            word_begin = i;
                        }
                    }
                }

                if (show_tag && (line_count == Init_ptr->nodeN + 2) ) printf("Read order %d initial data end.\n", Init_ptr->order);
            } break;

            case mate: {

                if (line_count == 1) {
                    if ( assign_mesh_type(temp_str, E_ID_ptr->type, E_ID_ptr->typeN-1) == 1)
                        return;
                }

                else if (line_count == 2) {

                    sscanf(temp_str, "%d", &(E_ID_ptr->elemN[E_ID_ptr->typeN-1]));

                    E_ID_ptr->elemID[E_ID_ptr->typeN-1] = (int*)malloc(sizeof(int)*E_ID_ptr->elemN[E_ID_ptr->typeN-1]);
                    E_ID_ptr->elemSN[E_ID_ptr->typeN-1] = (int*)malloc(sizeof(int)*E_ID_ptr->elemN[E_ID_ptr->typeN-1]);
                }

                else if (line_count < E_ID_ptr->elemN[E_ID_ptr->typeN-1] + 3) {
                    sscanf(temp_str, "%d %d", &E_ID_ptr->elemSN[E_ID_ptr->typeN-1][line_count-3] ,
                                              &E_ID_ptr->elemID[E_ID_ptr->typeN-1][line_count-3]);
                }

                if (show_tag && (line_count == E_ID_ptr->elemN[E_ID_ptr->typeN-1] + 3) ) printf("Read mesh material tag end.\n");
            } break;

            default:
                break;
        }
    }
    fclose(dat_file_rdptr);
    printf("Read mesh done!\n");
}

int assign_mesh_type(char* type_str, Mesh_Type *Mtype, int type_i) {

    /*
    if (strncmp(type_str, "l2", 2) == 0)
        Mtype[type_i] = L2;

    else if (strncmp(type_str, "l3", 2) == 0)
        Mtype[type_i] = L3;

    else if (strncmp(type_str, "t3", 2) == 0)
        Mtype[type_i] = T3;

    else if (strncmp(type_str, "t6", 2) == 0)
        Mtype[type_i] = T6;

    else if (strncmp(type_str, "q4", 2) == 0)
        Mtype[type_i] = Q4;

    else if (strncmp(type_str, "q8", 2) == 0)
        Mtype[type_i] = Q8;

    else if (strncmp(type_str, "q9", 2) == 0)
        Mtype[type_i] = Q9;

    else if (strncmp(type_str, "w4", 2) == 0)
        Mtype[type_i] = W4;

    else if (strncmp(type_str, "w10", 3) == 0)
        Mtype[type_i] = W10;

    else if (strncmp(type_str, "c8", 2) == 0)
        Mtype[type_i] = C8;

    else if (strncmp(type_str, "c20", 3) == 0)
        Mtype[type_i] = C20;

    else if (strncmp(type_str, "c27", 3) == 0)
        Mtype[type_i] = C27;

    else if (strncmp(type_str, "h6", 3) == 0)
        Mtype[type_i] = H6;

    else if (strncmp(type_str, "h15", 3) == 0)
        Mtype[type_i] = H15;

    else if (strncmp(type_str, "h18", 3) == 0)
        Mtype[type_i] = H18;
    */

    char shap_type = type_str[0];
    int  shap_nodn;
    sscanf(type_str + 1, "%d", &shap_nodn);

    switch (shap_type) {
        case 'l':
            switch (shap_nodn) {
                case 2:
                    Mtype[type_i] = L2;
                    break;
                case 3:
                    Mtype[type_i] = L3;
                    break;
                default:
                    printf("Unknown shap type: %s!\n",type_str);
                    return 1;
            }
            break;
        case 't':
            switch (shap_nodn) {
                case 3:
                    Mtype[type_i] = T3;
                    break;
                case 6:
                    Mtype[type_i] = T6;
                    break;
                default:
                    printf("Unknown shap type: %s!\n",type_str);
                    return 1;
            }
            break;
        case 'q':
            switch (shap_nodn) {
                case 4:
                    Mtype[type_i] = Q4;
                    break;
                case 8:
                    Mtype[type_i] = Q8;
                    break;
                case 9:
                    Mtype[type_i] = Q9;
                    break;
                default:
                    printf("Unknown shap type: %s!\n",type_str);
                    return 1;
            }
            break;
        case 'w':
            switch (shap_nodn) {
                case 4:
                    Mtype[type_i] = W4;
                    break;
                case 10:
                    Mtype[type_i] = W10;
                    break;
                default:
                    printf("Unknown shap type: %s!\n",type_str);
                    return 1;
            }
            break;
        case 'c':
            switch (shap_nodn) {
                case 8:
                    Mtype[type_i] = C8;
                    break;
                case 20:
                    Mtype[type_i] = C20;
                    break;
                case 27:
                    Mtype[type_i] = C27;
                    break;
                default:
                    printf("Unknown shap type: %s!\n",type_str);
                    return 1;
            }
            break;
        case 'h':
            switch (shap_nodn) {
                case 6:
                    Mtype[type_i] = H6;
                    break;
                case 15:
                    Mtype[type_i] = H15;
                    break;
                case 18:
                    Mtype[type_i] = H18;
                    break;
                default:
                    printf("Unknown shap type: %s!\n",type_str);
                    return 1;
            }
            break;
        case 'p':
            switch (shap_nodn) {
                case 1:
                    Mtype[type_i] = P1;
                    break;
                case 5:
                    Mtype[type_i] = P5;
                    break;
                case 13:
                    Mtype[type_i] = P13;
                    break;
                case 14:
                    Mtype[type_i] = P14;
                    break;
                default:
                    printf("Unknown shap type: %s!\n",type_str);
                    return 1;
            }
            break;
        default:
            printf("Unknown shap type: %s!\n",type_str);
            return 1;
    }

    return 0;
}

void assign_local_dim(Mesh_Type type, int *l_dim) {
    switch (type) {
        case P1 : *l_dim = 0; break;
        case L2 : *l_dim = 1; break;
        case L3 : *l_dim = 1; break;
        case T3 : *l_dim = 2; break;
        case T6 : *l_dim = 2; break;
        case Q4 : *l_dim = 2; break;
        case Q8 : *l_dim = 2; break;
        case Q9 : *l_dim = 2; break;
        case W4 : *l_dim = 3; break;
        case W10: *l_dim = 3; break;
        case C8 : *l_dim = 3; break;
        case C20: *l_dim = 3; break;
        case C27: *l_dim = 3; break;
        case H6 : *l_dim = 3; break;
        case H15: *l_dim = 3; break;
        case H18: *l_dim = 3; break;
        case P5 : *l_dim = 3; break;
        case P13: *l_dim = 3; break;
        case P14: *l_dim = 3; break;
        default:  *l_dim = 0; break;
    }
}