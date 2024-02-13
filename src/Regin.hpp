//
// Created by Andy on 1/18/2023.
//

// ReSharper disable CppDFAUnreachableFunctionCall
#pragma once

#include <cassert>

#include "MathExt.hpp"


constexpr long unsafe_Regin0(const long i, const long j)
{
	assert((i >= j && i >= 0)
	       && "Bad usage of `unsafe_Regin0`. \n\t-> Please either change the code to met the invoke condition "
	          "or switch to `Regin0`");
	return (i * (i + 1)) / 2 + j;
}

constexpr long Regin0(const long i, const long j)
{
	return i >= j ? unsafe_Regin0(i, j) : unsafe_Regin0(j, i);
}

template <int I, int J>
constexpr int unsafe_Regin0()
{
	static_assert(I >= 0 && J >= 0 && I >= J, "cesvdc");
	return (I * (I + 1)) / 2 + J;
}

template <int I, int J>
constexpr int Regin0()
{
	return I >= J ? unsafe_Regin0<I, J>() : unsafe_Regin0<J, I>();
}

template <>
constexpr int Regin0<Eigen::Dynamic, Eigen::Dynamic>()
{
	return Eigen::Dynamic;
}

template <int N>
constexpr long Regin0(const long i)
{
	static_assert(IsPowerOfTwo<int>(N), "Template parameter `N` must be a integer power of two");
	if (N == 0)
	{
		return 0;
	}

	long result = i;
	long power = N;

	while (power > 1)
	{
		result = unsafe_Regin0(result, result);
		power /= 2;
	}

	return result;
}

template <>
constexpr long Regin0<0>(long)
{
	return 0;
}

template <>
constexpr long Regin0<1>(const long i)
{
	return i;
}

template <>
constexpr long Regin0<2>(const long i)
{
	return unsafe_Regin0(i, i);
}

template <int N>
long Regin1(const long i)
{
	return Regin0<N>(i - 1) + 1;
}

template <int I, int J>
constexpr int Regin1()
{
	return I >= J ? unsafe_Regin0<I - 1, J - 1>() + 1 : unsafe_Regin0<J - 1, I - 1>() + 1;
}

template <>
constexpr int Regin1<Eigen::Dynamic, Eigen::Dynamic>()
{
	return Eigen::Dynamic;
}
