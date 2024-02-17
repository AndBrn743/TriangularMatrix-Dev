//
// Created by Andy on 2/13/2024.
//

#pragma once

#include <Eigen/Dense>

namespace Hoppy
{
	template <typename TScalar, int KDimensionAtCompileTime = Eigen::Dynamic, int KOption = Eigen::RowMajor>
	class UpperTriangularMatrix;

	template <typename TScalar, int KDimensionAtCompileTime = Eigen::Dynamic, int KOption = Eigen::RowMajor>
	class LowerTriangularMatrix;

	template <typename TScalar, int KDimensionAtCompileTime = Eigen::Dynamic, int KOption = Eigen::RowMajor>
	class SymmetricMatrix;

	template <typename TScalar, int KDimensionAtCompileTime = Eigen::Dynamic, int KOption = Eigen::RowMajor>
	class AntiSymmetricMatrix;

	template <typename TScalar, int KDimensionAtCompileTime = Eigen::Dynamic, int KOption = Eigen::RowMajor>
	class HermitianMatrix;

	template <typename TScalar, int KDimensionAtCompileTime = Eigen::Dynamic, int KOption = Eigen::RowMajor>
	class AntiHermitianMatrix;

	template <typename Derived>
	class PlainTriangularCompressedObjectBase;

	template <typename Derived>
	class TriangularCompressedMatrixBase;

	template <typename Derived>
	class TriangularCompressedBase;

	template <typename Derived, int Level>
	class TriangularCompressedCoeffsBase;
}  // namespace Hoppy
