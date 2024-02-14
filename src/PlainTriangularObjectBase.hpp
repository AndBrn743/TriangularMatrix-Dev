//
// Created by Andy on 2/13/2024.
//

#pragma once

#include "Regin.hpp"

namespace Hoppy
{
	template <typename Derived>
	class PlainTriangularObjectBase : public TriangularMatrixBase<Derived>
	{
	public:
		using Scalar = typename Eigen::internal::traits<Derived>::Scalar;
		using Base = TriangularMatrixBase<Derived>;
		using Base::IsShapeAs;
		// using Base::IsSquare;
		// using Base::IsVector;
		using Base::operator();
		using Base::operator=;
		// friend PlainSquareObjectBase<Base>;
		typedef const PlainTriangularObjectBase& Nested;
		static constexpr int DimensionAtCompileTime = Eigen::internal::traits<Derived>::DimensionAtCompileTime;


	public:
		PlainTriangularObjectBase() = default;

		explicit PlainTriangularObjectBase(Eigen::Index dimension)
		    : m_storage(RequiredBufferSizeOf(dimension), RequiredBufferSizeOf(dimension), 1), m_dimension(dimension)
		{
			/* NO CODE */
		}

		explicit PlainTriangularObjectBase(const Eigen::Index rowCount, const Eigen::Index columnCount)
		    : m_storage(RequiredBufferSizeOf(rowCount, columnCount), RequiredBufferSizeOf(rowCount, columnCount), 1),
		      m_dimension(rowCount)
		{
			assert(rowCount == columnCount && "vwbdcgsj");
		}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"
		void Resize(const Index dimension)
		{
			if (dimension != m_dimension)
			{
				m_storage.resize(RequiredBufferSizeOf(dimension), RequiredBufferSizeOf(dimension), 1);
				m_dimension.setValue(dimension);
			}
		}

		void Resize(const Eigen::Index rowCount, const Eigen::Index columnCount)
		{
			if (rowCount != columnCount)
			{
				std::cout << "fewvdkjsvszgu: ( " << rowCount << " != " << columnCount << " )" << std::endl;
			}
			assert(rowCount == columnCount && "fewvdkjsvszgu");
			Resize(rowCount);
		}

		[[nodiscard]] Eigen::Index rows() const
		{
			return m_dimension;
		}

		[[nodiscard]] Eigen::Index cols() const
		{
			return m_dimension;
		}

		[[nodiscard]] Eigen::Index BufferSize() const
		{
			return RequiredBufferSizeOf(m_storage.rows(), m_storage.cols());
		}
#pragma clang diagnostic pop

		[[nodiscard]] Eigen::Index Dimension() const
		{
			return m_dimension;
		}

		[[nodiscard]] const Scalar* Data() const
		{
			return m_storage.data();
		}

		Scalar* Data()
		{
			return m_storage.data();
		}

		static Eigen::Index RequiredBufferSizeOf(const Eigen::Index rowCount, const Eigen::Index columnCount)
		{
			assert(rowCount == columnCount && "vdsbuivjdZUYGDScefjabwu");
			return RequiredBufferSizeOf(rowCount);
		}

		static Eigen::Index RequiredBufferSizeOf(const Eigen::Index dimension)
		{
			return Regin1<2>(dimension);
		}

	private:
		Eigen::DenseStorage<Scalar,
		                    Regin1<DimensionAtCompileTime, DimensionAtCompileTime>(),
		                    Regin1<DimensionAtCompileTime, DimensionAtCompileTime>(),
		                    1,
		                    EIGEN_DEFAULT_MATRIX_STORAGE_ORDER_OPTION>
		        m_storage;
		Eigen::internal::variable_if_dynamic<Eigen::Index, DimensionAtCompileTime> m_dimension;
	};
}  // namespace Hoppy
