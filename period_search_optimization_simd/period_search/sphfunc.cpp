#include <cmath>
#include "globals.h"
#include "declarations.h"
#include "arrayHelpers.hpp"

/**
 * @brief Computes spherical harmonics functions (denormalized) for Laplace series.
 *
 * This function calculates the spherical harmonics functions based on given angular
 * coordinates. It was converted from Mikko's original Fortran code (J.Durech, 8.11.2006).
 *
 * The function operates on 1-based indexed arrays, and it does not use the zero
 * elements of the input vectors.
 *
 * @param ndir The number of directions.
 * @param at A vector of doubles representing the theta angles. The vector should be of size ndir+1.
 * @param af A vector of doubles representing the phi angles. The vector should be of size ndir+1.
 */
void sphfunc(const int ndir, const std::vector<double>& at, const std::vector<double>& af)
{
    int i, j, m, l, n, k, ibot;

    double aleg[MAX_LM + 1][MAX_LM + 1][MAX_LM + 1];

    for (i = 1; i <= ndir; i++)
    {
        Ts[i][0] = 1;
        Tc[i][0] = 1;
        Ts[i][1] = sin(at[i]);
        Tc[i][1] = cos(at[i]);
        for (j = 2; j <= Lmax; j++)
        {
            Ts[i][j] = Ts[i][1] * Ts[i][j - 1];
            Tc[i][j] = Tc[i][1] * Tc[i][j - 1];
        }
        Fs[i][0] = 0;
        Fc[i][0] = 1;
        for (j = 1; j <= Mmax; j++)
        {
            Fs[i][j] = sin(j*af[i]);
            Fc[i][j] = cos(j*af[i]);

			//printf("[%3d][%3d] % 0.6f\n", i, j, Fc[i][j]);
			//printf("[%3d][%3d] % 0.6f\n", i, j, Fs[i][j]);
        }
    }

    for (m = 0; m <= Lmax; m++)
        for (l = 0; l <= Lmax; l++)
            for (n = 0; n <= Lmax; n++)
                aleg[m][l][n] = 0;

    aleg[0][0][0] = 1;
    aleg[1][1][0] = 1;

    for (l = 1; l <= Lmax; l++)
        aleg[0][l][l] = aleg[0][l - 1][l - 1] * (2 * l - 1);

    for (m = 0; m <= Mmax; m++)
        for (l = m + 1; l <= Lmax; l++)
        {
            if ((2 * ((l - m) / 2)) == (l - m))
            {
                aleg[0][l][m] = -(l + m - 1) * aleg[0][l - 2][m] / (1 * (l - m));
                ibot = 2;
            }
            else
                ibot = 1;

            if (l != 1)
                for (n = ibot; n <= l - m; n = n + 2)
                    aleg[n][l][m] = ((2 * l - 1) * aleg[n - 1][l - 1][m] - (l + m - 1) * aleg[n][l - 2][m]) / (1 * (l - m));
        }

    for (i = 1; i <= ndir; i++)
    {
        k = 0;
        for (m = 0; m <= Mmax; m++)
            for (l = m; l <= Lmax; l++)
            {
                Pleg[i][l][m] = 0;
                if ((2 * ((l - m) / 2)) == (l - m))
                    ibot = 0;
                else
                    ibot = 1;

                for (n = ibot; n <= l - m; n = n + 2)
                    Pleg[i][l][m] = Pleg[i][l][m] + aleg[n][l][m] * Tc[i][n] * Ts[i][m];
                k++;
                Dsph[i][k] = Fc[i][m] * Pleg[i][l][m];
                if (m != 0)
                {
                    k++;
                    Dsph[i][k] = Fs[i][m] * Pleg[i][l][m];
                }
            }
    }

    // NOTE: For unit tests reference only
    //printData(ndir, k, "ndir");

    /*printf("Fc[%d][%d]:\n", ndir, Mmax);
    for (int q = 0; q <= Mmax; q++)
    {
        printf("_fc_%d[] = { ", q);
        for (int p = 0; p <= ndir; p++)
        {
            printf("% 0.6f, ", Fc[p][q]);
            if (p % 9 == 0)
                printf("\n");
        }
        printf("};\n");
    }*/

    /*printf("Fs[%d][%d]:\n", ndir, Mmax);
    for (int q = 0; q <= Mmax; q++)
    {
        printf("_fs_%d[] = { ", q);
        for (int p = 0; p <= ndir; p++)
        {
            printf("% 0.6f, ", Fs[p][q]);
            if (p % 9 == 0)
                printf("\n");
        }
        printf("};\n");
    }*/

    /*printf("\nPleg[%d][%d][%d]:\n", i - 1, l - 1, m - 1);
    for (int r = 0; r <= m - 1; r++)
    {
        for (int q = 0; q <= l - 1; q++)
        {
            printf("\n_pleg_j%d_k%d[] = {", q, r);
            for (int p = 0; p <= i - 1; p++)
            {

                printf("% 0.6f, ", Pleg[p][q][r]);
                if (i % (i-1) == 0)
                    printf("\n");
            }

            printf("};\n");
        }
    }*/

    /*printf("\nDsph[%d][%d]:\n", i - 1, 16);
    for (int q = 0; q <= 16; q++)
    {
        printf("\n_dsph_%d[] = { ", q);
        for (int p = 0; p <= i - 1; p++)
        {
            printf("% 0.6f, ", Dsph[p][q]);
            if (i % 9 == 0)
                printf("\n");
        }
        printf("};\n");
    }*/
}

