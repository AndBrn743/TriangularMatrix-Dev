//
// Created by Andy on 2/13/2024.
//

#pragma once

namespace Hoppy
{
	template <typename Derived>
	struct accessors_level
	{
		enum
		{
			// has_direct_access = (traits<Derived>::Flags & Eigen::DirectAccessBit) ? 1 : 0,
			// has_write_access = (traits<Derived>::Flags & Eigen::LvalueBit) ? 1 : 0,
			// value = has_direct_access ? (has_write_access ? Eigen::DirectWriteAccessors : Eigen::DirectAccessors)
			//                           : (has_write_access ? Eigen::WriteAccessors : Eigen::ReadOnlyAccessors)
			has_direct_access = 0,
			has_write_access = 1,
			value = has_direct_access ? (has_write_access ? Eigen::DirectWriteAccessors : Eigen::DirectAccessors)
			                          : (has_write_access ? Eigen::WriteAccessors : Eigen::ReadOnlyAccessors)
		};
	};

	template <typename Derived>
	class TriangularBase : public TriangularCoeffsBase<Derived, accessors_level<Derived>::value>
	{
	public:
		using Base = TriangularCoeffsBase<Derived, accessors_level<Derived>::value>;
		using Scalar = typename Eigen::internal::traits<Derived>::Scalar;
		using RealScalar = typename Eigen::NumTraits<Scalar>::Real;
		using Base::cols;
		using Base::derived;
		using Base::rows;
		using Base::size;
		using Base::operator();
		typedef const TriangularBase& Nested;

		static constexpr int SizeAtCompileTime =
		        Eigen::internal::traits<Derived>::RowsAtCompileTime != Eigen::Dynamic
		                        && Eigen::internal::traits<Derived>::ColsAtCompileTime != Eigen::Dynamic
		                ? Eigen::internal::traits<Derived>::RowsAtCompileTime
		                          * Eigen::internal::traits<Derived>::ColsAtCompileTime
		                : Eigen::Dynamic;
		static constexpr int MaxSizeAtCompileTime = SizeAtCompileTime;
		static constexpr bool IsVectorAtCompileTime = Eigen::internal::traits<Derived>::RowsAtCompileTime == 1
		                                              || Eigen::internal::traits<Derived>::ColsAtCompileTime == 1;
		using PlainObject = Eigen::Matrix<typename Eigen::internal::traits<Derived>::Scalar,
		                                  Eigen::internal::traits<Derived>::RowsAtCompileTime,
		                                  Eigen::internal::traits<Derived>::ColsAtCompileTime,
		                                  Eigen::AutoAlign
		                                          | (Eigen::internal::traits<Derived>::Flags & Eigen::RowMajorBit
		                                                     ? Eigen::RowMajor
		                                                     : Eigen::ColMajor),
		                                  Eigen::internal::traits<Derived>::MaxRowsAtCompileTime,
		                                  Eigen::internal::traits<Derived>::MaxColsAtCompileTime>;


		// NOTE: The method is required by operator <<
		EIGEN_DEVICE_FUNC auto coeff(const Eigen::Index i, const Eigen::Index j) const
		{
			return (*this)(i, j);
		}

		friend std::ostream& operator<<(std::ostream& s, const TriangularBase& m)
		{
			return Eigen::internal::print_matrix(s, m, EIGEN_DEFAULT_IO_FORMAT);
		}

		template <typename OtherDerived>
		constexpr bool IsShapeAs(const Eigen::EigenBase<OtherDerived>& other)
		{
			return rows() == other.rows() && cols() == other.cols();
		}

	public:
		Derived& FillWithOne()
		{
			return FillWith(1);
		}

		Derived& FillWithZero()
		{
			return FillWith(0);
		}

		Derived& FillWithRandom()
		{
			return FillWith([](Eigen::Index, Eigen::Index) -> Scalar { return Eigen::internal::random<Scalar>(); });
		}

		Derived& FillWith(const Scalar s)
		{
			return FillWith([&](Eigen::Index, Eigen::Index) { return s; });
		}

		Derived& FillWith(const std::function<Scalar(Eigen::Index, Eigen::Index)>& functor)
		{
			IndependentCwiseOp([this, &functor](Eigen::Index i, Eigen::Index j) { (*this)(i, j) = functor(i, j); });
			return derived();
		}

		const Derived& CwiseOp(const std::function<void(Eigen::Index, Eigen::Index)>& functor)
		{
			for (Eigen::Index i = 0; i < rows(); i++)
			{
				for (Eigen::Index j = 0; j < cols(); j++)
				{
					functor(i, j);
				}
			}

			return derived();
		}

		const Derived& CwiseOp(const std::function<void(Eigen::Index, Eigen::Index)>& functor) const
		{
			for (Eigen::Index i = 0; i < rows(); i++)
			{
				for (Eigen::Index j = 0; j < cols(); j++)
				{
					functor(i, j);
				}
			}

			return derived();
		}

		Derived& UpperCwiseOp(const std::function<void(Eigen::Index, Eigen::Index)>& functor)
		{
			for (Eigen::Index i = 0; i < rows(); i++)
			{
				for (Eigen::Index j = i; j < cols(); j++)
				{
					functor(i, j);
				}
			}

			return derived();
		}

		Derived& LowerCwiseOp(const std::function<void(Eigen::Index, Eigen::Index)>& functor)
		{
			for (Eigen::Index i = 0; i < rows(); i++)
			{
				for (Eigen::Index j = 0; j <= i; j++)
				{
					functor(i, j);
				}
			}

			return derived();
		}

		Derived& IndependentCwiseOp(const std::function<void(Eigen::Index, Eigen::Index)>& functor)
		{
			if (Eigen::internal::traits<Derived>::IsUpperCritical)
			{
				UpperCwiseOp(functor);
			}
			else
			{
				LowerCwiseOp(functor);
			}

			return derived();
		}

		template <typename OtherDerived>
		/* NOLINTNEXTLINE(*-unconventional-assign-operator) */
		Derived& operator=(const Eigen::EigenBase<OtherDerived>& rhs)
		{
			derived().Resize(rhs.RowCount(), rhs.ColumnCount());
			return FillWith([&rhs](Eigen::Index i, Eigen::Index j) -> Scalar { return rhs.derived()(i, j); });
		}

		template <typename OtherDerived>
		/* NOLINTNEXTLINE(*-unconventional-assign-operator) */
		Derived& operator+=(const Eigen::EigenBase<OtherDerived>& rhs)
		{
			derived().Resize(rhs.RowCount(), rhs.ColumnCount());
			return IndependentCwiseOp([this, &rhs](Eigen::Index i, Eigen::Index j)
			                          { (*this)(i, j) += rhs.derived()(i, j); });
		}

		template <typename OtherDerived>
		/* NOLINTNEXTLINE(*-unconventional-assign-operator) */
		Derived& operator-=(const Eigen::EigenBase<OtherDerived>& rhs)
		{
			derived().Resize(rhs.RowCount(), rhs.ColumnCount());
			return IndependentCwiseOp([this, &rhs](Eigen::Index i, Eigen::Index j)
			                          { (*this)(i, j) -= rhs.derived()(i, j); });
		}

		/* NOLINTNEXTLINE(*-unconventional-assign-operator) */
		Derived& operator*=(const Scalar s)
		{
			return IndependentCwiseOp([this, &s](Eigen::Index i, Eigen::Index j) { (*this)(i, j) *= s; });
		}

		/* NOLINTNEXTLINE(*-unconventional-assign-operator) */
		Derived& operator/=(const Scalar& s)
		{
			return IndependentCwiseOp([this, &s](Eigen::Index i, Eigen::Index j) { (*this)(i, j) /= s; });
		}

		Eigen::MatrixX<Scalar> ToFullMatrix() const
		{
			Eigen::MatrixX<Scalar> result(derived().Dimension(), derived().Dimension());
			CwiseOp([&result, this](Eigen::Index i, Eigen::Index j) { result(i, j) = (*this)(i, j); });
			return result;
		}

		Eigen::MatrixX<Scalar> ExtractLowerToFullMatrix() const
		{
			Eigen::MatrixX<Scalar> result = Eigen::MatrixX<Scalar>::Zero(derived().Dimension(), derived().Dimension());
			LowerCwiseOp([&result, this](Eigen::Index i, Eigen::Index j) { result(i, j) = (*this)(i, j); });
			return result;
		}

		Eigen::MatrixX<Scalar> ExtractUpperToFullMatrix() const
		{
			Eigen::MatrixX<Scalar> result = Eigen::MatrixX<Scalar>::Zero(derived().Dimension(), derived().Dimension());
			UpperCwiseOp([&result, this](Eigen::Index i, Eigen::Index j) { result(i, j) = (*this)(i, j); });
			return result;
		}

		void Resize(const Eigen::Index size)
		{
			Resize(size, size);
		}

		void Resize(Eigen::Index rowCount, Eigen::Index columnCount)
		{
			if (rowCount != derived().rows() || columnCount != derived().cols())
			{
				throw std::runtime_error("TriangularBase::Resize(...) does not allow resize");
			}
		}

		template <typename OtherDerived>
		void ResizeAs(const Eigen::EigenBase<OtherDerived>& other)
		{
			derived().Resize(other.rows(), other.cols());
		}
	};

}  // namespace Hoppy
