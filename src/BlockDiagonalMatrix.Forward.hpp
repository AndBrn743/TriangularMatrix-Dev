//
// Created by Andy on 7/13/2024.
//

#pragma once

#include <Eigen/Dense>


namespace Hoppy
{
	using namespace Eigen;

	template <typename BlockType>
	class BlockDiagonalMatrix;

	struct BlockDiagonalStorage
	{
		/* NO CODE */
	};

	struct BlockDiagonalShape
	{
		/* NO CODE */
	};

	template <typename Derived>
	class BlockDiagonalMatrixExpr;

	template <typename T>
	class UnaryMinus;

	template <typename LHS, typename RHS>
	class BinaryAdd;

	template <typename LHS, typename RHS>
	class BinaryMinus;

	template <typename LHS, typename RHS>
	class BinaryMultiply;

	template <typename T, typename Scalar>
	class ScalarMultiply;

	template <typename T, typename Scalar>
	class ScalarDivide;
}  // namespace Hoppy
