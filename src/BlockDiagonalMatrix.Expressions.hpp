//
// Created by Andy on 7/13/2024.
//

#pragma once

#include "BlockDiagonalMatrix.Forward.hpp"


namespace Hoppy
{
	template <typename T>
	class UnaryMinus : public BlockDiagonalMatrixExpr<UnaryMinus<T>>
	{
	public:
		using Scalar = typename internal::traits<UnaryMinus>::Scalar;
		static constexpr int Flags = NestByRefBit;

		explicit UnaryMinus(const T& operand) : _operand(operand)
		{
			/* NO CODE */
		}

		Index totalDimension() const
		{
			return _operand.totalDimension();
		}

		Index rows() const
		{
			return _operand.rows();
		}

		Index cols() const
		{
			return _operand.cols();
		}

		decltype(auto) operator[](const Index index) const
		{
			return -_operand[index];
		}

		decltype(auto) operator[](const Index index)
		{
			return -_operand[index];
		}

		Index blockCount() const
		{
			return _operand.blockCount();
		}

	private:
		const T& _operand;
	};

	template <typename LHS, typename RHS>
	class BinaryAdd : public BlockDiagonalMatrixExpr<BinaryAdd<LHS, RHS>>
	{
	public:
		using Scalar = typename internal::traits<BinaryAdd>::Scalar;
		static constexpr int Flags = NestByRefBit;

		BinaryAdd(const LHS& lhs, const RHS& rhs) : _lhs(lhs), _rhs(rhs)
		{
			if (_lhs.blockCount() != _rhs.blockCount())
			{
				throw std::runtime_error("Size mismatch in BinaryAdd");
			}
		}

		decltype(auto) operator[](const Index index) const
		{
			return _lhs[index] + _rhs[index];
		}

		decltype(auto) operator[](const Index index)
		{
			return _lhs[index] + _rhs[index];
		}

		Index blockCount() const
		{
			return _lhs.blockCount();
		}

		Index cols() const
		{
			return _lhs.cols();
		}

		Index rows() const
		{
			return _lhs.rows();
		}

	private:
		const LHS& _lhs;
		const RHS& _rhs;
	};

	template <typename LHS, typename RHS>
	class BinaryMinus : public BlockDiagonalMatrixExpr<BinaryMinus<LHS, RHS>>
	{
	public:
		using Scalar = typename internal::traits<BinaryMinus>::Scalar;
		static constexpr int Flags = NestByRefBit;

		BinaryMinus(const LHS& lhs, const RHS& rhs) : _lhs(lhs), _rhs(rhs)
		{
			if (_lhs.blockCount() != _rhs.blockCount())
			{
				throw std::runtime_error("Size mismatch in BinaryAdd");
			}
		}

		decltype(auto) operator[](const Index index) const
		{
			return _lhs[index] - _rhs[index];
		}

		decltype(auto) operator[](const Index index)
		{
			return _lhs[index] - _rhs[index];
		}

		Index blockCount() const
		{
			return _lhs.blockCount();
		}

		Index cols() const
		{
			return _lhs.cols();
		}

		Index rows() const
		{
			return _lhs.rows();
		}

	private:
		const LHS& _lhs;
		const RHS& _rhs;
	};

	template <typename LHS, typename RHS>
	class BinaryMultiply : public BlockDiagonalMatrixExpr<BinaryMultiply<LHS, RHS>>
	{
	public:
		using Scalar = typename internal::traits<BinaryMultiply>::Scalar;
		static constexpr int Flags = NestByRefBit;

		BinaryMultiply(const LHS& lhs, const RHS& rhs) : _lhs(lhs), _rhs(rhs)
		{
			if (_lhs.blockCount() != _rhs.blockCount())
			{
				throw std::runtime_error("Block count mismatch in BinaryMultiply");
			}
		}

		auto operator[](const Index index) const
		{
			return _lhs[index] * _rhs[index];
		}

		Index blockCount() const
		{
			return _lhs.blockCount();
		}

		Index rows() const
		{
			return _lhs.rows();
		}

		Index cols() const
		{
			return _rhs.cols();
		}

	private:
		const LHS& _lhs;
		const RHS& _rhs;
	};

	template <typename T, typename OtherScalar>
	class ScalarMultiply : public BlockDiagonalMatrixExpr<ScalarMultiply<T, OtherScalar>>
	{
		static_assert(internal::is_scalar<OtherScalar>::value, "Bad dispatch");

	public:
		using Scalar = typename internal::traits<ScalarMultiply>::Scalar;
		static constexpr int Flags = NestByRefBit;

		ScalarMultiply(const T& operand, OtherScalar scalar) : _operand(operand), _scalar(std::move(scalar))
		{
			/* NO CODE */
		}

		decltype(auto) operator[](const Index index) const
		{
			return _operand[index] * _scalar;
		}

		decltype(auto) operator[](const Index index)
		{
			return _operand[index] * _scalar;
		}

		Index blockCount() const
		{
			return _operand.blockCount();
		}

		Index cols() const
		{
			return _operand.cols();
		}

		Index rows() const
		{
			return _operand.rows();
		}

	private:
		const T& _operand;
		OtherScalar _scalar;
	};

	template <typename T, typename OtherScalar>
	class ScalarDivide : public BlockDiagonalMatrixExpr<ScalarDivide<T, OtherScalar>>
	{
		static_assert(internal::is_scalar<OtherScalar>::value, "Bad dispatch");

	public:
		using Scalar = typename internal::traits<ScalarDivide>::Scalar;
		static constexpr int Flags = NestByRefBit;

		ScalarDivide(const T& operand, OtherScalar scalar) : _operand(operand), _scalar(std::move(scalar))
		{
			/* NO CODE */
		}

		decltype(auto) operator[](const Index index) const
		{
			return _operand[index] / _scalar;
		}

		decltype(auto) operator[](const Index index)
		{
			return _operand[index] / _scalar;
		}

		Index blockCount() const
		{
			return _operand.blockCount();
		}

		Index cols() const
		{
			return _operand.cols();
		}

		Index rows() const
		{
			return _operand.rows();
		}


	private:
		const T& _operand;
		OtherScalar _scalar;
	};


	template <typename T>
	UnaryMinus<T> operator-(const BlockDiagonalMatrixExpr<T>& operand)
	{
		return UnaryMinus<T>(operand.derived());
	}

	template <typename LHS, typename RHS>
	BinaryAdd<LHS, RHS> operator+(const BlockDiagonalMatrixExpr<LHS>& lhs, const BlockDiagonalMatrixExpr<RHS>& rhs)
	{
		return BinaryAdd<LHS, RHS>(lhs.derived(), rhs.derived());
	}

	template <typename LHS, typename RHS>
	BinaryMinus<LHS, RHS> operator-(const BlockDiagonalMatrixExpr<LHS>& lhs, const BlockDiagonalMatrixExpr<RHS>& rhs)
	{
		return BinaryMinus<LHS, RHS>(lhs.derived(), rhs.derived());
	}

	template <typename LHS, typename RHS>
	BinaryMultiply<LHS, RHS> operator*(const BlockDiagonalMatrixExpr<LHS>& lhs, const BlockDiagonalMatrixExpr<RHS>& rhs)
	{
		return BinaryMultiply<LHS, RHS>(lhs.derived(), rhs.derived());
	}

	template <typename T, typename Scalar>
	typename std::enable_if<internal::is_scalar<Scalar>::value && !std::is_base_of<EigenBase<Scalar>, Scalar>::value,
	                        ScalarMultiply<T, Scalar>>::type  //
	operator*(const BlockDiagonalMatrixExpr<T>& operand, Scalar scalar)
	{
		return ScalarMultiply<T, Scalar>(operand.derived(), scalar);
	}

	template <typename Scalar, typename T>
	typename std::enable_if<internal::is_scalar<Scalar>::value && !std::is_base_of<EigenBase<Scalar>, Scalar>::value,
	                        ScalarMultiply<T, Scalar>>::type  //
	operator*(Scalar scalar, const BlockDiagonalMatrixExpr<T>& operand)
	{
		return ScalarMultiply<T, Scalar>(operand.derived(), scalar);
	}

	template <typename T, typename Scalar>
	typename std::enable_if<internal::is_scalar<Scalar>::value && !std::is_base_of<EigenBase<Scalar>, Scalar>::value,
	                        ScalarDivide<T, Scalar>>::type  //
	operator/(const BlockDiagonalMatrixExpr<T>& operand, Scalar scalar)
	{
		return ScalarDivide<T, Scalar>(operand.derived(), scalar);
	}

	template <typename DenseDerived, typename BlockDiagonalDerived>
	Product<DenseDerived, BlockDiagonalDerived, Eigen::DefaultProduct>  //
	operator*(const MatrixBase<DenseDerived>& denseMatrix,
	          const BlockDiagonalMatrixExpr<BlockDiagonalDerived>& blockDiagonalMatrix)
	{
		return Eigen::Product<DenseDerived, BlockDiagonalDerived, Eigen::DefaultProduct>(denseMatrix.derived(),
		                                                                                 blockDiagonalMatrix.derived());
	}

	template <typename BlockDiagonalDerived, typename DenseDerived>
	Product<BlockDiagonalDerived, DenseDerived, Eigen::DefaultProduct>  //
	operator*(const BlockDiagonalMatrixExpr<BlockDiagonalDerived>& blockDiagonalMatrix,
	          const MatrixBase<DenseDerived>& denseMatrix)
	{
		return Eigen::Product<BlockDiagonalDerived, DenseDerived, Eigen::DefaultProduct>(blockDiagonalMatrix.derived(),
		                                                                                 denseMatrix.derived());
	}


	template <typename Lhs, typename Rhs>
	class DenseBlockDiagonalBinaryAdd : public EigenBase<DenseBlockDiagonalBinaryAdd<Lhs, Rhs>>,
	                                    internal::no_assignment_operator
	{
	public:
		using LhsNested = typename internal::ref_selector<Lhs>::type;
		using RhsNested = typename internal::ref_selector<Rhs>::type;
		using Scalar = typename internal::traits<DenseBlockDiagonalBinaryAdd>::Scalar;
		static constexpr int ColsAtCompileTime = internal::traits<DenseBlockDiagonalBinaryAdd>::ColsAtCompileTime;
		static constexpr int RowsAtCompileTime = internal::traits<DenseBlockDiagonalBinaryAdd>::RowsAtCompileTime;
		static constexpr int Flags = internal::traits<DenseBlockDiagonalBinaryAdd>::Flags;
		static constexpr bool IsVectorAtCompileTime =
		        internal::traits<DenseBlockDiagonalBinaryAdd>::IsVectorAtCompileTime;
		using PlainObject = Matrix<Scalar, RowsAtCompileTime, ColsAtCompileTime>;
		using NestedExpression = DenseBlockDiagonalBinaryAdd;

		DenseBlockDiagonalBinaryAdd(const Lhs& lhs, const Rhs& rhs) : _lhs(lhs), _rhs(rhs)
		{
			EIGEN_STATIC_ASSERT_SAME_MATRIX_SIZE(Lhs, Rhs)
			eigen_assert(lhs.rows() == rhs.rows() && lhs.cols() == rhs.cols());
		}

		template <typename Dest>
		void evalTo(Dest& dst) const
		{
			dst = _lhs;
			dst += _rhs;
		}

		EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE EIGEN_CONSTEXPR Index rows() const EIGEN_NOEXCEPT
		{
			return internal::traits<typename internal::remove_all<LhsNested>::type>::RowsAtCompileTime == Dynamic
			               ? _rhs.rows()
			               : _lhs.rows();
		}
		EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE EIGEN_CONSTEXPR Index cols() const EIGEN_NOEXCEPT
		{
			return internal::traits<typename internal::remove_all<LhsNested>::type>::ColsAtCompileTime == Dynamic
			               ? _rhs.cols()
			               : _lhs.cols();
		}


	private:
		LhsNested _lhs;
		RhsNested _rhs;
	};

	template <typename Derived, typename OtherDerived>
	DenseBlockDiagonalBinaryAdd<const Derived, const OtherDerived>  //
	operator+(const MatrixBase<Derived>& lhs, const BlockDiagonalMatrixExpr<OtherDerived>& rhs)
	{
		return DenseBlockDiagonalBinaryAdd<const Derived, const OtherDerived>(lhs.derived(), rhs.derived());
	}

	template <typename Derived, typename OtherDerived>
	DenseBlockDiagonalBinaryAdd<const Derived, const OtherDerived>  //
	operator+(const BlockDiagonalMatrixExpr<Derived>& lhs, const MatrixBase<OtherDerived>& rhs)
	{
		return DenseBlockDiagonalBinaryAdd<const Derived, const OtherDerived>(lhs.derived(), rhs.derived());
	}


	template <typename Lhs, typename Rhs>
	class DenseBlockDiagonalBinaryMinus : public EigenBase<DenseBlockDiagonalBinaryMinus<Lhs, Rhs>>,
	                                    internal::no_assignment_operator
	{
	public:
		using LhsNested = typename internal::ref_selector<Lhs>::type;
		using RhsNested = typename internal::ref_selector<Rhs>::type;
		using Scalar = typename internal::traits<DenseBlockDiagonalBinaryMinus>::Scalar;
		static constexpr int ColsAtCompileTime = internal::traits<DenseBlockDiagonalBinaryMinus>::ColsAtCompileTime;
		static constexpr int RowsAtCompileTime = internal::traits<DenseBlockDiagonalBinaryMinus>::RowsAtCompileTime;
		static constexpr int Flags = internal::traits<DenseBlockDiagonalBinaryMinus>::Flags;
		static constexpr bool IsVectorAtCompileTime =
		        internal::traits<DenseBlockDiagonalBinaryMinus>::IsVectorAtCompileTime;
		using PlainObject = Matrix<Scalar, RowsAtCompileTime, ColsAtCompileTime>;
		using NestedExpression = DenseBlockDiagonalBinaryMinus;

		DenseBlockDiagonalBinaryMinus(const Lhs& lhs, const Rhs& rhs) : _lhs(lhs), _rhs(rhs)
		{
			EIGEN_STATIC_ASSERT_SAME_MATRIX_SIZE(Lhs, Rhs)
			eigen_assert(lhs.rows() == rhs.rows() && lhs.cols() == rhs.cols());
		}

		template <typename Dest>
		void evalTo(Dest& dst) const
		{
			dst = _lhs;
			dst -= _rhs;
		}

		EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE EIGEN_CONSTEXPR Index rows() const EIGEN_NOEXCEPT
		{
			return internal::traits<typename internal::remove_all<LhsNested>::type>::RowsAtCompileTime == Dynamic
			               ? _rhs.rows()
			               : _lhs.rows();
		}
		EIGEN_DEVICE_FUNC EIGEN_STRONG_INLINE EIGEN_CONSTEXPR Index cols() const EIGEN_NOEXCEPT
		{
			return internal::traits<typename internal::remove_all<LhsNested>::type>::ColsAtCompileTime == Dynamic
			               ? _rhs.cols()
			               : _lhs.cols();
		}


	private:
		LhsNested _lhs;
		RhsNested _rhs;
	};

	template <typename Derived, typename OtherDerived>
	DenseBlockDiagonalBinaryMinus<const Derived, const OtherDerived>  //
	operator-(const MatrixBase<Derived>& lhs, const BlockDiagonalMatrixExpr<OtherDerived>& rhs)
	{
		return DenseBlockDiagonalBinaryMinus<const Derived, const OtherDerived>(lhs.derived(), rhs.derived());
	}

	template <typename Derived, typename OtherDerived>
	DenseBlockDiagonalBinaryMinus<const Derived, const OtherDerived>  //
	operator-(const BlockDiagonalMatrixExpr<Derived>& lhs, const MatrixBase<OtherDerived>& rhs)
	{
		return DenseBlockDiagonalBinaryMinus<const Derived, const OtherDerived>(lhs.derived(), rhs.derived());
	}
}  // namespace Hoppy


template <typename BlockDiagonalDerived, typename DenseDerived>
struct Eigen::internal::generic_product_impl<BlockDiagonalDerived,
                                             DenseDerived,
                                             Hoppy::BlockDiagonalShape,
                                             Eigen::DenseShape,
                                             Eigen::GemmProduct>
    : generic_product_impl_base<BlockDiagonalDerived,
                                DenseDerived,
                                generic_product_impl<BlockDiagonalDerived, DenseDerived>>
{
	using Scalar = typename Product<BlockDiagonalDerived, DenseDerived>::Scalar;

	template <typename Dest>
	static void scaleAndAddTo(Dest& dst,
	                          const BlockDiagonalDerived& blockDiagonalMatrix,
	                          const DenseDerived& denseMatrix,
	                          const Scalar scalar)
	{
		Index offset = 0;
		for (Index i = 0; i < blockDiagonalMatrix.blockCount(); i++)
		{
			const Index dim = blockDiagonalMatrix[i].rows();
			dst.block(offset, 0, dim, denseMatrix.cols()) +=
			        scalar * blockDiagonalMatrix[i] * denseMatrix.block(offset, 0, dim, denseMatrix.cols());
			offset += dim;
		}
	}
};

template <typename DenseDerived, typename BlockDiagonalDerived>
struct Eigen::internal::generic_product_impl<DenseDerived,
                                             BlockDiagonalDerived,
                                             Eigen::DenseShape,
                                             Hoppy::BlockDiagonalShape,
                                             Eigen::GemmProduct>
    : generic_product_impl_base<DenseDerived,
                                BlockDiagonalDerived,
                                generic_product_impl<DenseDerived, BlockDiagonalDerived>>
{
	using Scalar = typename Product<DenseDerived, BlockDiagonalDerived>::Scalar;

	template <typename Dest>
	static void scaleAndAddTo(Dest& dst,
	                          const DenseDerived& denseMatrix,
	                          const BlockDiagonalDerived& blockDiagonalMatrix,
	                          const Scalar scalar)
	{
		Index offset = 0;
		for (Index i = 0; i < blockDiagonalMatrix.blockCount(); i++)
		{
			const auto dim = blockDiagonalMatrix[i].rows();
			dst.middleCols(offset, dim) += scalar * denseMatrix.middleCols(offset, dim) * blockDiagonalMatrix[i];
			offset += dim;
		}
	}
};


template <typename Lhs, typename Rhs>
struct Eigen::internal::traits<Hoppy::DenseBlockDiagonalBinaryAdd<Lhs, Rhs>>
{
	static constexpr int Flags = NestByRefBit;

	static constexpr int RowsAtCompileTime =
	        traits<Lhs>::RowsAtCompileTime == Dynamic || traits<Rhs>::RowsAtCompileTime == Dynamic
	                ? Dynamic
	                : traits<Lhs>::RowsAtCompileTime;
	static constexpr int ColsAtCompileTime =
	        traits<Lhs>::ColsAtCompileTime == Dynamic || traits<Rhs>::ColsAtCompileTime == Dynamic
	                ? Dynamic
	                : traits<Lhs>::ColsAtCompileTime;
	static constexpr int MaxRowsAtCompileTime =
	        traits<Lhs>::MaxRowsAtCompileTime == Dynamic || traits<Rhs>::MaxRowsAtCompileTime == Dynamic
	                ? Dynamic
	                : traits<Lhs>::MaxRowsAtCompileTime;
	static constexpr int MaxColsAtCompileTime =
	        traits<Lhs>::MaxColsAtCompileTime == Dynamic || traits<Rhs>::MaxColsAtCompileTime == Dynamic
	                ? Dynamic
	                : traits<Lhs>::MaxColsAtCompileTime;
	static constexpr bool IsVectorAtCompileTime = RowsAtCompileTime == 1 || ColsAtCompileTime == 1;

	using Scalar = typename std::common_type<typename traits<Lhs>::Scalar, typename traits<Rhs>::Scalar>::type;
	using StorageIndex = Index;
	// Note: although `Dense` seems more appropriate for `StorageKind`, doing so will cause Eigen to call
	// `call_dense_assignment_loop` which we don't want
	using StorageKind = Hoppy::BlockDiagonalStorage;
};

template <typename Lhs, typename Rhs>
struct Eigen::internal::traits<Hoppy::DenseBlockDiagonalBinaryMinus<Lhs, Rhs>>
{
	static constexpr int Flags = NestByRefBit;

	static constexpr int RowsAtCompileTime =
	        traits<Lhs>::RowsAtCompileTime == Dynamic || traits<Rhs>::RowsAtCompileTime == Dynamic
	                ? Dynamic
	                : traits<Lhs>::RowsAtCompileTime;
	static constexpr int ColsAtCompileTime =
	        traits<Lhs>::ColsAtCompileTime == Dynamic || traits<Rhs>::ColsAtCompileTime == Dynamic
	                ? Dynamic
	                : traits<Lhs>::ColsAtCompileTime;
	static constexpr int MaxRowsAtCompileTime =
	        traits<Lhs>::MaxRowsAtCompileTime == Dynamic || traits<Rhs>::MaxRowsAtCompileTime == Dynamic
	                ? Dynamic
	                : traits<Lhs>::MaxRowsAtCompileTime;
	static constexpr int MaxColsAtCompileTime =
	        traits<Lhs>::MaxColsAtCompileTime == Dynamic || traits<Rhs>::MaxColsAtCompileTime == Dynamic
	                ? Dynamic
	                : traits<Lhs>::MaxColsAtCompileTime;
	static constexpr bool IsVectorAtCompileTime = RowsAtCompileTime == 1 || ColsAtCompileTime == 1;

	using Scalar = typename std::common_type<typename traits<Lhs>::Scalar, typename traits<Rhs>::Scalar>::type;
	using StorageIndex = Index;
	// Note: although `Dense` seems more appropriate for `StorageKind`, doing so will cause Eigen to call
	// `call_dense_assignment_loop` which we don't want
	using StorageKind = Hoppy::BlockDiagonalStorage;
};
