//
// Created by Andy on 2/13/2024.
//

#pragma once

#include "Regin.hpp"

namespace Hoppy
{
	template <typename Derived>
	class PlainTriangularCompressedObjectBase : public TriangularCompressedMatrixBase<Derived>
	{
	public:
		using Scalar = typename Eigen::internal::traits<Derived>::Scalar;
		using Base = TriangularCompressedMatrixBase<Derived>;
		using Base::FillWith;
		using Base::IsShapeAs;
		using Base::ResizeAs;
		using Base::operator();
		using Base::operator=;
		typedef const PlainTriangularCompressedObjectBase& Nested;
		static constexpr int DimensionAtCompileTime = Eigen::internal::traits<Derived>::DimensionAtCompileTime;


	public:
		PlainTriangularCompressedObjectBase() = default;

		explicit PlainTriangularCompressedObjectBase(const Eigen::Index dimension)
		{
			Resize(dimension);
			InitalizeMatrixElementsAccordingToMacro();
		}

		explicit PlainTriangularCompressedObjectBase(const Eigen::Index rowCount, const Eigen::Index columnCount)
		    : m_storage(RequiredBufferSizeOf(rowCount, columnCount), RequiredBufferSizeOf(rowCount, columnCount), 1),
		      m_dimension(rowCount)
		{
			Resize(rowCount, columnCount);
			InitalizeMatrixElementsAccordingToMacro();
		}

		explicit PlainTriangularCompressedObjectBase(
		        const Eigen::Transpose<typename Eigen::internal::eval<Derived>::type>& other)
		{
			// the transpose of my transpose is my-self
			ResizeAs(other);
			FillWith([&other](Eigen::Index i, Eigen::Index j) -> Scalar { return other(i, j); });
		}

		template <typename OtherDerived>
		explicit PlainTriangularCompressedObjectBase(const Eigen::MatrixBase<OtherDerived>& other)
		{
			ResizeAs(other);
			FillWith([&other](Eigen::Index i, Eigen::Index j) -> Scalar { return other(i, j); });
		}

		template <typename OtherDerived>
		explicit PlainTriangularCompressedObjectBase(const Eigen::EigenBase<OtherDerived>& other)
		{
			ResizeAs(other);
			const Eigen::MatrixX<typename Eigen::internal::traits<OtherDerived>::Scalar> temp = other;
			FillWith([&temp](Eigen::Index i, Eigen::Index j) -> Scalar { return temp(i, j); });
		}


		void Resize(const Eigen::Index dimension)
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
				throw std::runtime_error(
				        std::string("Triangular compressed matrices can only be square (can't be resized to ")
				        + std::to_string(rowCount) + " x " + std::to_string(columnCount) + " )");
			}

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

		[[nodiscard]] Eigen::Index Dimension() const
		{
			return m_dimension;
		}

		[[nodiscard]] const Scalar* data() const
		{
			return m_storage.data();
		}

		Scalar* data()
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
		// ReSharper disable once CppMemberFunctionMayBeStatic
		void InitalizeMatrixElementsAccordingToMacro()
		{
#if defined(EIGEN_INITIALIZE_MATRICES_BY_NAN)
			FillWithNan();
#if defined(EIGEN_INITIALIZE_MATRICES_BY_ZERO)
#warning Both EIGEN_INITIALIZE_MATRICES_BY_ZERO and EIGEN_INITIALIZE_MATRICES_BY_NAN are defined
#endif
#elif defined(EIGEN_INITIALIZE_MATRICES_BY_ZERO)
			FillWithZero();
#endif
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
