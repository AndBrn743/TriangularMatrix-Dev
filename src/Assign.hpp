//
// Created by Andy on 2/28/2024.
//

#pragma once

namespace Eigen
{
	namespace internal
	{
		struct TriangularCompressed2Dense
		{
			/* EMPTY */
		};

		template <>
		struct AssignmentKind<DenseShape, TriangularCompressedShape>
		{
			using Kind = TriangularCompressed2Dense;
		};

		template <typename DstXprType, typename SrcXprType, typename Functor, typename Weak>
		struct Assignment<DstXprType, SrcXprType, Functor, TriangularCompressed2Dense, Weak>
		{
			static void run(DstXprType& dst, const SrcXprType& src, const Functor& func)
			{
				if (is_same<Functor, assign_op<typename DstXprType::Scalar, typename SrcXprType::Scalar>>::value)
				{
					dst.setZero();
				}

				resize_if_allowed(dst, src, func);
				evaluator<SrcXprType> srcEval(src);
				evaluator<DstXprType> dstEval(dst);

				for (Index i = 0; i < src.rows(); i++)
				{
					for (Index j = 0; j < src.cols(); j++)
					{
						func.assignCoeff(dstEval.coeffRef(i, j), srcEval.coeff(i, j));
					}
				}
			}
		};
	}  // namespace internal
}  // namespace Eigen

