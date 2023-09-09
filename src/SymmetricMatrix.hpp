//
// Created by Andy on 9/2/2023.
//

#pragma once

#include "TriangularMatrixBase.hpp"

namespace Hoppy
{
	template <typename TScalar>
	class SymmetricMatrix : public TriangularMatrixBase<SymmetricMatrix<TScalar>>
	{
		using Base = TriangularMatrixBase<SymmetricMatrix<TScalar>>;
		friend Base;


	public:
		SymmetricMatrix() = default;

		explicit SymmetricMatrix(long dimension) : Base(dimension)
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
				return this->m_DataStorage[j * (j + 1) / 2 + i];
			}
		}

		inline void Set(long i, long j, TScalar value)
		{
			if (i > j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] = value;
			}
			else
			{
				this->m_DataStorage[j * (j + 1) / 2 + i] = value;
			}
		}

		inline void AdditionAssignment(long i, long j, TScalar value)
		{
			if (i > j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] += value;
			}
			else
			{
				this->m_DataStorage[j * (j + 1) / 2 + i] += value;
			}
		}

		inline void SubtractionAssignment(long i, long j, TScalar value)
		{
			if (i > j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] -= value;
			}
			else
			{
				this->m_DataStorage[j * (j + 1) / 2 + i] -= value;
			}
		}

		inline void MultiplicationAssignment(long i, long j, TScalar value)
		{
			if (i > j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] *= value;
			}
			else
			{
				this->m_DataStorage[j * (j + 1) / 2 + i] *= value;
			}
		}

		inline void DivisionAssignment(long i, long j, TScalar value)
		{
			if (i > j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] /= value;
			}
			else
			{
				this->m_DataStorage[j * (j + 1) / 2 + i] /= value;
			}
		}
	};


	template <typename TScalar>
	struct traits<SymmetricMatrix<TScalar>>
	{
		using Scalar = TScalar;
		static constexpr const bool is_upper_critical = false;
	};
}  // namespace Hoppy
