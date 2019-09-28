/*
 Copyright: Copyright (c) 2018
 Created: 2018-4-19
 Author: Zhang_Licheng
 Title: complex tensor expression in dummy index summation
 All rights reserved
*/
#include "fem.h"

#define show_tag 0

int  assign_mesh_type();
void assign_local_dim();

void readmesh(
    Coor_Info   *Coor,
    Node_Mesh   *Mesh,
    Field_info  *Field,
    int         *field_SN,
    char        *dat_file
){
    char   temp_str[255];
    //char   temp_char;
    //int    temp_int;
    //double temp_double;

    FILE *ReadData;
    if((ReadData=fopen(dat_file,"r"))==NULL) {
        printf("Read mesh failed!\n");
        return;
    }

    typedef enum{coor = 1, mesh, field, id, value, init, mate} paragh;
    paragh dataparagh;
    int line_count = 0;

    Mesh->typeN = 0;
    Mesh->type  = (Mesh_Type*)malloc(sizeof(Mesh_Type)*maxsize);
    Mesh->l_dim = (int* )malloc(sizeof(int )*maxsize);
    Mesh->nodeN = (int* )malloc(sizeof(int )*maxsize);
    Mesh->scale = (int* )malloc(sizeof(int )*maxsize);
    Mesh->topo  = (int**)malloc(sizeof(int*)*maxsize);

    Dof_Tag   *ID;
    Init_Data *Init;
    Elem_Tag  *E_ID;

    while(fgets(temp_str, 255, ReadData) != NULL) {

        line_count ++;

        if (strncmp(temp_str,"-coordinate-" ,4) == 0) {
            dataparagh = coor;
            line_count = 0;
            continue;
        }

        else if (strncmp(temp_str,"-mesh-" ,4) == 0) {
            dataparagh = mesh;
            line_count = 0;
            Mesh->typeN ++ ;
            continue;
        }

        else if (strncmp(temp_str,"-field-", 4) == 0) {
            dataparagh = field;
            (*field_SN) ++;
            ID   = &(Field[*field_SN-1].ID);
            Init = &(Field[*field_SN-1].Init);
            E_ID = &(Field[*field_SN-1].E_ID);

            Init->order  = 0;
            E_ID->typeN  = 0;
            E_ID->type   = (Mesh_Type*)malloc(sizeof(Mesh_Type)*maxsize);
            E_ID->elemN  = (int* )malloc(sizeof(int )*maxsize);
            E_ID->elemID = (int**)malloc(sizeof(int*)*maxsize);
            E_ID->elemSN = (int**)malloc(sizeof(int*)*maxsize);
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
            Init->order ++ ;
            continue;
        }

        else if (strncmp(temp_str, "-mate-", 4) == 0) {
            dataparagh = mate;
            line_count = 0;
            E_ID->typeN ++ ;
            continue;
        }

        if (dataparagh == coor) {

            if (line_count == 1) {

                sscanf(temp_str, "%d %d", &(Coor->nodeN), &(Coor->dim));

                Coor->coor = (double*)malloc(Coor->nodeN
                                            *Coor->dim
                                            *sizeof(double));
            }
            else if (line_count < Coor->nodeN + 2) {

                int  word_count = 0;
                int  word_begin = 0;
                bool word_find  = true;

                for (int i=0; temp_str[i] != '\0'; i++) {

                    if ( isspace(temp_str[i]) && word_find ) {

                        word_find = false;
                        temp_str[i] = '\0';

                        sscanf(temp_str + word_begin, "%lg",
                             &(Coor->coor[(line_count-2)*Coor->dim + word_count]));

                        word_count ++;
                    }

                    else if ( !isspace(temp_str[i]) && !word_find ) {
                        word_find  = true;
                        word_begin = i;
                    }
                }
            }

            if (show_tag && (line_count == Coor->nodeN + 2) ) printf("Read coor end.\n");
        }

        else if (dataparagh == mesh) {

            if (line_count == 1) {
                if ( assign_mesh_type(temp_str, Mesh->type, Mesh->typeN-1) )
                    return;
                assign_local_dim(Mesh->type[Mesh->typeN-1], &(Mesh->l_dim[Mesh->typeN-1]));
            }

            else if (line_count == 2) {

                sscanf(temp_str, "%d %d", &(Mesh->scale[Mesh->typeN-1]),
                                          &(Mesh->nodeN[Mesh->typeN-1]));

                Mesh->topo[Mesh->typeN-1]  = (int*)malloc(sizeof(int)*
                                           Mesh->scale[Mesh->typeN-1]*
                                           Mesh->nodeN[Mesh->typeN-1]);
            }

            else if (line_count < Mesh->scale[Mesh->typeN-1] + 3) {

                int  word_count = 0;
                int  word_begin = 0;
                bool word_find  = true;

                for (int i=0; temp_str[i] != '\0'; i++) {

                    if ( isspace(temp_str[i]) && word_find ) {

                        word_find = false;
                        temp_str[i] = '\0';

                        sscanf(temp_str + word_begin, "%d",
                             &(Mesh->topo[Mesh->typeN-1][(line_count-3)*Mesh->nodeN[Mesh->typeN-1] + word_count]) );

                        word_count ++;
                    }

                    else if ( !isspace(temp_str[i]) && !word_find ) {
                        word_find  = true;
                        word_begin = i;
                    }
                }
            }

            if (show_tag && (line_count == Mesh->scale[Mesh->typeN-1] + 3) ) printf("Read mesh end.\n");
        }

        else if (dataparagh == id) {

            if (line_count == 1) {

                sscanf(temp_str, "%d %d", &(ID->nodeN), &(ID->dofN));

                ID->nodeSN = (int*   )malloc(ID->nodeN * sizeof(int));
                ID->dofID  = (int*   )malloc(ID->nodeN * ID->dofN * sizeof(int));
                ID->dofval = (double*)malloc(ID->nodeN * ID->dofN * sizeof(double));
            }
            else if (line_count < ID->nodeN + 2) {

                int  word_count = 0;
                int  word_begin = 0;
                bool word_find  = true;

                for (int i=0; temp_str[i] != '\0'; i++) {

                    if ( isspace(temp_str[i]) && word_find ) {

                        word_find = false;
                        temp_str[i] = '\0';

                        if (word_count == 0)
                            sscanf(temp_str + word_begin, "%d", &(ID->nodeSN[line_count-2]));
                        
                        else
                            sscanf(temp_str + word_begin, "%d",
                                 &(ID->dofID[(line_count-2)*ID->dofN + word_count-1]));
                        
                        word_count ++;
                    }

                    else if ( !isspace(temp_str[i]) && !word_find ) {
                        word_find  = true;
                        word_begin = i;
                    }
                }
            }
        }

        else if (dataparagh == value) {

            if (line_count == 1) {
                continue;
            }

            else if (line_count < ID->nodeN + 2) {

                int  word_count = 0;
                int  word_begin = 0;
                bool word_find  = true;

                for (int i=0; temp_str[i] != '\0'; i++) {

                    if ( isspace(temp_str[i]) && word_find ) {

                        word_find = false;
                        temp_str[i] = '\0';

                        if (word_count != 0)
                            sscanf(temp_str + word_begin, "%lg",
                                 &(ID->dofval[(line_count-2)*ID->dofN + word_count-1]));
                        
                        word_count ++;
                    }

                    else if ( !isspace(temp_str[i]) && !word_find ) {
                        word_find  = true;
                        word_begin = i;
                    }
                }
            }

            if (show_tag && (line_count == ID->nodeN + 2) ) printf("Read constraint end.\n");
        }

        else if (dataparagh == init) {

            if (line_count == 1) {

                switch(Init->order) {
                    case 1:
                        sscanf(temp_str, "%d %d", &Init->nodeN, &Init->dofN);
                        Init->init_0 = (double*)malloc(Init->nodeN*Init->dofN*sizeof(double));
                        break;
                    case 2:
                        Init->init_1 = (double*)malloc(Init->nodeN*Init->dofN*sizeof(double));
                        break;
                    case 3:
                        Init->init_2 = (double*)malloc(Init->nodeN*Init->dofN*sizeof(double));
                        break;
                }

            }

            else if (line_count < Init->nodeN + 2) {

                int  word_count = 0;
                int  word_begin = 0;
                bool word_find  = true;

                for (int i=0; temp_str[i] != '\0'; i++) {

                    if ( isspace(temp_str[i]) && word_find ) {

                        word_find = false;
                        temp_str[i] = '\0';

                        if (word_count != 0) {

                            switch(Init->order) {
                                case 0:
                                    sscanf(temp_str + word_begin, "%lg",
                                         &(Init->init_0[(line_count-2)*Init->dofN + word_count-1]));
                                    break;
                                case 1:
                                    sscanf(temp_str + word_begin, "%lg",
                                         &(Init->init_1[(line_count-2)*Init->dofN + word_count-1]));
                                    break;
                                case 2:
                                    sscanf(temp_str + word_begin, "%lg",
                                         &(Init->init_2[(line_count-2)*Init->dofN + word_count-1]));
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

            if (show_tag && (line_count == Init->nodeN + 2) ) printf("Read order %d initial data end.\n", Init->order);
        }

        else if (dataparagh == mate) {

            if (line_count == 1) {
                if ( assign_mesh_type(temp_str, E_ID->type, E_ID->typeN-1) == 1)
                    return;
            }

            else if (line_count == 2) {

                sscanf(temp_str, "%d", &(E_ID->elemN[E_ID->typeN-1]));

                E_ID->elemID[E_ID->typeN-1] = (int*)malloc(sizeof(int)*E_ID->elemN[E_ID->typeN-1]);
                E_ID->elemSN[E_ID->typeN-1] = (int*)malloc(sizeof(int)*E_ID->elemN[E_ID->typeN-1]);
            }

            else if (line_count < E_ID->elemN[E_ID->typeN-1] + 3) {
                sscanf(temp_str, "%d %d", &E_ID->elemSN[E_ID->typeN-1][line_count-3] ,
                                          &E_ID->elemID[E_ID->typeN-1][line_count-3]);
            }

            if (show_tag && (line_count == E_ID->elemN[E_ID->typeN-1] + 3) ) printf("Read mesh material tag end.\n");
        }
    }
    fclose(ReadData);
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