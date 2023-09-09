//
// Created by Andy on 9/2/2023.
//

#pragma once

#include "TriangularMatrixBase.hpp"

namespace Hoppy
{
	template <typename TScalar>
	class LowerTriangularMatrix : public TriangularMatrixBase<LowerTriangularMatrix<TScalar>>
	{
		using Base = TriangularMatrixBase<LowerTriangularMatrix<TScalar>>;
		friend Base;


	public:
		LowerTriangularMatrix() = default;

		explicit LowerTriangularMatrix(long dimension) : Base(dimension)
		{
			/* NO CODE */
		}


	private:
		inline TScalar Get(long i, long j) const
		{
			if (i >= j)
			{
				return this->m_DataStorage[i * (i + 1) / 2 + j];
			}
			else
			{
				return TScalar(0);
			}
		}

		inline void Set(long i, long j, TScalar value)
		{
			if (i >= j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] = value;
			}
		}

		inline void AdditionAssignment(long i, long j, TScalar value)
		{
			if (i >= j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] += value;
			}
		}

		inline void SubtractionAssignment(long i, long j, TScalar value)
		{
			if (i >= j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] -= value;
			}
		}

		inline void MultiplicationAssignment(long i, long j, TScalar value)
		{
			if (i >= j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] *= value;
			}
		}

		inline void DivisionAssignment(long i, long j, TScalar value)
		{
			if (i >= j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] /= value;
			}
		}
	};


	template <typename TScalar>
	struct traits<LowerTriangularMatrix<TScalar>>
	{
		using Scalar = TScalar;
	};
}  // namespace Hoppy
