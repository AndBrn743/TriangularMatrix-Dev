//
// Created by Andy on 1/18/2023.
//

#pragma once

#include <cassert>
#include <cmath>
#include <type_traits>

#include "MathExt.hpp"


/// \Summary
/// Returns the regular (canonical) index of natual indices in <b>Natural Number</b> domain
/// \Note This method REQUIRE the first argument greater or equal to the second one.
///       If you cannot guarantee this requirement, please invoke <code>Reg0</code> instead.
/// \param i The 1st natual index
/// \param j The 2nd natual index
inline constexpr long unsafe_Regin0(long i, long j)
{
	assert(i >= j
	       && "Bad usage of `unsafe_Regin0`. \n\t-> Please either change the code to met the invoke condition or "
	          "switch to `Regin0`");
	return (i * (i + 1)) / 2 + j;
}


/// \Summary
/// Returns the regular (canonical) index of natual indices in <b>Natural Number</b> domain
/// \param i The 1st natual index
/// \param j The 2nd natual index
inline constexpr long Regin0(long i, long j)
{
	return i >= j ? unsafe_Regin0(i, j) : unsafe_Regin0(j, i);
}


/// \Summary
/// Returns the regular (canonical) index of natual indices in <b>Positive Integer</b> domain
/// \Note This method REQUIRE the first argument greater or equal to the second one
///       If you cannot guarantee this requirement, please invoke <code>Reg1</code> instead.
/// \param i The 1st natual index
/// \param j The 2nd natual index
inline constexpr long unsafe_Regin1(long i, long j)
{
	assert(i >= j
	       && "Bad usage of `unsafe_Regin1`. \n\t-> Please either change the code to met the invoke condition or "
	          "switch to `Regin1`");
	return (i * (i - 1)) / 2 + j;
}


/// \Summary
/// Returns the regular (canonical) index of natual indices in <b>Positive Integer</b> domain
/// \param i The 1st natual index
/// \param j The 2nd natual index
inline constexpr long Regin1(long i, long j)
{
	return i >= j ? unsafe_Regin1(i, j) : unsafe_Regin1(j, i);
}


/// \Summary
/// Returns the regular (canonical) index of natual indices in <b>Natural Number</b> domain
/// \param i The 1st natual index from the 1st index pair
/// \param j The 2nd natual index from the 1st index pair
/// \param k The 1st natual index from the 2nd index pair
/// \param l The 2nd natual index from the 2nd index pair
inline constexpr long Regin0(long i, long j, long k, long l)
{
	return Regin0(Regin0(i, j), Regin0(k, l));
}


/// \Summary
/// Returns the regular (canonical) index of natual indices in <b>Positive Integer</b> domain
/// \param i The 1st natual index from the 1st index pair
/// \param j The 2nd natual index from the 1st index pair
/// \param k The 1st natual index from the 2nd index pair
/// \param l The 2nd natual index from the 2nd index pair
inline constexpr long Regin1(long i, long j, long k, long l)
{
	return Regin1(Regin1(i, j), Regin1(k, l));
}


/// \Summary
/// Returns the regular (canonical) index of natual indices in <b>Natural Number</b> domain
/// \tparam N Operation Power
///         <p><code>Reg0\<2\>(i) = Regin0(i, i)</code>
///         <p><code>Reg0\<4\>(i) = Regin0(i, i, i, i)</code>
/// \param i  The natual index
template<int N>
inline long Regin0(long i)
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

template<>
inline constexpr long Regin0<0>(__attribute__((unused)) long i)
{
	return 0;
}

template<>
inline constexpr long Regin0<1>(long i)
{
	return i;
}

template<>
inline constexpr long Regin0<2>(long i)
{
	return unsafe_Regin0(i, i);
}

template<>
inline constexpr long Regin0<4>(long i)
{
	auto result = unsafe_Regin0(i, i);
	result = unsafe_Regin0(result, result);
	return result;
}


/// \Summary
/// Returns the regular (canonical) index of natual indices in <b>Positive Integer</b> domain
/// \tparam N Operation Power
///         <p><code>Reg1\<2\>(i) = Regin1(i, i)</code>
///         <p><code>Reg1\<4\>(i) = Regin1(i, i, i, i)</code>
/// \param i  The natual index
template<int N>
inline constexpr long Regin1(long i)
{
	static_assert(IsPowerOfTwo<int>(N), "Template parameter `N` must be a integer power of two");
	if (N == 0)
	{
		return 1;
	}
	
	long result = i;
	long power = N;
	
	while (power > 1)
	{
		result = unsafe_Regin1(result, result);
		power /= 2;
	}
	
	return result;
}

template<>
inline constexpr long Regin1<0>(__attribute__((unused)) long i)
{
	return 1;
}

template<>
inline constexpr long Regin1<1>(long i)
{
	return i;
}

template<>
inline constexpr long Regin1<2>(long i)
{
	return unsafe_Regin1(i, i);
}

template<>
inline constexpr long Regin1<4>(long i)
{
	auto result = unsafe_Regin1(i, i);
	result = unsafe_Regin1(result, result);
	return result;
}
