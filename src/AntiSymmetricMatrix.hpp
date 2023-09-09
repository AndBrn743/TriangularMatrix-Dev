//
// Created by Andy on 9/2/2023.
//

#pragma once

#include "TriangularMatrixBase.hpp"

namespace Hoppy
{
	template <typename TScalar>
	class AntiSymmetricMatrix : public TriangularMatrixBase<AntiSymmetricMatrix<TScalar>>
	{
		using Base = TriangularMatrixBase<AntiSymmetricMatrix<TScalar>>;
		friend Base;


	public:
		AntiSymmetricMatrix() = default;

		explicit AntiSymmetricMatrix(long dimension) : Base(dimension)
		{
			for (long i = 0; i < this->Dimension(); i++)
			{
				this->Set(i, i, TScalar(0));
			}
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
				return -this->m_DataStorage[j * (j + 1) / 2 + i];
			}
		}

		inline void Set(long i, long j, TScalar value)
		{
			if (i > j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] = value;
			}
			else if (i < j)
			{
				this->m_DataStorage[j * (j + 1) / 2 + i] = -value;
			}
		}

		inline void AdditionAssignment(long i, long j, TScalar value)
		{
			if (i > j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] += value;
			}
			else if (i < j)

			{
				this->m_DataStorage[j * (j + 1) / 2 + i] -= value;
			}
		}

		inline void SubtractionAssignment(long i, long j, TScalar value)
		{
			if (i > j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] -= value;
			}
			else if (i < j)
			{
				this->m_DataStorage[j * (j + 1) / 2 + i] += value;
			}
		}

		inline void MultiplicationAssignment(long i, long j, TScalar value)
		{
			if (i > j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] *= value;
			}
			else if (i < j)
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
			else if (i < j)
			{
				this->m_DataStorage[j * (j + 1) / 2 + i] /= value;
			}
		}
	};


	template <typename TScalar>
	struct traits<AntiSymmetricMatrix<TScalar>>
	{
		using Scalar = TScalar;
	};
}  // namespace Hoppy
