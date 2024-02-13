//
// Created by Andy on 2/13/2024.
//

#pragma once

#include <Eigen/Dense>

namespace Hoppy
{
	using namespace Eigen;

	template <typename TScalar, int KDimensionAtCompileTime = Dynamic, int KOption = Eigen::RowMajor>
	class UpperTriangularMatrix;

	template <typename TScalar, int KDimensionAtCompileTime = Dynamic, int KOption = Eigen::RowMajor>
	class LowerTriangularMatrix;

	template <typename TScalar, int KDimensionAtCompileTime = Dynamic, int KOption = Eigen::RowMajor>
	class SymmetricMatrix;

	template <typename TScalar, int KDimensionAtCompileTime = Dynamic, int KOption = Eigen::RowMajor>
	class AntiSymmetricMatrix;

	template <typename TScalar, int KDimensionAtCompileTime = Dynamic, int KOption = Eigen::RowMajor>
	class HermitianMatrix;

	template <typename TScalar, int KDimensionAtCompileTime = Dynamic, int KOption = Eigen::RowMajor>
	class AntiHermitianMatrix;

	template <typename Derived>
	class PlainTriangularObjectBase;

	template <typename Derived>
	class TriangularMatrixBase;

	template <typename Derived>
	class TriangularBase;

	template <typename Derived, int Level>
	class TriangularCoeffsBase;
}  // namespace Hoppy
