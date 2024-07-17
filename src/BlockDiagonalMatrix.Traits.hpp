//
// Created by Andy on 7/13/2024.
//

#pragma once

#include "BlockDiagonalMatrix.Forward.hpp"

template <typename BlockType>
struct Eigen::internal::is_scalar<Hoppy::BlockDiagonalMatrix<BlockType>> : std::false_type
{
	/* NO CODE */
};

template <typename Derived>
struct Eigen::internal::is_scalar<Hoppy::UnaryMinus<Derived>> : std::false_type
{
	/* NO CODE */
};

template <typename T, typename U>
struct Eigen::internal::is_scalar<Hoppy::BinaryAdd<T, U>> : std::false_type
{
	/* NO CODE */
};

template <typename T, typename U>
struct Eigen::internal::is_scalar<Hoppy::BinaryMinus<T, U>> : std::false_type
{
	/* NO CODE */
};

template <typename T, typename U>
struct Eigen::internal::is_scalar<Hoppy::BinaryMultiply<T, U>> : std::false_type
{
	/* NO CODE */
};

template <typename T, typename U>
struct Eigen::internal::is_scalar<Hoppy::ScalarMultiply<T, U>> : std::false_type
{
	/* NO CODE */
};

template <typename T, typename U>
struct Eigen::internal::is_scalar<Hoppy::ScalarDivide<T, U>> : std::false_type
{
	/* NO CODE */
};


template <>
struct Eigen::internal::storage_kind_to_shape<Hoppy::BlockDiagonalStorage>
{
	using Shape = Hoppy::BlockDiagonalShape;
};



template <typename Derived>
struct Eigen::internal::traits<Hoppy::BlockDiagonalMatrixExpr<Derived>>
{
	using Scalar = typename traits<Derived>::Scalar;
	using Shape = Hoppy::BlockDiagonalShape;
	using StorageKind = Hoppy::BlockDiagonalStorage;
	using StorageIndex = Index;
	static constexpr int Flags = 0;
	static constexpr int RowsAtCompileTime = Dynamic;
	static constexpr int ColsAtCompileTime = Dynamic;
	static constexpr int MaxRowsAtCompileTime = Dynamic;
	static constexpr int MaxColsAtCompileTime = Dynamic;
};

template <typename BlockType>
struct Eigen::internal::traits<Hoppy::BlockDiagonalMatrix<BlockType>>
{
	using Scalar = typename traits<BlockType>::Scalar;
	using Shape = Hoppy::BlockDiagonalShape;
	using StorageKind = Hoppy::BlockDiagonalStorage;
	using StorageIndex = Index;
	static constexpr int Flags = 0;
	static constexpr int RowsAtCompileTime = Dynamic;
	static constexpr int ColsAtCompileTime = Dynamic;
	static constexpr int MaxRowsAtCompileTime = Dynamic;
	static constexpr int MaxColsAtCompileTime = Dynamic;
};

template <typename T>
struct Eigen::internal::traits<Hoppy::UnaryMinus<T>> : traits<T>
{
	/* NO CODE */
};

template <typename Lhs, typename Rhs>
struct Eigen::internal::traits<Hoppy::BinaryAdd<Lhs, Rhs>>
{
	using Scalar = typename std::common_type<typename traits<Lhs>::Scalar, typename traits<Rhs>::Scalar>::type;
	using StorageKind =
	        typename promote_storage_type<typename traits<Lhs>::StorageKind, typename traits<Rhs>::StorageKind>::ret;
	using StorageIndex =
	        typename std::common_type<typename traits<Lhs>::StorageIndex, typename traits<Rhs>::StorageIndex>::type;
	static constexpr int Flags = NestByRefBit;
	static constexpr int MaxRowsAtCompileTime =
	        traits<Lhs>::MaxRowsAtCompileTime == Dynamic || traits<Rhs>::MaxRowsAtCompileTime == Dynamic
	                ? Dynamic
	                : std::max(traits<Lhs>::MaxRowsAtCompileTime, traits<Rhs>::MaxRowsAtCompileTime);
	static constexpr int MaxColsAtCompileTime =
	        traits<Lhs>::MaxColsAtCompileTime == Dynamic || traits<Rhs>::MaxColsAtCompileTime == Dynamic
	                ? Dynamic
	                : std::max(traits<Lhs>::MaxColsAtCompileTime, traits<Rhs>::MaxColsAtCompileTime);
	static constexpr int RowsAtCompileTime =
	        traits<Lhs>::RowsAtCompileTime == Dynamic || traits<Rhs>::RowsAtCompileTime == Dynamic
	                ? Dynamic
	                : std::max(traits<Lhs>::RowsAtCompileTime, traits<Rhs>::RowsAtCompileTime);
	static constexpr int ColsAtCompileTime =
	        traits<Lhs>::ColsAtCompileTime == Dynamic || traits<Rhs>::ColsAtCompileTime == Dynamic
	                ? Dynamic
	                : std::max(traits<Lhs>::ColsAtCompileTime, traits<Rhs>::ColsAtCompileTime);
};

template <typename Lhs, typename Rhs>
struct Eigen::internal::traits<Hoppy::BinaryMinus<Lhs, Rhs>>
{
	using Scalar = typename std::common_type<typename traits<Lhs>::Scalar, typename traits<Rhs>::Scalar>::type;
	using StorageKind =
	        typename promote_storage_type<typename traits<Lhs>::StorageKind, typename traits<Rhs>::StorageKind>::ret;
	using StorageIndex =
	        typename std::common_type<typename traits<Lhs>::StorageIndex, typename traits<Rhs>::StorageIndex>::type;
	static constexpr int Flags = NestByRefBit;
	static constexpr int MaxRowsAtCompileTime =
	        traits<Lhs>::MaxRowsAtCompileTime == Dynamic || traits<Rhs>::MaxRowsAtCompileTime == Dynamic
	                ? Dynamic
	                : std::max(traits<Lhs>::MaxRowsAtCompileTime, traits<Rhs>::MaxRowsAtCompileTime);
	static constexpr int MaxColsAtCompileTime =
	        traits<Lhs>::MaxColsAtCompileTime == Dynamic || traits<Rhs>::MaxColsAtCompileTime == Dynamic
	                ? Dynamic
	                : std::max(traits<Lhs>::MaxColsAtCompileTime, traits<Rhs>::MaxColsAtCompileTime);
	static constexpr int RowsAtCompileTime =
	        traits<Lhs>::RowsAtCompileTime == Dynamic || traits<Rhs>::RowsAtCompileTime == Dynamic
	                ? Dynamic
	                : std::max(traits<Lhs>::RowsAtCompileTime, traits<Rhs>::RowsAtCompileTime);
	static constexpr int ColsAtCompileTime =
	        traits<Lhs>::ColsAtCompileTime == Dynamic || traits<Rhs>::ColsAtCompileTime == Dynamic
	                ? Dynamic
	                : std::max(traits<Lhs>::ColsAtCompileTime, traits<Rhs>::ColsAtCompileTime);
};

template <typename Lhs, typename Rhs>
struct Eigen::internal::traits<Hoppy::BinaryMultiply<Lhs, Rhs>>
{
	using Scalar = typename std::common_type<typename traits<Lhs>::Scalar, typename traits<Rhs>::Scalar>::type;
	using StorageKind =
	        typename promote_storage_type<typename traits<Lhs>::StorageKind, typename traits<Rhs>::StorageKind>::ret;
	using StorageIndex =
	        typename std::common_type<typename traits<Lhs>::StorageIndex, typename traits<Rhs>::StorageIndex>::type;
	static constexpr int Flags = NestByRefBit;
	static constexpr int MaxRowsAtCompileTime = traits<Lhs>::MaxRowsAtCompileTime;
	static constexpr int MaxColsAtCompileTime = traits<Rhs>::MaxColsAtCompileTime;
	static constexpr int RowsAtCompileTime = traits<Lhs>::RowsAtCompileTime;
	static constexpr int ColsAtCompileTime = traits<Rhs>::ColsAtCompileTime;
};

template <typename T, typename OtherScalar>
struct Eigen::internal::traits<Hoppy::ScalarMultiply<T, OtherScalar>>
{
	using Scalar = typename std::common_type<typename traits<T>::Scalar, OtherScalar>::type;
	using StorageKind = typename traits<T>::StorageKind;
	using StorageIndex = typename traits<T>::StorageIndex;
	static constexpr int Flags = NestByRefBit;
	static constexpr int MaxRowsAtCompileTime = traits<T>::MaxRowsAtCompileTime;
	static constexpr int MaxColsAtCompileTime = traits<T>::MaxColsAtCompileTime;
	static constexpr int RowsAtCompileTime = traits<T>::RowsAtCompileTime;
	static constexpr int ColsAtCompileTime = traits<T>::ColsAtCompileTime;
};

template <typename T, typename OtherScalar>
struct Eigen::internal::traits<Hoppy::ScalarDivide<T, OtherScalar>>
{
	using Scalar = typename std::common_type<typename traits<T>::Scalar, OtherScalar>::type;
	using StorageKind = typename traits<T>::StorageKind;
	using StorageIndex = typename traits<T>::StorageIndex;
	static constexpr int Flags = NestByRefBit;
	static constexpr int MaxRowsAtCompileTime = traits<T>::MaxRowsAtCompileTime;
	static constexpr int MaxColsAtCompileTime = traits<T>::MaxColsAtCompileTime;
	static constexpr int RowsAtCompileTime = traits<T>::RowsAtCompileTime;
	static constexpr int ColsAtCompileTime = traits<T>::ColsAtCompileTime;
};
