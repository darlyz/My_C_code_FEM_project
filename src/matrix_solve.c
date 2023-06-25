/*
 Copyright: Copyright (c) 2019
 Created: 2019-4-19
 Author: Zhang_Licheng
 All rights reserved
*/
#include "fem.h"
#include "az_aztec.h"

void matrsolv(Equat_Set* Equa_ptr) {

    //AZ_set_proc_config(AZ_set_proc_config, AZ_NOT_MPI);

    int nZeroN    = Equa_ptr->nZeroN;
    int equaN     = Equa_ptr->equaN;
    int *row_nZN  = Equa_ptr->row_nZN;
    int **clm_idx = Equa_ptr->clm_idx;
    double **matrix = Equa_ptr->matrix;

    int    *bindx = (int   *)calloc(nZeroN+1, sizeof(int));
    double *val   = (double*)calloc(nZeroN+1, sizeof(double));

    // convert matrix to MSR format
    int nontrivial_count = 0, temp_count = 0;
    for (int eq_i=0; eq_i<equaN; eq_i++) {

        if (eq_i==0)
            bindx[0] = equaN + 1;

        else {
            nontrivial_count += row_nZN[eq_i-1] - 1;
            bindx[eq_i] = nontrivial_count + bindx[0];
        }

        for (int clm_i=0; clm_i<row_nZN[eq_i]; clm_i++) {

            if (clm_idx[eq_i][clm_i] == eq_i + 1) {

                val[eq_i] = matrix[eq_i][clm_i];
                continue;
            }

            temp_count ++;

            bindx[equaN + temp_count] = clm_idx[eq_i][clm_i] - 1;
            val  [equaN + temp_count] = matrix [eq_i][clm_i];
        }
    }
    nontrivial_count += row_nZN[equaN-1] - 1;
    bindx[equaN] =  nontrivial_count + bindx[0];

    //for (int i = 0; i <nZeroN+1; i++) {
    //    printf ("%d %le\n",bindx[i],val[i]);
    //}

    // 
    int N_update = equaN;
    int *update  = (int*)malloc(N_update * sizeof(int));
    for (int i = 0; i < N_update; i++)
        update [i] = i;

    //double *b=(double *) calloc(N_update,sizeof(double));
    double *x=(double *) calloc(N_update,sizeof(double));

    //memcpy(b, Equa_ptr->vector, N_update*sizeof(double));
    double *b = Equa_ptr->vector;

    int  noptions[AZ_OPTIONS_SIZE];
    double params[AZ_PARAMS_SIZE];

    AZ_defaults(noptions, params);
    //choose_solver_options(noptions, params);

    params  [AZ_tol      ] = 1e-8;
    //params  [Az_dropout  ] = 0.  ;
    params  [AZ_omega    ] = 1.  ;
    noptions[AZ_max_iter ] = 3000;
    noptions[AZ_solver   ] = AZ_bicgstab ;
    noptions[AZ_precond  ] = AZ_ls ;
    noptions[AZ_poly_ord ] = 7 ;
    noptions[AZ_scaling  ] = AZ_none ;
    noptions[AZ_output   ] = 1 ;

    //----------------------------------------------------------------

    int    nproc_config[AZ_PROC_SIZE];
    double status[AZ_STATUS_SIZE];

    int *ndata_org;
    int *nexternal;
    int *nupdate_index;
    int *nextern_index;

    AZ_set_proc_config(nproc_config, AZ_NOT_MPI);

    int MSRorVBR = AZ_MSR_MATRIX;

    AZ_transform(nproc_config,&nexternal,bindx,val,update,
                 &nupdate_index,&nextern_index,&ndata_org,
                 N_update, NULL, NULL, NULL, NULL,MSRorVBR);
    AZ_reorder_vec(b,ndata_org,nupdate_index,NULL);


    AZ_solve(x,b, noptions, params,NULL,bindx,NULL,NULL,
             NULL,val, ndata_org, status, nproc_config);

    AZ_invorder_vec(x,ndata_org,nupdate_index,0,b);

    memcpy(Equa_ptr->vector, x, N_update*sizeof(double));

    if (!AZ_FALSE)
    {
        AZ_free(nupdate_index);
        AZ_free(nextern_index);
        AZ_free(nexternal);
        AZ_free(ndata_org);
    }

    free(bindx );
    free(val   );
    free(update);
    free(x     );

    printf("solv done!\n");
}