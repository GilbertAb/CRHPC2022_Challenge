#include <mpi.h>

void matrix_mult(double *A, double *B, double *C, int N, int argc, char **argv){

	int thread_status;
	int meu_ranque, num_procs; 
	int TAM_VET = N;
	double *CFinal;
	double *vet_envia, vet_recebe[TAM_VET];
	MPI_Init_thread(&argc, &argv,MPI_THREAD_MULTIPLE,&thread_status);
	MPI_Comm_rank(MPI_COMM_WORLD,&meu_ranque);
   	MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
	

	int i, j, k;
	register double sum;

	MPI_Scatter(A, TAM_VET, MPI_DOUBLE, vet_recebe, TAM_VET, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(B, TAM_VET, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	MPI_Barrier(MPI_COMM_WORLD);

	for(i = meu_ranque*num_procs; i < N; i+= num_procs)
		for(j = 0; j < N; j++){
			sum = 0;
			for(k = 0; k < N; k++)
				sum += A[i * N + k] * B[k * N + j];
			C[i * N + j] = sum;
		}

	MPI_Gather(C, TAM_VET, MPI_DOUBLE, C, TAM_VET, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();	
}
