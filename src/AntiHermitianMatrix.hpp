//
// Created by Andy on 9/2/2023.
//

#pragma once

#include "TriangularMatrixBase.hpp"

namespace Hoppy
{
	template <typename TScalar>
	class AntiHermitianMatrix : public TriangularMatrixBase<AntiHermitianMatrix<TScalar>>
	{
		using Base = TriangularMatrixBase<AntiHermitianMatrix<TScalar>>;
		friend Base;


	public:
		AntiHermitianMatrix() = default;

		explicit AntiHermitianMatrix(long dimension) : Base(dimension)
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
				return MathExt::Conjugate(this->m_DataStorage[j * (j + 1) / 2 + i]);
			}
		}

		inline void Set(long i, long j, TScalar value)
		{
			if (i > j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] = value;
			}
			else if (i == j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] = TScalar(MathExt::ImaginaryPart(value));
			}
			else
			{
				this->m_DataStorage[j * (j + 1) / 2 + i] = -MathExt::Conjugate(value);
			}
		}

		inline void AdditionAssignment(long i, long j, TScalar value)
		{
			if (i > j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] += value;
			}
			else if (i == j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] += TScalar(MathExt::ImaginaryPart(value));
			}
			else
			{
				this->m_DataStorage[j * (j + 1) / 2 + i] -= MathExt::Conjugate(value);
			}
		}

		inline void SubtractionAssignment(long i, long j, TScalar value)
		{
			if (i > j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] -= value;
			}
			else if (i == j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] -= TScalar(MathExt::ImaginaryPart(value));
			}
			else
			{
				this->m_DataStorage[j * (j + 1) / 2 + i] += MathExt::Conjugate(value);
			}
		}

		inline void MultiplicationAssignment(long i, long j, TScalar value)
		{
			if (i > j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] *= value;
			}
			else if (i == j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] *= TScalar(MathExt::RealPart(value));
			}
			else
			{
				this->m_DataStorage[j * (j + 1) / 2 + i] *= -MathExt::Conjugate(value);
			}
		}

		inline void DivisionAssignment(long i, long j, TScalar value)
		{
			if (i > j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] /= value;
			}
			else if (i == j)
			{
				this->m_DataStorage[i * (i + 1) / 2 + j] /= TScalar(MathExt::RealPart(value));
			}
			else
			{
				this->m_DataStorage[j * (j + 1) / 2 + i] /= -MathExt::Conjugate(value);
			}
		}
	};


	template <typename TScalar>
	struct traits<AntiHermitianMatrix<TScalar>>
	{
		using Scalar = TScalar;
	};
}  // namespace Hoppy