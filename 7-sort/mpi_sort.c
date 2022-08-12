/**********************************************************************
 * Enumeration sort
 * Based on https://github.com/yohannteston/Parallel-course-Lab3/blob/master/labOpenMP/enumsort.c
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <mpi.h>

#define LENGTH 8

FILE *fin, *fout;

char *strings;
long int N;

void openfiles() {
	fin = stdin;
//	fin = fopen("sort.in", "r+");
//	if (fin == NULL ) {
//		perror("fopen fin");
//		exit(EXIT_FAILURE);
//	}

	fout = stdout;
//	if (fout == NULL ) {
//		perror("fopen fout");
//		exit(EXIT_FAILURE);
//	}
}

void closefiles(void) {
//	fclose(fin);
//	fclose(fout);
}

void enun_sort(char *a, int length, long int size, int argc, char* argv[]) {

	long int i, j, rank;
	char *tmp = malloc(length);
	int myrank,nproc;
	int thread_status;
	MPI_Init_thread(&argc, &argv,MPI_THREAD_MULTIPLE,&thread_status);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
   	MPI_Comm_size(MPI_COMM_WORLD,&nproc);

	/* Enumeration sort */
	#pragma omp parallel for private(j,i,rank)
	for (j = myrank*nproc; j < size; j += nproc) {
		rank = 0;
		for (i = 0; i < size; i++) {
			if (strcmp(a + (i * length), a + (j * length)) < 0)
				rank++;
		}
		if (j < rank)
			while (rank < size && !strcmp(a + (rank * length), a + (j * length)))
				rank++;
		else
			while (rank < j && !strcmp(a + (rank * length), a + (j * length)))
				rank++;
		if (rank != j) {
			strcpy(tmp, a + (rank * length));
			strcpy(a + (rank * length), a + (j * length));
			strcpy(a + (j * length), tmp);
			j--;
		}
	}
	MPI_Finalize();
	free(tmp);
}

int main(int argc, char* argv[]) {

	long int i;

	openfiles();

	fscanf(fin, "%ld", &N);
	strings = (char*) calloc(N, LENGTH);
	if (strings == NULL ) {
		perror("malloc strings");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < N; i++)
		fscanf(fin, "%s", strings + (i * LENGTH));

	enun_sort(strings, LENGTH, N, argc, argv);

	for (i = 0; i < N; i++)
		fprintf(fout, "%s\n", strings + (i * LENGTH));

	free(strings);
	closefiles();

	return EXIT_SUCCESS;
}

