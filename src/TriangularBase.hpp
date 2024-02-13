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
		using Scalar = typename internal::traits<Derived>::Scalar;
		using RealScalar = typename NumTraits<Scalar>::Real;
		using Base::cols;
		using Base::derived;
		using Base::rows;
		using Base::size;
		using Base::operator();
		typedef const TriangularBase& Nested;

		// NOTE: The method is required by operator <<
		EIGEN_DEVICE_FUNC auto coeff(const Index i, const Index j) const
		{
			return (*this)(i, j);
		}

		friend std::ostream& operator<<(std::ostream& s, const TriangularBase& m)
		{
			return internal::print_matrix(s, m, EIGEN_DEFAULT_IO_FORMAT);
		}

		template <typename OtherDerived>
		constexpr bool IsShapeAs(const EigenBase<OtherDerived>& other)
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
			return FillWith([](Index, Index) -> Scalar { return internal::random<Scalar>(); });
		}

		Derived& FillWith(const Scalar s)
		{
			return FillWith([&](Index, Index) { return s; });
		}

		Derived& FillWith(const std::function<Scalar(Index, Index)>& functor)
		{
			IndependentCwiseOp([this, &functor](Index i, Index j) { (*this)(i, j) = functor(i, j); });
			return derived();
		}

		const Derived& CwiseOp(const std::function<void(Index, Index)>& functor)
		{
			for (Index i = 0; i < rows(); i++)
			{
				for (Index j = 0; j < cols(); j++)
				{
					functor(i, j);
				}
			}

			return derived();
		}

		const Derived& CwiseOp(const std::function<void(Index, Index)>& functor) const
		{
			for (Index i = 0; i < rows(); i++)
			{
				for (Index j = 0; j < cols(); j++)
				{
					functor(i, j);
				}
			}

			return derived();
		}

		Derived& UpperCwiseOp(const std::function<void(Index, Index)>& functor)
		{
			for (Index i = 0; i < rows(); i++)
			{
				for (Index j = i; j < cols(); j++)
				{
					functor(i, j);
				}
			}

			return derived();
		}

		Derived& LowerCwiseOp(const std::function<void(Index, Index)>& functor)
		{
			for (Index i = 0; i < rows(); i++)
			{
				for (Index j = 0; j <= i; j++)
				{
					functor(i, j);
				}
			}

			return derived();
		}

		Derived& IndependentCwiseOp(const std::function<void(Index, Index)>& functor)
		{
			if (internal::traits<Derived>::IsUpperCritical)
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
		Derived& operator=(const EigenBase<OtherDerived>& rhs)
		{
			derived().Resize(rhs.RowCount(), rhs.ColumnCount());
			return FillWith([&rhs](Index i, Index j) -> Scalar { return rhs.AsDerived()(i, j); });
		}

		template <typename OtherDerived>
		/* NOLINTNEXTLINE(*-unconventional-assign-operator) */
		Derived& operator+=(const EigenBase<OtherDerived>& rhs)
		{
			derived().Resize(rhs.RowCount(), rhs.ColumnCount());
			return IndependentCwiseOp([this, &rhs](Index i, Index j) { (*this)(i, j) += rhs.AsDerived()(i, j); });
		}

		template <typename OtherDerived>
		/* NOLINTNEXTLINE(*-unconventional-assign-operator) */
		Derived& operator-=(const EigenBase<OtherDerived>& rhs)
		{
			derived().Resize(rhs.RowCount(), rhs.ColumnCount());
			return IndependentCwiseOp([this, &rhs](Index i, Index j) { (*this)(i, j) -= rhs.AsDerived()(i, j); });
		}

		/* NOLINTNEXTLINE(*-unconventional-assign-operator) */
		Derived& operator*=(const Scalar s)
		{
			return IndependentCwiseOp([this, &s](Index i, Index j) { (*this)(i, j) *= s; });
		}

		/* NOLINTNEXTLINE(*-unconventional-assign-operator) */
		Derived& operator/=(const Scalar& s)
		{
			return IndependentCwiseOp([this, &s](Index i, Index j) { (*this)(i, j) /= s; });
		}

		Eigen::MatrixX<Scalar> ToFullMatrix() const
		{
			Eigen::MatrixX<Scalar> result(derived().Dimension(), derived().Dimension());
			CwiseOp([&result, this](Index i, Index j) { result(i, j) = (*this)(i, j); });
			return result;
		}

		Eigen::MatrixX<Scalar> ExtractLowerToFullMatrix() const
		{
			Eigen::MatrixX<Scalar> result = Eigen::MatrixX<Scalar>::Zero(derived().Dimension(), derived().Dimension());
			LowerCwiseOp([&result, this](Index i, Index j) { result(i, j) = (*this)(i, j); });
			return result;
		}

		Eigen::MatrixX<Scalar> ExtractUpperToFullMatrix() const
		{
			Eigen::MatrixX<Scalar> result = Eigen::MatrixX<Scalar>::Zero(derived().Dimension(), derived().Dimension());
			UpperCwiseOp([&result, this](Index i, Index j) { result(i, j) = (*this)(i, j); });
			return result;
		}

		Eigen::MatrixX<Scalar> Inverse() const
		{
			return ToFullMatrix().inverse();
		}

		template <typename OtherDerived>
		friend Eigen::MatrixX<Scalar> operator*(const TriangularBase& m, const EigenBase<OtherDerived>& n)
		{
			return m.ToFullMatrix() * n;
		}

		template <typename OtherDerived>
		friend Eigen::MatrixX<Scalar> operator*(const EigenBase<OtherDerived>& n, const TriangularBase& m)
		{
			return n * m.ToFullMatrix();
		}

		template <typename OtherDerived>
		Derived TransformedBy(const EigenBase<OtherDerived>& n) const
		{
			auto copy = ToFullMatrix();
			return n.derived().conjugate().transpose() * copy * n.derived();
		}

		template <typename OtherDerived>
		Derived& TransformBy(const EigenBase<OtherDerived>& n)
		{
			return TransformedBy(n);
		}

		template <typename OtherDerived>
		Derived InverseTransformedBy(const EigenBase<OtherDerived>& n) const
		{
			auto copy = ToFullMatrix();
			return n.derived() * copy * n.derived().conjugate().transpose();
		}

		template <typename OtherDerived>
		Derived& InverseTransformBy(const EigenBase<OtherDerived>& n)
		{
			return InverseTransformedBy(n);
		}

	public:
		void Resize(const Index size)
		{
			Resize(size, 1);
		}

		void Resize(Index rowCount, Index columnCount)
		{
			if (rowCount != derived().RowCount() || columnCount != derived().ColumnCount())
			{
				throw std::runtime_error("DenseBase::Resize(...) does not allow resize");
			}
		}
	};

}  // namespace Hoppy
