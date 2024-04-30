//
// Created by Andy Brown on 4/29/2024.
// This is an extension to the Eigen library, which can be obtained from
// https://eigen.tuxfamily.org/index.php?title=Main_Page
//
// Copyright (C) 2024 Andy Brown <AndBrwn1933@outlook.com>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//

#pragma once

#include <Eigen/Core>

namespace Eigen
{
	template <typename MatrixType>
	class NonResizableView;

	template <typename MatrixType, typename StorageKind>
	class NonResizableViewImpl;

	namespace internal
	{
		template <typename MatrixType>
		struct traits<NonResizableView<MatrixType>> : traits<MatrixType>
		{
			using MatrixTypeNested = typename ref_selector<MatrixType>::type;
			using MatrixTypeNestedPlain = typename remove_reference<MatrixTypeNested>::type;
		};
	}  // namespace internal

	template <typename MatrixType, typename StorageKind>
	class NonResizableViewImpl;

	template <typename MatrixType>
	class NonResizableView
	    : public NonResizableViewImpl<MatrixType, typename internal::traits<MatrixType>::StorageKind>
	{
	public:
		using MatrixTypeNested = typename internal::ref_selector<MatrixType>::non_const_type;

		using Base =
		        typename NonResizableViewImpl<MatrixType, typename internal::traits<MatrixType>::StorageKind>::Base;
		EIGEN_GENERIC_PUBLIC_INTERFACE(NonResizableView)
		using NestedExpression = typename internal::remove_all<MatrixType>::type;
		static constexpr int Options = MatrixType::Options;

		EIGEN_DEVICE_FUNC
		explicit EIGEN_STRONG_INLINE NonResizableView(MatrixType& matrix) : m_matrix(matrix)
		{
			/* NO CODE */
		}

		EIGEN_INHERIT_ASSIGNMENT_OPERATORS(NonResizableView)

		EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE EIGEN_CONSTEXPR Index rows() const EIGEN_NOEXCEPT
		{
			// return m_matrix.cols();
			return m_matrix.rows();
		}
		EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE EIGEN_CONSTEXPR Index cols() const EIGEN_NOEXCEPT
		{
			// return m_matrix.rows();
			return m_matrix.cols();
		}

		/** \returns the nested expression */
		EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE const typename internal::remove_all<MatrixTypeNested>::type&
		nestedExpression() const
		{
			return m_matrix;
		}

		/** \returns the nested expression */
		EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE typename internal::remove_reference<MatrixTypeNested>::type&
		nestedExpression()
		{
			return m_matrix;
		}

		// EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE void resize(Index nrows, Index ncols) => delete
		// EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE void conservativeResize(Index nrows, Index ncols) => delete

	protected:
		typename internal::ref_selector<MatrixType>::non_const_type m_matrix;
	};

	namespace internal
	{
		template <typename MatrixType, bool HasDirectAccess = has_direct_access<MatrixType>::ret>
		struct NonResizableViewImpl_base
		{
			using type = typename dense_xpr_base<NonResizableView<MatrixType>>::type;
		};

	}  // end namespace internal

	// Generic API dispatcher
	template <typename XprType, typename StorageKind>
	class NonResizableViewImpl : public internal::generic_xpr_base<NonResizableView<XprType>>::type
	{
	public:
		using Base = typename internal::generic_xpr_base<NonResizableView<XprType>>::type;
	};

	template <typename MatrixType>
	class NonResizableViewImpl<MatrixType, Dense>
		: public internal::NonResizableViewImpl_base<MatrixType>::type
	{
	public:
		using Base = typename internal::NonResizableViewImpl_base<MatrixType>::type;
		using Base::coeffRef;
		EIGEN_DENSE_PUBLIC_INTERFACE(NonResizableView<MatrixType>)
		EIGEN_INHERIT_ASSIGNMENT_OPERATORS(NonResizableViewImpl)

		EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE Index innerStride() const
		{
			return derived().nestedExpression().innerStride();
		}

		EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE Index outerStride() const
		{
			return derived().nestedExpression().outerStride();
		}

		using ScalarWithConstIfNotLvalue =
		        typename internal::conditional<internal::is_lvalue<MatrixType>::value, Scalar, const Scalar>::type;

		EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE ScalarWithConstIfNotLvalue* data()
		{
			return derived().nestedExpression().data();
		}

		EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE const Scalar* data() const
		{
			return derived().nestedExpression().data();
		}

		// FIXME: shall we keep the const version of coeffRef?
		EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE const Scalar& coeffRef(Index rowId, Index colId) const
		{
			return derived().nestedExpression().coeffRef(rowId, colId);
		}

		EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE const Scalar& coeffRef(Index index) const
		{
			return derived().nestedExpression().coeffRef(index);
		}

	protected:
		EIGEN_DEFAULT_EMPTY_CONSTRUCTOR_AND_DESTRUCTOR(NonResizableViewImpl)
	};


	namespace internal
	{
		template <typename ArgType>
		struct evaluator<NonResizableView<ArgType>> : evaluator<ArgType>
		{
			using XprType = NonResizableView<ArgType>;

			EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE explicit evaluator(const XprType& t)
			    : evaluator<ArgType>(t.nestedExpression())
			{
				/* NO CODE */
			}
		};
	}  // namespace internal

}  // end namespace Eigen
