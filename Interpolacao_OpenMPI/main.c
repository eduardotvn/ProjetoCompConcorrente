#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

double* xValuesGenerator(long long int size)
{
    double* xValues = (double*)malloc(size * sizeof(double));
    double starter = (double)-(size / 2);

    for (long long int i = 0; i < size; i++)
    {
        xValues[i] = starter + i;
    }
    

    return xValues;
}

double* yValuesGenerator(long long int size)
{
    double* yValues = (double*)malloc(size*sizeof(double));
    
    for(int i = 0; i < size; i++)
    {
        yValues[i] = ((double)rand() / RAND_MAX) * 1000;
    }

    return yValues;
}

double lagrange_basis(double x, double* data_x, int n, int k) {
    double result = 1.0;
    for (int j = 0; j < n; j++) {
        if (j != k) {
            result *= (x - data_x[j]) / (data_x[k] - data_x[j]);
        }
    }
    return result;
}

double lagrange_interpolation(double x, double* data_x, double* data_y, int n) {
    double result = 0.0;
    for (int i = 0; i < n; i++) {
        result += data_y[i] * lagrange_basis(x, data_x, n, i);
    }
    return result;
}

int main(int argc, char *argv[]) {
    int rank, size;
    long long int poly_size;
    double* xValues = xValuesGenerator(size);
    double* yValues = yValuesGenerator(size);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    double result;

    int chunk_size = poly_size / size;
    int remainder = poly_size % size;
    int start_index = rank * chunk_size + (rank < remainder ? rank : remainder);
    int end_index = start_index + chunk_size + (rank < remainder ? 1 : 0);

    for (int i = start_index; i < end_index; i++) {
        result = lagrange_interpolation(xValues[i], xValues, yValues, poly_size);
        printf("Processo %d: Valor interpolado em x=%.2f eh %.2f\n", rank, xValues[i], result);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
