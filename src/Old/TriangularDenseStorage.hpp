//
// Created by Andy on 9/2/2023.
//

#pragma once

#include <valarray>

#include "Regin.hpp"


namespace Hoppy
{
	template <typename TScalar>
	class TriangularDenseStorage
	{
	public:
		using Scalar = TScalar;
		using Iterator = TScalar*;
		using ConstIterator = const TScalar*;


	public:
		TriangularDenseStorage() = default;

		explicit TriangularDenseStorage(long dimension, TScalar value = TScalar(0))
		    : m_Dimension(dimension), m_DataBuffer(value, CalculateSizeOfDimension(dimension))
		{
			/* NO CODE */
		}

		inline void Resize(long dimension)
		{
			m_DataBuffer.resize(CalculateSizeOfDimension(dimension));
			m_Dimension = dimension;
		}

		void ConservativeResize(long dimension)
		{
			long newSize = CalculateSizeOfDimension(dimension);
			if (newSize == this->Size())
			{
				return;
			}

			std::valarray<TScalar> newDataBuffer(TScalar(0), newSize);

			for (long i = 0; i < std::min(newSize, this->Size()); i++)
			{
				newDataBuffer[i] = this->m_DataBuffer[i];
			}

			std::swap(this->m_DataBuffer, newDataBuffer);
			m_Dimension = dimension;
		}

		inline long Dimension() const
		{
			return m_Dimension;
		}

		inline long Size() const
		{
			return m_DataBuffer.size();
		}


	public:
		inline void FillWith(TScalar s)
		{
			m_DataBuffer = s;
		}

		inline void operator+=(TScalar s)
		{
			m_DataBuffer += s;
		}

		inline void operator-=(TScalar s)
		{
			m_DataBuffer -= s;
		}

		inline void operator*=(TScalar s)
		{
			m_DataBuffer *= s;
		}

		inline void operator/=(TScalar s)
		{
			m_DataBuffer /= s;
		}

		inline void operator+=(const TriangularDenseStorage& tds)
		{
			m_DataBuffer += tds;
		}

		inline void operator-=(const TriangularDenseStorage& tds)
		{
			m_DataBuffer -= tds;
		}

		inline void operator*=(const TriangularDenseStorage& tds)
		{
			m_DataBuffer *= tds;
		}

		inline void operator/=(const TriangularDenseStorage& tds)
		{
			m_DataBuffer /= tds;
		}

		friend inline TriangularDenseStorage operator+(const TriangularDenseStorage& lhs,
		                                               const TriangularDenseStorage& rhs)
		{
			TriangularDenseStorage result(lhs);
			result += rhs;
			return result;
		}

		friend inline TriangularDenseStorage operator-(const TriangularDenseStorage& lhs,
		                                               const TriangularDenseStorage& rhs)
		{
			TriangularDenseStorage result(lhs);
			result -= rhs;
			return result;
		}

		friend inline TriangularDenseStorage operator*(const TriangularDenseStorage& lhs,
		                                               const TriangularDenseStorage& rhs)
		{
			TriangularDenseStorage result(lhs);
			result *= rhs;
			return result;
		}

		friend inline TriangularDenseStorage operator/(const TriangularDenseStorage& lhs,
		                                               const TriangularDenseStorage& rhs)
		{
			TriangularDenseStorage result(lhs);
			result /= rhs;
			return result;
		}


	public:
		inline TScalar* Data()
		{
			return &(m_DataBuffer[0]);
		}

		inline const TScalar* Data() const
		{
			return &(m_DataBuffer[0]);
		}

		inline TScalar Max() const
		{
			return m_DataBuffer.max();
		}

		inline TScalar Min() const
		{
			return m_DataBuffer.min();
		}

		inline TScalar Sum() const
		{
			return m_DataBuffer.sum();
		}

		inline Iterator begin()
		{
			return &(m_DataBuffer[0]);
		}

		inline ConstIterator begin() const
		{
			return &(m_DataBuffer[0]);
		}

		inline Iterator end()
		{
			return &(m_DataBuffer[m_DataBuffer.size() - 1]) + 1;
		}

		inline ConstIterator end() const
		{
			return &(m_DataBuffer[m_DataBuffer.size() - 1]) + 1;
		}

		inline TScalar operator[](long index) const
		{
			return m_DataBuffer[index];
		}

		inline TScalar& operator[](long index)
		{
			return m_DataBuffer[index];
		}


	private:
		static inline constexpr long CalculateSizeOfDimension(long dim)
		{
			return dim * (dim + 1) / 2;
		}


	private:
		long m_Dimension = 0;
		std::valarray<TScalar> m_DataBuffer;
	};
}  // namespace Hoppy