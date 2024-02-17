//
// Created by Andy on 2/13/2024.
//

// ReSharper disable CppRedundantNamespaceDefinition
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


namespace Eigen
{
	namespace internal
	{
		template <int ProductTag>
		struct product_promote_storage_type<Hoppy::TriangularCompressed, Hoppy::TriangularCompressed, ProductTag>
		{
			using ret = Dense;
		};

		template <typename T>
		struct eval<T, Hoppy::TriangularCompressed>
		{
			using type = typename plain_matrix_type<T>::type;
		};
	}  // namespace internal
}  // namespace Eigen


#if defined(CREATE_PLAINTRIANGULAROBJECTBASE_BASED_MATRIX_CLASS)
#undef CREATE_PLAINTRIANGULAROBJECTBASE_BASED_MATRIX_CLASS
#endif

#define CREATE_PLAINTRIANGULAROBJECTBASE_BASED_MATRIX_CLASS(CLASS_NAME)                                                \
	namespace Hoppy                                                                                                    \
	{                                                                                                                  \
		template <typename _Scalar, int _DimensionAtCompileTime, int _Option>                                          \
		class CLASS_NAME                                                                                               \
		    : public PlainTriangularCompressedObjectBase<CLASS_NAME<_Scalar, _DimensionAtCompileTime, _Option>>        \
		{                                                                                                              \
		public:                                                                                                        \
			using Base = PlainTriangularCompressedObjectBase<CLASS_NAME<_Scalar, _DimensionAtCompileTime, _Option>>;   \
			typedef const CLASS_NAME<_Scalar, _DimensionAtCompileTime, _Option>& Nested;                               \
			typedef const CLASS_NAME<_Scalar, _DimensionAtCompileTime, _Option>& NestedExpression;                     \
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
		struct CLASS_NAME##Xpr                                                                                         \
		{                                                                                                              \
			/* NO CODE */                                                                                              \
		};                                                                                                             \
                                                                                                                       \
		namespace internal                                                                                             \
		{                                                                                                              \
			template <typename TScalar, int KDimensionAtCompileTime, int KOption>                                      \
			struct traits<Hoppy::CLASS_NAME<TScalar, KDimensionAtCompileTime, KOption>>                                \
			{                                                                                                          \
				using Scalar = TScalar;                                                                                \
				static constexpr int DimensionAtCompileTime = KDimensionAtCompileTime;                                 \
				static constexpr int RowsAtCompileTime = KDimensionAtCompileTime;                                      \
				static constexpr int ColsAtCompileTime = KDimensionAtCompileTime;                                      \
				static constexpr int MaxRowsAtCompileTime = KDimensionAtCompileTime;                                   \
				static constexpr int MaxColsAtCompileTime = KDimensionAtCompileTime;                                   \
				static constexpr int SizeAtCompileTime =                                                               \
				        KDimensionAtCompileTime != Dynamic ? KDimensionAtCompileTime : Dynamic;                        \
				static constexpr int MaxSizeAtCompileTime = SizeAtCompileTime;                                         \
				static constexpr int InnerStrideAtCompileTime = 1;                                                     \
				static constexpr int OuterStrideAtCompileTime = Dynamic;                                               \
				using XprKind = MatrixXpr;                                                                             \
				using SpecificXprKind = CLASS_NAME##Xpr;                                                               \
				using StorageIndex = Eigen::Index;                                                                     \
				static constexpr int Option = KOption;                                                                 \
				static constexpr bool IsUpperCritical =                                                                \
				        std::is_same<Hoppy::CLASS_NAME<TScalar, KDimensionAtCompileTime, KOption>,                     \
				                     Hoppy::UpperTriangularMatrix<TScalar, KDimensionAtCompileTime, KOption>>::value;  \
				static constexpr bool IsVectorAtCompileTime = KDimensionAtCompileTime == 1;                            \
				static constexpr int Flags = DirectAccessBit | LvalueBit | NestByRefBit | RowMajorBit;                 \
				using StorageKind = Hoppy::TriangularCompressed;                                                       \
			};                                                                                                         \
                                                                                                                       \
			template <typename TScalar, int KDimensionAtCompileTime, int KOption>                                      \
			struct traits<const Hoppy::CLASS_NAME<TScalar, KDimensionAtCompileTime, KOption>>                          \
			{                                                                                                          \
				using Scalar = const TScalar;                                                                          \
				static constexpr int DimensionAtCompileTime = KDimensionAtCompileTime;                                 \
				static constexpr int RowsAtCompileTime = KDimensionAtCompileTime;                                      \
				static constexpr int ColsAtCompileTime = KDimensionAtCompileTime;                                      \
				static constexpr int MaxRowsAtCompileTime = KDimensionAtCompileTime;                                   \
				static constexpr int MaxColsAtCompileTime = KDimensionAtCompileTime;                                   \
				static constexpr int SizeAtCompileTime =                                                               \
				        KDimensionAtCompileTime != Dynamic ? KDimensionAtCompileTime : Dynamic;                        \
				static constexpr int MaxSizeAtCompileTime = SizeAtCompileTime;                                         \
				static constexpr int InnerStrideAtCompileTime = 1;                                                     \
				static constexpr int OuterStrideAtCompileTime = Dynamic;                                               \
				using XprKind = MatrixXpr;                                                                             \
				using SpecificXprKind = CLASS_NAME##Xpr;                                                               \
				using StorageIndex = Eigen::Index;                                                                     \
				static constexpr int Option = KOption;                                                                 \
				static constexpr bool IsUpperCritical =                                                                \
				        std::is_same<Hoppy::CLASS_NAME<TScalar, KDimensionAtCompileTime, KOption>,                     \
				                     Hoppy::UpperTriangularMatrix<TScalar, KDimensionAtCompileTime, KOption>>::value;  \
				static constexpr bool IsVectorAtCompileTime = KDimensionAtCompileTime == 1;                            \
				static constexpr int Flags = DirectAccessBit | LvalueBit | NestByRefBit | RowMajorBit;                 \
				using StorageKind = Hoppy::TriangularCompressed;                                                       \
			};                                                                                                         \
                                                                                                                       \
			template <typename TScalar, int KDimensionAtCompileTime, int KOption>                                      \
			struct traits<Hoppy::CLASS_NAME<TScalar, KDimensionAtCompileTime, KOption>&>                               \
			    : traits<Hoppy::CLASS_NAME<TScalar, KDimensionAtCompileTime, KOption>>                                 \
			{                                                                                                          \
				/* NO CODE */                                                                                          \
			};                                                                                                         \
                                                                                                                       \
			template <typename TScalar, int KDimensionAtCompileTime, int KOption>                                      \
			struct traits<const Hoppy::CLASS_NAME<TScalar, KDimensionAtCompileTime, KOption>&>                         \
			    : traits<const Hoppy::CLASS_NAME<TScalar, KDimensionAtCompileTime, KOption>>                           \
			{                                                                                                          \
				/* NO CODE */                                                                                          \
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
