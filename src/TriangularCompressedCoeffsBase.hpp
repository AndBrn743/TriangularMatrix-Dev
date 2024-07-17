//
// Created by Andy on 2/13/2024.
//

#pragma once

namespace Hoppy
{
	template <typename Derived, int Level>
	class TriangularCompressedCoeffsBase : public Eigen::EigenBase<Derived>
	{
	public:
		using Base = Eigen::EigenBase<Derived>;
		using Base::derived;
		using Scalar = typename Eigen::internal::traits<Derived>::Scalar;


		Scalar coeff(const Eigen::Index i, const Eigen::Index j) const
		{
			// NOTE: this duck is needed by operator <<
			return Eigen::internal::evaluator<Derived>(derived()).coeff(i, j);
		}

		decltype(auto) coeffRef(const Eigen::Index i, const Eigen::Index j)
		{
			return Eigen::internal::evaluator<Derived>(derived()).CoeffRef(i, j);
		}

		constexpr Scalar operator()(const Eigen::Index i, const Eigen::Index j) const
		{
			assert(i >= 0 && i < derived().rows() && "Row index out of valid range");
			assert(j >= 0 && j < derived().cols() && "Column index out of valid range");
			return coeff(i, j);
		}

		constexpr decltype(auto) operator()(const Eigen::Index i, const Eigen::Index j)
		{
			assert(i >= 0 && i < derived().rows() && "Row index out of valid range");
			assert(j >= 0 && j < derived().cols() && "Column index out of valid range");
			return coeffRef(i, j);
		}
	};
}  // namespace Hoppy
