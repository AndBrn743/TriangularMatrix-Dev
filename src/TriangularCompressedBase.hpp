//
// Created by Andy on 2/13/2024.
//

#pragma once
#include "TriangularCompressedMatrices.hpp"

namespace Hoppy
{
	template <typename Derived>
	class TriangularCompressedBase : public TriangularCompressedCoeffsBase<Derived, Eigen::internal::accessors_level<Derived>::value>
	{
	public:
		using Base = TriangularCompressedCoeffsBase<Derived, Eigen::internal::accessors_level<Derived>::value>;
		using Scalar = typename Eigen::internal::traits<Derived>::Scalar;
		using RealScalar = typename Eigen::NumTraits<Scalar>::Real;
		using Base::cols;
		using Base::derived;
		using Base::rows;
		using Base::size;
		using Base::operator();
		using Nested = const TriangularCompressedBase&;

		static constexpr int Flags = Eigen::internal::traits<Derived>::Flags;
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

		using EvalReturnType =
		        typename Eigen::internal::add_const_on_value_type<typename Eigen::internal::eval<Derived>::type>::type;
		EvalReturnType eval() const
		{
			return static_cast<typename Eigen::internal::eval<Derived>::type>(derived());
		}

		friend std::ostream& operator<<(std::ostream& s, const TriangularCompressedBase& m)
		{
			return Eigen::internal::print_matrix(s, m.eval(), EIGEN_DEFAULT_IO_FORMAT);
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

		Derived& FillWithNan()
		{
			return FillWith(std::numeric_limits<Scalar>::quiet_NaN());
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


#define EIGEN_DOC_UNARY_ADDONS(X, Y)
#define internal Eigen::internal
#define NumTraits Eigen::NumTraits
#define CwiseUnaryOp Eigen::CwiseUnaryOp
#define CwiseUnaryView Eigen::CwiseUnaryView
#include <Eigen/src/plugins/CommonCwiseUnaryOps.h>
#undef internal
#undef NumTraits
#undef CwiseUnaryOp
#undef CwiseUnaryView
#undef EIGEN_DOC_UNARY_ADDONS

		template <typename OtherDerived>
		/* NOLINTNEXTLINE(*-unconventional-assign-operator) */
		Derived& operator=(const Eigen::EigenBase<OtherDerived>& rhs)
		{
#if defined(EIGEN_NO_AUTOMATIC_RESIZING)
			if (!IsShapeAs(rhs))
			{
				throw std::runtime_error(
				        "Explicit resize operation is required as EIGEN_NO_AUTOMATIC_RESIZING was defined");
			}
#else
			ResizeAs(rhs);
#endif
			return FillWith([&rhs](Eigen::Index i, Eigen::Index j) -> Scalar { return rhs.derived()(i, j); });
		}

		template <typename OtherDerived>
		/* NOLINTNEXTLINE(*-unconventional-assign-operator) */
		Derived& operator+=(const Eigen::EigenBase<OtherDerived>& rhs)
		{
			if (!IsShapeAs(rhs))
			{
				throw std::runtime_error("Operation cannot be performed for dimension mismatch(es)");
			}

			return IndependentCwiseOp([this, &rhs](Eigen::Index i, Eigen::Index j)
			                          { (*this)(i, j) += rhs.derived()(i, j); });
		}

		template <typename OtherDerived>
		/* NOLINTNEXTLINE(*-unconventional-assign-operator) */
		Derived& operator-=(const Eigen::EigenBase<OtherDerived>& rhs)
		{
			if (!IsShapeAs(rhs))
			{
				throw std::runtime_error("Operation cannot be performed for dimension mismatch(es)");
			}

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
			Eigen::MatrixX<Scalar> result(derived().rows(), derived().cols());
			CwiseOp([&result, this](Eigen::Index i, Eigen::Index j) { result(i, j) = (*this)(i, j); });
			return result;
		}

		Eigen::MatrixX<Scalar> ExtractLowerToFullMatrix() const
		{
			Eigen::MatrixX<Scalar> result = Eigen::MatrixX<Scalar>::Zero(derived().rows(), derived().cols());
			LowerCwiseOp([&result, this](Eigen::Index i, Eigen::Index j) { result(i, j) = (*this)(i, j); });
			return result;
		}

		Eigen::MatrixX<Scalar> ExtractUpperToFullMatrix() const
		{
			Eigen::MatrixX<Scalar> result = Eigen::MatrixX<Scalar>::Zero(derived().rows(), derived().cols());
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


	protected:
		template <typename... Args>
		explicit TriangularCompressedBase(Args&&...)
		{
			/* NO CODE. Added only to treat invalid upcasting operation as compile error. */
		}
	};

}  // namespace Hoppy


namespace Eigen
{
	namespace internal
	{
		template <typename T>
		struct plain_matrix_type<T, Hoppy::TriangularCompressed>
		{
		private:
			using Traits = traits<T>;
			using SpecficXprKind = typename Traits::SpecificXprKind;
			using Scalar = typename Traits::Scalar;
			static constexpr int Dimension = Traits::DimensionAtCompileTime;
			static constexpr int Options = Traits::Option;

		public:
			// clang-format off
			using type =
				typename std::conditional<std::is_same<SpecficXprKind, HermitianMatrixXpr>::value, Hoppy::HermitianMatrix<Scalar, Dimension, Options>,
				typename std::conditional<std::is_same<SpecficXprKind, AntiHermitianMatrixXpr>::value, Hoppy::AntiHermitianMatrix<Scalar, Dimension, Options>,
				typename std::conditional<std::is_same<SpecficXprKind, SymmetricMatrixXpr>::value, Hoppy::SymmetricMatrix<Scalar, Dimension, Options>,
				typename std::conditional<std::is_same<SpecficXprKind, AntiSymmetricMatrixXpr>::value, Hoppy::AntiSymmetricMatrix<Scalar, Dimension, Options>,
				typename std::conditional<std::is_same<SpecficXprKind, UpperTriangularMatrixXpr>::value, Hoppy::UpperTriangularMatrix<Scalar, Dimension, Options>,
				typename std::conditional<std::is_same<SpecficXprKind, LowerTriangularMatrixXpr>::value, Hoppy::LowerTriangularMatrix<Scalar, Dimension, Options>,
				void>::type>::type>::type>::type>::type>::type;
			// clang-format on
		};
	}  // namespace internal
}  // namespace Eigen