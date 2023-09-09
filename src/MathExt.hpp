//
// Created by Andy on 7/31/2023.
//

#pragma once

#include <type_traits>
#include <cmath>
#include <complex>
#include <limits>


/// \Summary
/// Contains mathematical functions that should come with STL but not
template<typename T>
inline static constexpr bool IsIntegerType()
{
	return std::is_integral<T>::value;
}


template<typename T>
inline static constexpr bool IsRealType()
{
	return std::is_same<T, double>::value || std::is_same<T, float>::value;
}


typedef std::complex<double> DoubleComplex;
typedef std::complex<float> SingleComplex;


template<typename T>
inline static constexpr bool IsComplexType()
{
	return std::is_same<T, DoubleComplex>::value || std::is_same<T, SingleComplex>::value;
}


template<typename T>
inline static constexpr bool IsFloatPointNumericalType()
{
	return IsRealType<T>() || IsComplexType<T>();
}


template<typename T>
inline static constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type IsOdd(T n)
{
	return n % 2 != 0;
}


template<typename T>
inline static constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type IsEven(T n)
{
	return n % 2 == 0;
}


template<typename T>
inline static constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type IsPowerOfTwo(T n)
{
	return (n & (n - 1)) == 0;
}


namespace MathExt
{
	template<typename TReal>
	static inline constexpr typename std::enable_if<IsRealType<TReal>(), TReal>::type  //
	RealPart(const TReal number)
	{
		return number;
	}
	
	template<typename TReal>
	static inline constexpr typename std::enable_if<IsRealType<TReal>(), TReal>::type  //
	RealPart(const std::complex<TReal> number)
	{
		return std::real(number);
	}
	
	template<typename TReal>
	static inline constexpr typename std::enable_if<IsRealType<TReal>(), TReal>::type  //
	ImaginaryPart(const TReal)
	{
		return TReal(0);
	}
	
	template<typename TReal>
	static inline constexpr typename std::enable_if<IsRealType<TReal>(), TReal>::type  //
	ImaginaryPart(const std::complex<TReal> number)
	{
		return std::imag(number);
	}
	
	template<typename T>
	static inline constexpr auto Re(T number) -> decltype(RealPart(number))
	{
		return RealPart(number);
	}
	
	template<typename T>
	static inline constexpr auto Im(T number) -> decltype(ImaginaryPart(number))
	{
		return ImaginaryPart(number);
	}
	
	template<typename TReal>
	static inline constexpr typename std::enable_if<IsRealType<TReal>(), TReal>::type  //
	Conjugate(const TReal number)
	{
		return number;
	}
	
	template<typename TReal>
	static inline constexpr typename std::enable_if<IsRealType<TReal>(), std::complex<TReal>>::type  //
	Conjugate(const std::complex<TReal> number)
	{
		return std::conj(number);
	}
	
	template<typename TReal, typename TInteger>
	static inline constexpr
	typename std::enable_if<IsRealType<TReal>() && IsIntegerType<TInteger>(), std::complex<TReal>>::type
	operator*(std::complex<TReal> lhs, TInteger rhs)
	{
		return lhs * std::complex<TReal>(RT(rhs));
	}
	
	template<typename TReal, typename TInteger>
	static inline constexpr
	typename std::enable_if<IsRealType<TReal>() && IsIntegerType<TInteger>(), std::complex<TReal>>::type
	operator*(TInteger lhs, std::complex<TReal> rhs)
	{
		return std::complex<TReal>(TReal(lhs)) * rhs;
	}
	
	template<typename TReal, typename TInteger>
	static inline constexpr
	typename std::enable_if<IsRealType<TReal>() && IsIntegerType<TInteger>(), std::complex<TReal>>::type
	operator/(std::complex<TReal> lhs, TInteger rhs)
	{
		return lhs / std::complex<TReal>(TReal(rhs));
	}
	
	template<typename TReal, typename TInteger>
	static inline constexpr
	typename std::enable_if<IsRealType<TReal>() && IsIntegerType<TInteger>(), std::complex<TReal>>::type
	operator/(TInteger lhs, std::complex<TReal> rhs)
	{
		return std::complex<TReal>(TReal(lhs)) / rhs;
	}
	
	template<typename TReal, typename TInteger>
	static inline constexpr
	typename std::enable_if<IsRealType<TReal>() && IsIntegerType<TInteger>(), std::complex<TReal>>::type
	operator+(std::complex<TReal> lhs, TInteger rhs)
	{
		return lhs + std::complex<TReal>(TReal(rhs));
	}
	
	template<typename TReal, typename TInteger>
	static inline constexpr
	typename std::enable_if<IsRealType<TReal>() && IsIntegerType<TInteger>(), std::complex<TReal>>::type
	operator+(TInteger lhs, std::complex<TReal> rhs)
	{
		return std::complex<TReal>(TReal(lhs)) + rhs;
	}
	
	template<typename TReal, typename TInteger>
	static inline constexpr
	typename std::enable_if<IsRealType<TReal>() && IsIntegerType<TInteger>(), std::complex<TReal>>::type
	operator-(std::complex<TReal> lhs, TInteger rhs)
	{
		return lhs - std::complex<TReal>(TReal(rhs));
	}
	
	template<typename TReal, typename TInteger>
	static inline constexpr
	typename std::enable_if<IsRealType<TReal>() && IsIntegerType<TInteger>(), std::complex<TReal>>::type
	operator-(TInteger lhs, std::complex<TReal> rhs)
	{
		return std::complex<TReal>(TReal(lhs)) - rhs;
	}
	
	static inline constexpr double Exp(double x)
	{
		return exp(x);
	}
	
	static inline constexpr double Pow(double x, double n)
	{
		return pow(x, n);
	}
	
	static double Pow(double x, int n)
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
	
	static inline constexpr double Square(double x)
	{
		return x * x;
	}
	
	static inline constexpr double Cube(double x)
	{
		return x * x * x;
	}
	
	static inline constexpr double Round(double value)
	{
		return std::round(value);
	}
	
	static inline double Round(double value, int digits)
	{
		double multiplier = std::pow(10.0, digits);
		return std::round(value * multiplier) / multiplier;
	}
	
	static inline constexpr double Sqrt(double value)
	{
		return std::sqrt(value);
	}
	
	static inline constexpr double Log(double value)
	{
		return std::log(value);
	}
	
	static inline constexpr double Sin(double value)
	{
		return std::sin(value);
	}
	
	static inline constexpr double Cos(double value)
	{
		return std::cos(value);
	}
	
	static inline constexpr double Tan(double value)
	{
		return std::tan(value);
	}
	
	static inline constexpr double Asin(double value)
	{
		return std::asin(value);
	}
	
	static inline constexpr double Acos(double value)
	{
		return std::acos(value);
	}
	
	static inline constexpr double Atan(double value)
	{
		return std::atan(value);
	}
	
	static inline constexpr double Sinh(double x)
	{
		return 0.5 * (Exp(x) - Exp(-x));
	}
	
	static inline constexpr double Cosh(double x)
	{
		return std::cosh(x);
	}
	
	static inline constexpr double Tanh(double x)
	{
		return std::tanh(x);
	}
	
	static inline constexpr double Sech(double x)
	{
		return 1.0 / Cosh(x);
	}
	
	static inline constexpr double Asinh(double x)
	{
		return std::asin(x);
	}
	
	static inline constexpr double Acosh(double x)
	{
		return std::acosh(x);
	}
	
	static inline constexpr double Atanh(double x)
	{
		return std::atanh(x);
	}
	
	static inline double Asech(double x)
	{
		if (x <= 0.0 || x > 1.0)
		{
			return std::numeric_limits<double>::quiet_NaN();  // Invalid input
		}
		return Acosh(1.0 / x);
	}
	
	static inline constexpr double LogisticFunction(double x)
	{
		return 1.0 / (1.0 + Exp(-x));
	}
	
	static inline double LogisticFunctionExtrapolation(
			double x, double x1, double y1, double x2, double y2, double max = 1, double min = 0)
	{
		double scaling = max - min;
		double _y1 = y1 - min;
		double _y2 = y2 - min;
		double temp = std::log(((scaling - _y1) * _y2) / (_y1 * (scaling - _y2))) / (x2 - x1);
		double a = temp / (x2 - x1);
		double b = std::log(_y1 / (scaling - _y1)) - x1 * a;
		
		return LogisticFunction(a * x + b) * scaling + min;
	}
	
	template<typename T>
	static inline constexpr auto QuadraticMean(T x1, T x2)
	{
		return Sqrt((Square(x1) + Square(x2)) / 2);
	}
	
	template<typename T>
	static inline constexpr auto ArithmeticMean(T x1, T x2)
	{
		return (x1 + x2) / 2;
	}
	
	template<typename T>
	static inline constexpr auto GeometricMean(T x1, T x2)
	{
		return Sqrt(x1 * x2);
	}
	
	template<typename T>
	static inline constexpr auto HarmonicMean(T x1, T x2)
	{
		if (x1 == T(0) || x2 == T(0))
		{
			return T(0);
		}
		
		constexpr const T one = T(1);
		return one / (one / x1 + one / x2);
	}
}  // namespace MathExt
