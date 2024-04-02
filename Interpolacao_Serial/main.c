#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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

double lagrangeInterpolation(double* xValues, double* yValues, long long int size, double x)
{

    double result = 0.0;

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

    long long int size = 1000;

    double* xValues = xValuesGenerator(size);
    double* yValues = yValuesGenerator(size);

    double interpolation = lagrangeInterpolation(xValues, yValues, size, 1.5);

    end_time = omp_get_wtime();

    printf("Interpolacao para 1.5 = %f\n", interpolation);
    printf("Duracao: %f segundos\n", end_time - start_time);
}
