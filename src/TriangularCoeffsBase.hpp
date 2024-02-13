//
// Created by Andy on 2/13/2024.
//

#pragma once

namespace Hoppy
{
  template <typename Derived, int Level>
  class TriangularCoeffsBase : public EigenBase<Derived>
  {
  public:
    using Base = EigenBase<Derived>;
    using Base::derived;
    using Scalar = typename internal::traits<Derived>::Scalar;

    auto operator()(Index i, Index j)
    {
      assert(i >= 0 && i < derived().rows() && "Row index out of valid range");
      assert(j >= 0 && j < derived().cols() && "Column index out of valid range");
      return internal::evaluator<Derived>(derived()).CoeffRef(i, j);
    }

    auto operator()(Index i, Index j) const
    {
      assert(i >= 0 && i < derived().rows() && "Row index out of valid range");
      assert(j >= 0 && j < derived().cols() && "Column index out of valid range");
      return internal::evaluator<Derived>(derived()).coeff(i, j);
    }
  };
}
