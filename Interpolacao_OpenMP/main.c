#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>

double* xValuesGenerator(long long int size)
{
    double* xValues = (double*)malloc(size * sizeof(double));
    double starter = (double)-(size / 2);

    #pragma omp parallel
    {
        int num_threads = omp_get_num_threads();
        printf("numthreads: %d\n", num_threads);
        int thread_id = omp_get_thread_num();
        long long int chunk_size = size / num_threads;
        long long int start_index = thread_id * chunk_size;
        long long int end_index = (thread_id == num_threads - 1) ? size : start_index + chunk_size;

        for (long long int i = start_index; i < end_index; i++)
        {
            xValues[i] = starter + i;
        }
    }

    return xValues;
}

double* yValuesGenerator(long long int size)
{
    double* yValues = (double*)malloc(size*sizeof(double));
    
    #pragma omp parallel for

    for(int i = 0; i < size; i++)
    {
        yValues[i] = ((double)rand() / RAND_MAX) * 1000;
    }

    return yValues;
}

double lagrangeInterpolation(double* xValues, double* yValues, long long int size, double x)
{

    double result = 0.0;

    #pragma omp parallel for reduction(+:result)
    for(long long int i = 0; i < size; i++)
    {
        double y = yValues[i];
        for(long long j = 0; j < size; j++)
        {
            if (j != i)
            {
                y *= ((x - xValues[j])/(xValues[i] - xValues[j]));
            }
        }
        result += y;  
    }

    return result;
}

void main(){
    srand(time(NULL));

    double start_time, end_time;
    start_time = omp_get_wtime();

    long long int size = 10000;

    double* xValues = xValuesGenerator(size);
    double* yValues = yValuesGenerator(size);

    double interpolation = lagrangeInterpolation(xValues, yValues, size, 1.5);

    end_time = omp_get_wtime();

    printf("Interpolacao para 1.5 = %f\n", interpolation);
    printf("Duracao: %f segundos\n", end_time - start_time);
}
