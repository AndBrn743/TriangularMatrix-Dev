//
// Created by Andy on 7/31/2023.
//

// ReSharper disable CppDFAUnreachableFunctionCall
// ReSharper disable CppUseTypeTraitAlias
#pragma once

#include <cmath>
#include <complex>
#include <limits>
#include <type_traits>


/// \Summary
/// Contains mathematical functions that should come with STL but not
template <typename T>
constexpr bool IsIntegerType()
{
	return std::is_integral<T>::value;
}


template <typename T>
constexpr bool IsRealType()
{
	return std::is_same<T, double>::value || std::is_same<T, float>::value;
}


typedef std::complex<double> DoubleComplex;
typedef std::complex<float> SingleComplex;


template <typename T>
constexpr bool IsComplexType()
{
	return std::is_same<T, DoubleComplex>::value || std::is_same<T, SingleComplex>::value;
}


template <typename T>
constexpr bool IsFloatPointNumericalType()
{
	return IsRealType<T>() || IsComplexType<T>();
}


template <typename T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type IsOdd(T n)
{
	return n % 2 != 0;
}


template <typename T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type IsEven(T n)
{
	return n % 2 == 0;
}


template <typename T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type IsPowerOfTwo(T n)
{
	return (n & (n - 1)) == 0;
}


namespace MathExt
{
	template <typename TReal>
	constexpr typename std::enable_if<IsRealType<TReal>(), TReal>::type  //
	RealPart(const TReal number)
	{
		return number;
	}

	template <typename TReal>
	constexpr typename std::enable_if<IsRealType<TReal>(), TReal>::type  //
	RealPart(const std::complex<TReal> number)
	{
		return std::real(number);
	}

	template <typename TReal>
	constexpr typename std::enable_if<IsRealType<TReal>(), TReal>::type  //
	ImaginaryPart(const TReal)
	{
		return TReal(0);
	}

	template <typename TReal>
	constexpr typename std::enable_if<IsRealType<TReal>(), TReal>::type  //
	ImaginaryPart(const std::complex<TReal> number)
	{
		return std::imag(number);
	}

	template <typename T>
	constexpr auto Re(T number) -> decltype(RealPart(number))
	{
		return RealPart(number);
	}

	template <typename T>
	constexpr auto Im(T number) -> decltype(ImaginaryPart(number))
	{
		return ImaginaryPart(number);
	}

	template <typename TReal>
	constexpr typename std::enable_if<IsRealType<TReal>(), TReal>::type  //
	Conjugate(const TReal number)
	{
		return number;
	}

	template <typename TReal>
	constexpr typename std::enable_if<IsRealType<TReal>(), std::complex<TReal>>::type  //
	Conjugate(const std::complex<TReal> number)
	{
		return std::conj(number);
	}

	template <typename TReal, typename TInteger>
	constexpr typename std::enable_if<IsRealType<TReal>() && IsIntegerType<TInteger>(), std::complex<TReal>>::type
	operator*(std::complex<TReal> lhs, TInteger rhs)
	{
		return lhs * std::complex<TReal>(RT(rhs));
	}

	template <typename TReal, typename TInteger>
	constexpr typename std::enable_if<IsRealType<TReal>() && IsIntegerType<TInteger>(), std::complex<TReal>>::type
	operator*(TInteger lhs, std::complex<TReal> rhs)
	{
		return std::complex<TReal>(TReal(lhs)) * rhs;
	}

	template <typename TReal, typename TInteger>
	constexpr typename std::enable_if<IsRealType<TReal>() && IsIntegerType<TInteger>(), std::complex<TReal>>::type
	operator/(std::complex<TReal> lhs, TInteger rhs)
	{
		return lhs / std::complex<TReal>(TReal(rhs));
	}

	template <typename TReal, typename TInteger>
	constexpr typename std::enable_if<IsRealType<TReal>() && IsIntegerType<TInteger>(), std::complex<TReal>>::type
	operator/(TInteger lhs, std::complex<TReal> rhs)
	{
		return std::complex<TReal>(TReal(lhs)) / rhs;
	}

	template <typename TReal, typename TInteger>
	constexpr typename std::enable_if<IsRealType<TReal>() && IsIntegerType<TInteger>(), std::complex<TReal>>::type
	operator+(std::complex<TReal> lhs, TInteger rhs)
	{
		return lhs + std::complex<TReal>(TReal(rhs));
	}

	template <typename TReal, typename TInteger>
	constexpr typename std::enable_if<IsRealType<TReal>() && IsIntegerType<TInteger>(), std::complex<TReal>>::type
	operator+(TInteger lhs, std::complex<TReal> rhs)
	{
		return std::complex<TReal>(TReal(lhs)) + rhs;
	}

	template <typename TReal, typename TInteger>
	constexpr typename std::enable_if<IsRealType<TReal>() && IsIntegerType<TInteger>(), std::complex<TReal>>::type
	operator-(std::complex<TReal> lhs, TInteger rhs)
	{
		return lhs - std::complex<TReal>(TReal(rhs));
	}

	template <typename TReal, typename TInteger>
	constexpr typename std::enable_if<IsRealType<TReal>() && IsIntegerType<TInteger>(), std::complex<TReal>>::type
	operator-(TInteger lhs, std::complex<TReal> rhs)
	{
		return std::complex<TReal>(TReal(lhs)) - rhs;
	}

	constexpr double Exp(const double x)
	{
		return exp(x);
	}

	constexpr double Pow(const double x, const double n)
	{
		return pow(x, n);
	}

	constexpr double Pow(double x, int n)
	{
		if (n > 20 || n < -20)
		{
			return std::pow(x, n);
		}

		if (n == 0)
		{
			return 1.0;
		}

		if (n < 0)
		{
			x = 1.0 / x;
			n = -n;
		}

		double result = 1.0;
		while (n > 0)
		{
			if (n & 1)
			{
				result *= x;
			}
			x *= x;
			n >>= 1;
		}
		return result;
	}

	constexpr double Square(const double x)
	{
		return x * x;
	}

	constexpr double Cube(const double x)
	{
		return x * x * x;
	}

	constexpr double Round(const double value)
	{
		return std::round(value);
	}

	constexpr double Round(const double value, const int digits)
	{
		const double multiplier = std::pow(10.0, digits);
		return std::round(value * multiplier) / multiplier;
	}

	constexpr double Sqrt(const double value)
	{
		return std::sqrt(value);
	}

	constexpr double Log(const double value)
	{
		return std::log(value);
	}

	constexpr double Sin(const double value)
	{
		return std::sin(value);
	}

	constexpr double Cos(const double value)
	{
		return std::cos(value);
	}

	constexpr double Tan(const double value)
	{
		return std::tan(value);
	}

	constexpr double Asin(const double value)
	{
		return std::asin(value);
	}

	constexpr double Acos(const double value)
	{
		return std::acos(value);
	}

	constexpr double Atan(const double value)
	{
		return std::atan(value);
	}

	constexpr double Sinh(const double x)
	{
		return 0.5 * (Exp(x) - Exp(-x));
	}

	constexpr double Cosh(const double x)
	{
		return std::cosh(x);
	}

	constexpr double Tanh(const double x)
	{
		return std::tanh(x);
	}

	constexpr double Sech(const double x)
	{
		return 1.0 / Cosh(x);
	}

	constexpr double Asinh(const double x)
	{
		return std::asin(x);
	}

	constexpr double Acosh(const double x)
	{
		return std::acosh(x);
	}

	constexpr double Atanh(const double x)
	{
		return std::atanh(x);
	}

	constexpr double Asech(const double x)
	{
		if (x <= 0.0 || x > 1.0)
		{
			return std::numeric_limits<double>::quiet_NaN();  // Invalid input
		}
		return Acosh(1.0 / x);
	}

	constexpr double LogisticFunction(const double x)
	{
		return 1.0 / (1.0 + Exp(-x));
	}

	constexpr double LogisticFunctionExtrapolation(const double x,
	                                               const double x1,
	                                               const double y1,
	                                               const double x2,
	                                               const double y2,
	                                               const double max = 1,
	                                               const double min = 0)
	{
		const double scaling = max - min;
		const double _y1 = y1 - min;
		const double _y2 = y2 - min;
		const double temp = std::log(((scaling - _y1) * _y2) / (_y1 * (scaling - _y2))) / (x2 - x1);
		const double a = temp / (x2 - x1);
		const double b = std::log(_y1 / (scaling - _y1)) - x1 * a;

		return LogisticFunction(a * x + b) * scaling + min;
	}

	template <typename T>
	constexpr auto QuadraticMean(T x1, T x2)
	{
		return Sqrt((Square(x1) + Square(x2)) / 2);
	}

	template <typename T>
	constexpr auto ArithmeticMean(T x1, T x2)
	{
		return (x1 + x2) / 2;
	}

	template <typename T>
	constexpr auto GeometricMean(T x1, T x2)
	{
		return Sqrt(x1 * x2);
	}

	template <typename T>
	constexpr auto HarmonicMean(T x1, T x2)
	{
		if (x1 == T(0) || x2 == T(0))
		{
			return T(0);
		}

		constexpr T one = T(1);
		return one / (one / x1 + one / x2);
	}
}  // namespace MathExt
