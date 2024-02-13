//
// Created by Andy on 2/13/2024.
//

#pragma once

namespace Hoppy
{
	template <typename Derived>
	class TriangularMatrixBase : public TriangularBase<Derived>
	{
	public:
		using Scalar = typename internal::traits<Derived>::Scalar;
		using Base = TriangularBase<Derived>;
		using Base::IsShapeAs;
		using Base::operator();
		using Base::operator=;

		// TODO: Add matrix algorithmics support here
		//       - matrix * matrix
		//       - matrix * vector
		//       - U * matrix * U
		//       - EVD
		//       - SVD
		//       - etc.
	};
}  // namespace Hoppy
