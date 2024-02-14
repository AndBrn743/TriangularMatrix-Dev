//
// Created by Andy on 2/13/2024.
//

#pragma once

namespace Hoppy
{
	struct TriangularCompressed
	{
		/* NO CODE */
	};

	struct TriangularCompressedShape
	{
		static std::string debugName()
		{
			return "TriangularCompressedShape";
		}
	};
}  // namespace Hoppy

#if defined(CREATE_PLAINTRIANGULAROBJECTBASE_BASED_MATRIX_CLASS)
#undef CREATE_PLAINTRIANGULAROBJECTBASE_BASED_MATRIX_CLASS
#endif

#define CREATE_PLAINTRIANGULAROBJECTBASE_BASED_MATRIX_CLASS(CLASS_NAME)                                                \
	namespace Hoppy                                                                                                    \
	{                                                                                                                  \
		template <typename _Scalar, int _DimensionAtCompileTime, int _Option>                                          \
		class CLASS_NAME : public PlainTriangularObjectBase<CLASS_NAME<_Scalar, _DimensionAtCompileTime, _Option>>     \
		{                                                                                                              \
		public:                                                                                                        \
			using Base = PlainTriangularObjectBase<CLASS_NAME<_Scalar, _DimensionAtCompileTime, _Option>>;             \
			/*friend Map<CLASS_NAME>;                           */                                                     \
			/*friend MapBase<CLASS_NAME>;                       */                                                     \
			/* friend MapBase<DenseBase<CLASS_NAME>>; */ /* Brown: Why i decided to add this line again? */            \
			typedef const CLASS_NAME<_Scalar, _DimensionAtCompileTime, _Option>& Nested;                               \
			typedef const CLASS_NAME<_Scalar, _DimensionAtCompileTime, _Option>& NestedExpression;                     \
			using PlainObject = PlainTriangularObjectBase<CLASS_NAME<_Scalar, _DimensionAtCompileTime, _Option>>;      \
                                                                                                                       \
		public:                                                                                                        \
			CLASS_NAME() = default;                                                                                    \
                                                                                                                       \
			template <typename... Args>                                                                                \
			explicit CLASS_NAME(Args... args) : Base(args...)                                                          \
			{                                                                                                          \
				/* NO CODE*/                                                                                           \
			}                                                                                                          \
                                                                                                                       \
			CLASS_NAME(const CLASS_NAME&) = default;                                                                   \
			CLASS_NAME(CLASS_NAME&&) noexcept = default;                                                               \
			CLASS_NAME& operator=(const CLASS_NAME&) = default;                                                        \
			CLASS_NAME& operator=(CLASS_NAME&&) noexcept = default;                                                    \
			using Base::operator();                                                                                    \
			using Base::operator=;                                                                                     \
			using Base::BufferSize;                                                                                    \
			using Base::rows;                                                                                          \
			using Base::cols;                                                                                          \
			using Base::size;                                                                                          \
			using Base::Data;                                                                                          \
			using Base::RequiredBufferSizeOf;                                                                          \
		};                                                                                                             \
	}                                                                                                                  \
                                                                                                                       \
	namespace Eigen                                                                                                    \
	{                                                                                                                  \
		namespace internal                                                                                             \
		{                                                                                                              \
			template <typename TScalar, int KDimensionAtCompileTime, int KOption>                                      \
			struct traits<Hoppy::CLASS_NAME<TScalar, KDimensionAtCompileTime, KOption>>                                \
			{                                                                                                          \
				using Scalar = TScalar;                                                                                \
				static constexpr int DimensionAtCompileTime = KDimensionAtCompileTime;                                 \
				static constexpr bool IsUpperCritical =                                                                \
				        std::is_same<Hoppy::CLASS_NAME<TScalar, KDimensionAtCompileTime, KOption>,                     \
				                     Hoppy::UpperTriangularMatrix<TScalar, KDimensionAtCompileTime, KOption>>::value;  \
				static constexpr int Flags = DirectAccessBit | LvalueBit | NestByRefBit | RowMajorBit;                 \
				using StorageKind = Hoppy::TriangularCompressed;                                                       \
			};                                                                                                         \
                                                                                                                       \
			template <typename TScalar, int KDimensionAtCompileTime, int KOption>                                      \
			struct traits<const Hoppy::CLASS_NAME<TScalar, KDimensionAtCompileTime, KOption>>                          \
			{                                                                                                          \
				using Scalar = const TScalar;                                                                          \
				static constexpr int DimensionAtCompileTime = KDimensionAtCompileTime;                                 \
				static constexpr bool IsUpperCritical =                                                                \
				        std::is_same<Hoppy::CLASS_NAME<TScalar, KDimensionAtCompileTime, KOption>,                     \
				                     Hoppy::UpperTriangularMatrix<TScalar, KDimensionAtCompileTime, KOption>>::value;  \
				static constexpr int Flags = DirectAccessBit | LvalueBit | NestByRefBit | RowMajorBit;                 \
				using StorageKind = Hoppy::TriangularCompressed;                                                       \
			};                                                                                                         \
                                                                                                                       \
			template <typename TScalar, int KDimensionAtCompileTime, int KOption>                                      \
			struct traits<const Hoppy::CLASS_NAME<TScalar, KDimensionAtCompileTime, KOption>&>                         \
			{                                                                                                          \
				using Scalar = const TScalar;                                                                          \
				static constexpr int DimensionAtCompileTime = KDimensionAtCompileTime;                                 \
				static constexpr bool IsUpperCritical =                                                                \
				        std::is_same<Hoppy::CLASS_NAME<TScalar, KDimensionAtCompileTime, KOption>,                     \
				                     Hoppy::UpperTriangularMatrix<TScalar, KDimensionAtCompileTime, KOption>>::value;  \
				static constexpr int Flags = DirectAccessBit | LvalueBit | NestByRefBit | RowMajorBit;                 \
				using StorageKind = Hoppy::TriangularCompressed;                                                       \
			};                                                                                                         \
		}                                                                                                              \
	}



CREATE_PLAINTRIANGULAROBJECTBASE_BASED_MATRIX_CLASS(UpperTriangularMatrix)
CREATE_PLAINTRIANGULAROBJECTBASE_BASED_MATRIX_CLASS(LowerTriangularMatrix)
CREATE_PLAINTRIANGULAROBJECTBASE_BASED_MATRIX_CLASS(HermitianMatrix)
CREATE_PLAINTRIANGULAROBJECTBASE_BASED_MATRIX_CLASS(AntiHermitianMatrix)
CREATE_PLAINTRIANGULAROBJECTBASE_BASED_MATRIX_CLASS(SymmetricMatrix)
CREATE_PLAINTRIANGULAROBJECTBASE_BASED_MATRIX_CLASS(AntiSymmetricMatrix)

#undef CREATE_PLAINTRIANGULAROBJECTBASE_BASED_MATRIX_CLASS

namespace Eigen
{
	namespace internal
	{
		template <>
		struct storage_kind_to_shape<Hoppy::TriangularCompressed>
		{
			using Shape = Hoppy::TriangularCompressedShape;
		};
	}  // namespace internal
}  // namespace Eigen
