#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>
#include <stdbool.h>
#include <omp.h>

bool primo (int n) {
        int i;

        for (i = 3; i < (int)(sqrt(n) + 1); i+=2)
             if (n%i == 0)
                 return false;
        return true;
}


int main(int argc, char *argv[]) {
	int cont = 0, total = 0;
	long int i, n;
	int inicio, salto;
	int meu_ranque, num_procs;
	if (argc < 2) {
        	printf("Valor inválido! Entre com um valor do maior inteiro\n");
       	 	return 0;
    } else {
        n = strtol(argv[1], (char **) NULL, 10);
    }

   // inicio = 3;
   // salto = 1;
   // inicio = 3 + meu_ranque*2;
   // salto = num_procs*2;	

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    inicio = 3 + meu_ranque;
    salto = num_procs;

	#pragma omp parallel for private(i) schedule(dynamic) reduction(+ : cont)
	for (i = inicio; i <= n; i += salto) 
	{	
		if(primo(i)) cont++;
	}

    MPI_Reduce(&cont, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		
	//total = cont;

    if (meu_ranque == 0){	
    total += 1;    /* Acrescenta o dois, que também é primo */
	printf("Quant. de primos entre 1 e n: %d \n", total);

    }
    MPI_Finalize();
	return(0);
}

