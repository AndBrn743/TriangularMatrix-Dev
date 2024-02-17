//
// Created by Andy on 2/13/2024.
//

#pragma once

#include "MathExt.hpp"
#include "TriangularCompressedMatrices.hpp"
#include <Eigen/Dense>
#include <Eigen/Eigen>

namespace Hoppy
{
	template <typename Derived>
	class TriangularCompressedMatrixBase : public TriangularCompressedBase<Derived>
	{
	public:
		using Scalar = typename Eigen::internal::traits<Derived>::Scalar;
		using Base = TriangularCompressedBase<Derived>;
		using Base::derived;
		using Base::FillWith;
		using Base::IsShapeAs;
		using Base::ToFullMatrix;
		using Base::operator();
		using Base::operator=;

		using CoeffReturnType = Scalar;
		using PlainObject = typename Base::PlainObject;


		template <typename OtherDerived>
		friend Eigen::Product<Derived, OtherDerived, Eigen::AliasFreeProduct> operator*(
		        const TriangularCompressedMatrixBase& lhs, const Eigen::EigenBase<OtherDerived>& rhs)
		{
			return {lhs.derived(), rhs.derived()};
		}

		template <typename OtherDerived>
		friend Eigen::Product<OtherDerived, Derived, Eigen::AliasFreeProduct> operator*(
		        const Eigen::EigenBase<OtherDerived>& lhs, const TriangularCompressedMatrixBase& rhs)
		{
			return {lhs.derived(), rhs.derived()};
		}

		template <typename OtherDerived>
		friend Eigen::Product<Derived, OtherDerived, Eigen::AliasFreeProduct> operator*(
		        const TriangularCompressedMatrixBase& lhs, const TriangularCompressedMatrixBase<OtherDerived>& rhs)
		{
			return {lhs.derived(), rhs.derived()};
		}

		Eigen::MatrixX<Scalar> Inverse() const
		{
			return ToFullMatrix().inverse();
		}

		template <typename OutScalar = Scalar>
		Eigen::MatrixX<OutScalar> Sqrt() const
		{
			StaticAssertForHermitians();

			const Eigen::SelfAdjointEigenSolver<decltype(ToFullMatrix())> solver(ToFullMatrix());
			const Eigen::MatrixX<Scalar>& vectors = solver.eigenvectors();
			Eigen::VectorX<OutScalar> values = solver.eigenvalues();
			std::cout << "[ LINE " << __LINE__ << " ]:\n" << values.transpose() << std::endl;
			std::for_each(values.begin(), values.end(), [](auto& v) -> void { v = std::sqrt(v); });
			std::cout << "[ LINE " << __LINE__ << " ]:\n" << values.transpose() << std::endl;
			return vectors * values.asDiagonal() * vectors.conjugate().transpose();
		}

		Eigen::MatrixX<Scalar> InverseSqrt() const
		{
			StaticAssertForHermitians();

			const Eigen::EigenSolver<decltype(ToFullMatrix())> solver(ToFullMatrix());
			const Eigen::MatrixX<Scalar>& vectors = solver.eigenvectors();
			auto values = solver.eigenvalues();
			std::for_each(values.begin(), values.end(), [](auto& v) -> void { v = 1.0 / std::sqrt(v); });
			return vectors * values.asDiagonal() * vectors.conjugate().transpose();
		}

		template <typename OtherDerived>
		Derived TransformedBy(const Eigen::EigenBase<OtherDerived>& n) const
		{
			auto copy = ToFullMatrix();
			return Derived{Eigen::MatrixX<Scalar>{n.derived().conjugate().transpose() * copy * n.derived()}};
		}

		template <typename OtherDerived>
		const Derived& TransformBy(const Eigen::EigenBase<OtherDerived>& n)
		{
			*this = TransformedBy(n);
			return *this;
		}

		template <typename OtherDerived>
		Derived BackTransformedBy(const Eigen::EigenBase<OtherDerived>& n) const
		{
			auto copy = ToFullMatrix();
			return Derived{Eigen::MatrixX<Scalar>{n.derived() * copy * n.derived().conjugate().transpose()}};
		}

		template <typename OtherDerived>
		const Derived& BackTransformBy(const Eigen::EigenBase<OtherDerived>& n)
		{
			*this = BackTransformedBy(n);
			return *this;
		}

		static constexpr bool IsCompileTimeHermitian()
		{
			return std::is_same<
			               Eigen::EigenBase<Derived>,
			               Eigen::EigenBase<HermitianMatrix<Scalar,
			                                                Eigen::internal::traits<Derived>::DimensionAtCompileTime,
			                                                Eigen::internal::traits<Derived>::Option>>>::value
			       || (std::is_same<Eigen::EigenBase<Derived>,
			                        Eigen::EigenBase<
			                                SymmetricMatrix<Scalar,
			                                                Eigen::internal::traits<Derived>::DimensionAtCompileTime,
			                                                Eigen::internal::traits<Derived>::Option>>>::value
			           && std::is_same<Scalar, decltype(Eigen::numext::real(Scalar(0)))>::value);
		}


		// TODO:
		//       - EVD
		//       - SVD
		//       - etc.

	protected:
		template <typename... Args>
		explicit TriangularCompressedMatrixBase(Args&&...)
		{
			/* NO CODE. Added only to treat invalid upcasting operation as compile error. */
		}


	private:
		static constexpr void StaticAssertForHermitians()
		{
			static_assert(IsCompileTimeHermitian(), "Method is for Hermitian (including real symmetric) matrices only");
		}
	};
}  // namespace Hoppy


namespace Eigen
{
	namespace internal
	{

		template <typename Lhs, typename Rhs>
		struct generic_product_impl<
		        Lhs,
		        Rhs,
		        Hoppy::TriangularCompressedShape,
		        // typename storage_kind_to_shape<typename traits<Rhs>::StorageKind>::Shape,
		        typename std::enable_if<
		                !std::is_same<typename storage_kind_to_shape<typename traits<Rhs>::StorageKind>::Shape,
		                              Hoppy::TriangularCompressedShape>::value,
		                typename storage_kind_to_shape<typename traits<Rhs>::StorageKind>::Shape>::type,
		        GemmProduct>  // GEMM stands for generic matrix-matrix
		    : generic_product_impl_base<Lhs, Rhs, generic_product_impl<Lhs, Rhs>>
		{
			typedef typename Product<Lhs, Rhs>::Scalar Scalar;

			template <typename Dest>
			static void scaleAndAddTo(Dest& dst, const Lhs& lhs, const Rhs& rhs, const Scalar& alpha)
			{
				dst.noalias() += alpha * lhs.ToFullMatrix() * rhs;
			}
		};


		template <typename Lhs, typename Rhs>
		struct generic_product_impl<
		        Lhs,
		        Rhs,
		        typename std::enable_if<
		                !std::is_same<typename storage_kind_to_shape<typename traits<Lhs>::StorageKind>::Shape,
		                              Hoppy::TriangularCompressedShape>::value,
		                typename storage_kind_to_shape<typename traits<Lhs>::StorageKind>::Shape>::type,
		        Hoppy::TriangularCompressedShape,
		        GemmProduct>  // GEMM stands for generic matrix-matrix
		    : generic_product_impl_base<Lhs, Rhs, generic_product_impl<Lhs, Rhs>>
		{
			typedef typename Product<Lhs, Rhs>::Scalar Scalar;

			template <typename Dest>
			static void scaleAndAddTo(Dest& dst, const Lhs& lhs, const Rhs& rhs, const Scalar& alpha)
			{
				dst.noalias() += alpha * lhs * rhs.ToFullMatrix();
			}
		};


		template <typename Derived, typename XprKind>
		struct generic_xpr_base<Derived, XprKind, Hoppy::TriangularCompressed>
		{
			typedef Hoppy::TriangularCompressedMatrixBase<Derived> type;
		};


		template <typename Lhs, typename Rhs>
		struct generic_product_impl<Lhs,
		                            Rhs,
		                            Hoppy::TriangularCompressedShape,
		                            Hoppy::TriangularCompressedShape,
		                            GemmProduct>  // GEMM stands for generic matrix-matrix
		    : generic_product_impl_base<Lhs, Rhs, generic_product_impl<Lhs, Rhs>>
		{
			typedef typename Product<Lhs, Rhs>::Scalar Scalar;

			template <typename Dest>
			static void scaleAndAddTo(Dest& dst, const Lhs& lhs, const Rhs& rhs, const Scalar& alpha)
			{
				dst.noalias() += alpha * lhs.ToFullMatrix() * rhs.ToFullMatrix();
			}
		};

	}  // namespace internal
}  // namespace Eigen
