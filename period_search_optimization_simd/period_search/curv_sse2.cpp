#include <cmath>
#include <vector>
#include <emmintrin.h>
#include "globals.h"
#include "CalcStrategySse2.hpp"

#if defined(__GNUC__)
__attribute__((target("sse2")))
#endif

/**
 * @brief Computes the curvature function and facet area from the Laplace series.
 *
 * This function calculates the curvature function and hence the facet area based on the Laplace series
 * using the provided coefficients and global data. The results are stored in the global variables.
 *
 * @param cg A reference to a vector of doubles containing the coefficients for the Laplace series.
 * @param gl A reference to a globals structure containing necessary global data.
 *
 * @note The function modifies the global variables `Area` and `Dg`.
 *
 * @date 8.11.2006
 */
void CalcStrategySse2::curv(std::vector<double>& cg, globals &gl)
{
	int k;

	for (auto i = 1; i <= Numfac; i++)
	{
		double g = 0;
		int n = 0;
		//m=0
		for (auto l = 0; l <= Lmax; l++)
		{
            n++;
            const double fsum = cg[n] * Fc[i][0];
			g = g + Pleg[i][l][0] * fsum;
		}
		//
		for (auto m = 1; m <= Mmax; m++)
		{
			for (auto l = m; l <= Lmax; l++)
			{
                n++;
				double fsum = cg[n] * Fc[i][m];
				n++;
				fsum = fsum + cg[n] * Fs[i][m];
				g = g + Pleg[i][l][m] * fsum;
			}
		}

		g = exp(g);
		gl.Area[i - 1] = gl.Darea[i - 1] * g;
        const __m128d avx_g = _mm_set1_pd(g);

		for (k = 1; k < n; k += 2)
		{
			__m128d avx_pom = _mm_loadu_pd(&Dsph[i][k]);
			avx_pom = _mm_mul_pd(avx_pom, avx_g);
			_mm_store_pd(&gl.Dg[i - 1][k - 1], avx_pom);
		}

		if (k == n)
		{
			gl.Dg[i - 1][k - 1] = g * Dsph[i][k]; //last odd value
		}
	}
}