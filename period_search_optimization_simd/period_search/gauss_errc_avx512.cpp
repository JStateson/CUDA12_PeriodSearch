#define SWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}

#include <cmath>
#include <cstdlib>
#include <vector>
#include <immintrin.h>
#include "declarations.h"
#include "CalcStrategyAvx512.hpp"

#if defined(__GNUC__)
__attribute__((target("avx512f")))
#endif

/**
* @brief Solves a linear system of equations using Gaussian elimination with partial pivoting.
*
* This function implements the Gaussian elimination algorithm with partial pivoting to solve a
* linear system of equations. It rearranges the covariance matrix and the right-hand side vector
* to find the solution.
*
* @param gl A reference to a globals structure containing the covariance matrix and other global data.
* @param n The dimension of the system (number of equations/variables).
* @param b A vector of doubles representing the right-hand side vector of the system.
* @param error An integer reference to store error codes:
*              - 0: No error
*              - 1: Singular matrix
*              - 2: Zero pivot element
*
* @note The function modifies the covariance matrix `covar` in place.
*
* @source Numerical Recipes
*
* @date 8.11.2006
*/
void CalcStrategyAvx512::gauss_errc(struct globals& gl, const int n, std::vector<double>& b, int& error)
{
	//int * indxc, * indxr, * ipiv;
	int i, icol = 0, irow = 0, j, k, l, ll;
	double big, dum, pivinv, temp;

	auto& a = gl.covar;

	//indxc = vector_int(n + 1);
	std::vector<int> indxc(n + 1 + 1, 0);
	//indxr = vector_int(n + 1);
	std::vector<int> indxr(n + 1 + 1, 0);
	//ipiv = vector_int(n + 1);
	//memset(ipiv, 0, n * sizeof(int));
	std::vector<int> ipiv(n + 1 + 1, 0);


	for (i = 1; i <= n; i++)
	{
		big = 0.0;
		for (j = 0; j < n; j++)
		{
			if (ipiv[j] != 1)
			{
				for (k = 0; k < n; k++)
				{
					if (ipiv[k] == 0)
					{
						if (fabs(a[j][k]) >= big)
						{
							big = fabs(a[j][k]);
							irow = j;
							icol = k;
						}
					}
					else if (ipiv[k] > 1)
					{
						//deallocate_vector((void*)indxc);
						//deallocate_vector((void*)ipiv);
						//deallocate_vector((void*)indxr);
						error = 1;

						return;
					}
				}
			}
		}
		++(ipiv[icol]);
		if (irow != icol)
		{
			for (l = 0; l < n; l++) SWAP(a[irow][l], a[icol][l])
				SWAP(b[irow], b[icol])
		}

		indxr[i] = irow;
		indxc[i] = icol;

		if (a[icol][icol] == 0.0)
		{
			//deallocate_vector((void*)indxc);
			//deallocate_vector((void*)ipiv);
			//deallocate_vector((void*)indxr);
			error = 2;

			return;
		}

		pivinv = 1.0 / a[icol][icol];
		__m512d avx_pivinv = _mm512_set1_pd(pivinv);
		a[icol][icol] = 1.0;
		int cyklus = (n >> 2) << 2;

		for (l = 0; l < cyklus; l += 8)
		{
			__m512d avx_a1 = _mm512_load_pd(&a[icol][l]);
			avx_a1 = _mm512_mul_pd(avx_a1, avx_pivinv);
			_mm512_store_pd(&a[icol][l], avx_a1);
		}

		if (l < n) a[icol][l] *= pivinv; //last odd value
		if (l + 1 < n) a[icol][l + 1] *= pivinv; //last odd value
		if (l + 2 < n) a[icol][l + 2] *= pivinv; //last odd value

		b[icol] *= pivinv;
		for (ll = 0; ll < n; ll++)
		{
			if (ll != icol)
			{
				dum = a[ll][icol];
				a[ll][icol] = 0.0;
				__m512d avx_dum = _mm512_set1_pd(dum);
				for (l = 0; l < cyklus; l += 8)
				{
					__m512d avx_a = _mm512_load_pd(&a[ll][l]);
					__m512d avx_aa = _mm512_load_pd(&a[icol][l]);
					avx_a = _mm512_fnmadd_pd(avx_aa, avx_dum, avx_a);
					_mm512_store_pd(&a[ll][l], avx_a);
				}

				if (l < n) a[ll][l] -= a[icol][l] * dum; //last odd value
				if (l + 1 < n) a[ll][l + 1] -= a[icol][l + 1] * dum; //last odd value
				if (l + 2 < n) a[ll][l + 2] -= a[icol][l + 2] * dum; //last odd value

				b[ll] -= b[icol] * dum;
			}
		}
	}

	for (l = n; l >= 1; l--)
	{
		if (indxr[l] != indxc[l])
			for (k = 0; k < n; k++)
				SWAP(a[k][indxr[l]], a[k][indxc[l]]);
	}

	//deallocate_vector((void*)indxc);
	//deallocate_vector((void*)ipiv);
	//deallocate_vector((void*)indxr);
	error = 0;
	return;
}
#undef SWAP
