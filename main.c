// Parallel Programming Homework 1 / Serial Graph Triangle Counting

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mmio.h"

int main(int argc, char *argv[])
{


    int ret_code;
    MM_typecode matcode;
    FILE *f;
    uint32_t M, N, nz;
    uint32_t i, *I, *J, *Val, *RowPtr,*nVal, *C;
//    double *val;



    if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
		exit(1);
	}
    else
    {
        if ((f = fopen(argv[1], "r")) == NULL)
            exit(1);
    }

    if (mm_read_banner(f, &matcode) != 0)
    {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }


    /*  This is how one can screen matrix types if their application */
    /*  only supports a subset of the Matrix Market data types.      */

    if (mm_is_complex(matcode) && mm_is_matrix(matcode) &&
            mm_is_sparse(matcode) )
    {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    /* find out size of sparse matrix .... */

    if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) !=0)
        exit(1);


    /* reseve memory for matrices */

    I = (int *) malloc(nz * sizeof(int)); // I[nz] = Col[nz]
    J = (int *) malloc(nz * sizeof(int)); // J[nz] = Row[nz]
    Val = (int *) malloc(nz * sizeof(int)); //Val[nz] = Val[nz]
    nVal = (int *) malloc(nz * sizeof(int)); //Val[nz] = Val[nz]
    RowPtr = (int *) malloc( (M+1) * sizeof(int));
    C = (int *) malloc( (M) * sizeof(int));


    /* NOTE: when reading in doubles, ANSI C requires the use of the "l"  */
    /*   specifier as in "%lg", "%lf", "%le", otherwise errors will occur */
    /*  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            */

    for (i=0; i<nz; i++)
    {
        Val[i] = 1;
        fscanf(f, "%d %d",&I[i], &J[i]);
        I[i]--;  /* adjust from 1-based to 0-based */
        J[i]--;
        nVal[i] = 0;
        if (i< M)
        {
            RowPtr[i] = 0;
            C[i] = 0;
        }
    }

    if (f !=stdin) fclose(f);

    /************************/
    /* now write out matrix */
    /************************/

    mm_write_banner(stdout, matcode);
    mm_write_mtx_crd_size(stdout, M, N, nz);

/* Testing the Col[nnz]and Row[nnz] values */

//    for (i=0; i<nz; i++)
//        fprintf(stdout, "%d %d\n ", J[i]+1,I[i]+1);


/* Converting the J[nz] aka Row[nz] , to RowPtr[M] */

    for (int i = 0; i < nz; i++)
    {
        RowPtr[J[i] + 1] = RowPtr[J[i] + 1] + 1;
    }
    for (int i = 0; i < M; i++)
    {
        RowPtr[i + 1] += RowPtr[i];
//        printf("%d ",RowPtr[i]);
    }







/* HERE STARTS MY PROGRAM NOT THE PREPROCESSING*/

    for (uint32_t k=0; k < (M-1); k++) {
        for (uint32_t p=RowPtr[k]; p<RowPtr[k+1];p++)
            {
                uint32_t sum = 0;
                for (uint32_t j=RowPtr[k]; j<RowPtr[k+1];j++) {

                    for (uint32_t i=RowPtr[I[p]]; i<RowPtr[I[p]+1];i++) {
                        if (I[j] == I[i]) {
                            sum = sum + 1;
                        }

                    }
                }
                nVal[p] = sum;
                C[k] = C[k] + nVal[p];

            }
    }

    uint32_t all = 0;
     for (uint32_t k=0; k < (M-1); k++) {
       if (C[k] !=0)
       {
          C[k] = C[k] / 2;
          all += C[k];
       }

    }
    printf("\n%d \n",all);


    return 0;
}
