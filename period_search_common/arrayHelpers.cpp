//#include "pch.h"
#include <cstdio>
#include <algorithm>
#include <memory>
#include <vector>

void init2Darray(double **&matrix, int dytemp_siszeX, int dytemp_sizeY)
{
	matrix = new double* [dytemp_siszeX];
	for (int i = 0; i < dytemp_siszeX; ++i)
	{
		matrix[i] = new double[dytemp_sizeY];
		std::fill_n(matrix[i], dytemp_sizeY, 0.0);
	}
}

void delete2Darray(double **&ary, int sizeX)
{
	for (int i = 0; i < sizeX; ++i) {
		delete[] ary[i];
	}

	delete[] ary;
}

void printArray(int array[], int iMax, char msg[])
{
    printf("\n%s[%d]:\n", msg, iMax);
    for (int i = 0; i <= iMax; i++)
    {
        printf("%d, ", array[i]);
        if (i % 20 == 0)
            printf("\n");
    }
}

void printArray(double array[], int iMax, char msg[])
{
    printf("\n%s[%d]:\n", msg, iMax);
	printf("[0] ");
    for (int i = 0; i <= iMax; i++)
    {
        printf("%.6f, ", array[i]);
        if (i > 0 && i < iMax && i % 9 == 0)
		{
            printf("\n");
			printf("[%d] ", i + 1);
		}
    }
}

void printArray(double **array, int iMax, int jMax, char msg[])
{
    printf("\n%s[%d][%d]:\n", msg, iMax, jMax);
    for (int j = 0; j <= jMax; j++)
    {
        printf("\n_%s_%d[] = { ", msg, j);
        for (int i = 0; i <= iMax; i++)
        {
            printf("% 0.6f, ", array[i][j]);
            if (i % 9 == 0)
                printf("\n");
        }
        printf("};\n");
    }
}

void printArray(double ***array, int iMax, int jMax, int kMax, char msg[])
{
    printf("\n%s[%d][%d][%d]:\n", msg, iMax, jMax, kMax);
    for(int k = 0; k <= kMax; k++)
    {
        for(int j = 0; j <= jMax; j++)
        {
            printf("\n_%s_j%d_k%d[] = {", msg, j, k);
            for(int i = 0; i <= iMax; i++)
            {

                printf("%.30f, ", array[i][j][k]);
                if (i % 9 == 0)
                    printf("\n");
            }

            printf("};\n");
        }
    }
}