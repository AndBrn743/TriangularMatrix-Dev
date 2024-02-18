//
// Created by Andy on 2/13/2024.
//

// ReSharper disable CppUseTypeTraitAlias
#pragma once

namespace Eigen
{
	namespace internal
	{
		template <typename Derived>
		struct CoeffReturnProxyBase;

		template <typename MatrixType>
		struct CoeffReturnProxy;

		template <typename TMatrixType>
		struct traits<CoeffReturnProxy<TMatrixType>>
		{
			using MatrixType = TMatrixType;
			using Scalar = typename std::conditional<std::is_const<MatrixType>::value,
			                                         const typename traits<MatrixType>::Scalar,
			                                         typename traits<MatrixType>::Scalar>::type;
		};

		template <typename TScalar, int... Args>
		struct CoeffReturnProxy<const Hoppy::UpperTriangularMatrix<TScalar, Args...>>
		    : CoeffReturnProxyBase<CoeffReturnProxy<const Hoppy::UpperTriangularMatrix<TScalar, Args...>>>
		{
			using MatrixType = const Hoppy::UpperTriangularMatrix<TScalar, Args...>;
			using Scalar = typename traits<MatrixType>::Scalar;
			using Base = CoeffReturnProxyBase<CoeffReturnProxy>;

			template <typename... Args2>
			explicit CoeffReturnProxy(Args2&&... args) : Base(std::forward<decltype(args)>(args)...)
			{
				/* NO CODE */
			}

			Scalar Get() const
			{
				return this->m_row > this->m_column ? Scalar(0) : this->m_data[Regin0(this->m_row, this->m_column)];
			}
		};

		template <typename TScalar, int... Args>
		struct CoeffReturnProxy<Hoppy::UpperTriangularMatrix<TScalar, Args...>>
		    : CoeffReturnProxy<const Hoppy::UpperTriangularMatrix<TScalar, Args...>>
		{
			using MatrixType = Hoppy::UpperTriangularMatrix<TScalar, Args...>;
			using Scalar = typename traits<MatrixType>::Scalar;
			using Base = CoeffReturnProxy<const MatrixType>;

			template <typename... Args2>
			explicit CoeffReturnProxy(Args2&&... args) : Base(std::forward<decltype(args)>(args)...)
			{
				/* NO CODE */
			}

#if defined(CREATE_ASSIGNMENT_OPERATORS)
#undef CREATE_ASSIGNMENT_OPERATORS
#endif
#define CREATE_ASSIGNMENT_OPERATORS(OPERATOR)                                                                          \
	template <typename OtherScalar> /* NOLINTNEXTLINE(*-unconventional-assign-operator) */                             \
	inline Scalar operator OPERATOR(const OtherScalar s)                                                               \
	{                                                                                                                  \
		if (this->m_row > this->m_column)                                                                              \
		{                                                                                                              \
			/* NO-OP */                                                                                                \
		}                                                                                                              \
		else                                                                                                           \
		{                                                                                                              \
			const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)] OPERATOR s;                         \
		}                                                                                                              \
		return *this;                                                                                                  \
	}

			CREATE_ASSIGNMENT_OPERATORS(=)
			CREATE_ASSIGNMENT_OPERATORS(+=)
			CREATE_ASSIGNMENT_OPERATORS(-=)
			CREATE_ASSIGNMENT_OPERATORS(*=)
			CREATE_ASSIGNMENT_OPERATORS(/=)

#undef CREATE_ASSIGNMENT_OPERATORS
		};

		template <typename TScalar, int... Args>
		struct CoeffReturnProxy<const Hoppy::LowerTriangularMatrix<TScalar, Args...>>
		    : CoeffReturnProxyBase<CoeffReturnProxy<const Hoppy::LowerTriangularMatrix<TScalar, Args...>>>
		{
			using MatrixType = const Hoppy::LowerTriangularMatrix<TScalar, Args...>;
			using Scalar = typename traits<MatrixType>::Scalar;
			using Base = CoeffReturnProxyBase<CoeffReturnProxy>;

			template <typename... Args2>
			explicit CoeffReturnProxy(Args2&&... args) : Base(std::forward<decltype(args)>(args)...)
			{
				/* NO CODE */
			}

			Scalar Get() const
			{
				return this->m_row < this->m_column ? Scalar(0) : this->m_data[Regin0(this->m_row, this->m_column)];
			}
		};

		template <typename TScalar, int... Args>
		struct CoeffReturnProxy<Hoppy::LowerTriangularMatrix<TScalar, Args...>>
		    : CoeffReturnProxy<const Hoppy::LowerTriangularMatrix<TScalar, Args...>>
		{
			using MatrixType = Hoppy::LowerTriangularMatrix<TScalar, Args...>;
			using Scalar = typename traits<MatrixType>::Scalar;
			using Base = CoeffReturnProxy<const MatrixType>;

			template <typename... Args2>
			explicit CoeffReturnProxy(Args2&&... args) : Base(std::forward<decltype(args)>(args)...)
			{
				/* NO CODE */
			}

#if defined(CREATE_ASSIGNMENT_OPERATORS)
#undef CREATE_ASSIGNMENT_OPERATORS
#endif
#define CREATE_ASSIGNMENT_OPERATORS(OPERATOR)                                                                          \
	template <typename OtherScalar> /* NOLINTNEXTLINE(*-unconventional-assign-operator) */                             \
	inline Scalar operator OPERATOR(const OtherScalar s)                                                               \
	{                                                                                                                  \
		if (this->m_row < this->m_column)                                                                              \
		{                                                                                                              \
			/* NO-OP */                                                                                                \
		}                                                                                                              \
		else                                                                                                           \
		{                                                                                                              \
			const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)] OPERATOR s;                         \
		}                                                                                                              \
		return *this;                                                                                                  \
	}

			CREATE_ASSIGNMENT_OPERATORS(=)
			CREATE_ASSIGNMENT_OPERATORS(+=)
			CREATE_ASSIGNMENT_OPERATORS(-=)
			CREATE_ASSIGNMENT_OPERATORS(*=)
			CREATE_ASSIGNMENT_OPERATORS(/=)

#undef CREATE_ASSIGNMENT_OPERATORS
		};

		template <typename TScalar, int... Args>
		struct CoeffReturnProxy<const Hoppy::HermitianMatrix<TScalar, Args...>>
		    : CoeffReturnProxyBase<CoeffReturnProxy<const Hoppy::HermitianMatrix<TScalar, Args...>>>
		{
			using MatrixType = const Hoppy::HermitianMatrix<TScalar, Args...>;
			using Scalar = typename traits<MatrixType>::Scalar;
			using Base = CoeffReturnProxyBase<CoeffReturnProxy>;

			template <typename... Args2>
			explicit CoeffReturnProxy(Args2&&... args) : Base(std::forward<decltype(args)>(args)...)
			{
				/* NO CODE */
			}

			Scalar Get() const
			{
				if (this->m_row > this->m_column)
				{
					return numext::conj(this->m_data[Regin0(this->m_row, this->m_column)]);
				}
				if (this->m_row != this->m_column)
				{
					return this->m_data[Regin0(this->m_row, this->m_column)];
				}
				else
				{
					return numext::real(this->m_data[Regin0(this->m_row, this->m_column)]);
				}
			}
		};

		template <typename TScalar, int... Args>
		struct CoeffReturnProxy<Hoppy::HermitianMatrix<TScalar, Args...>>
		    : CoeffReturnProxy<const Hoppy::HermitianMatrix<TScalar, Args...>>
		{
			using MatrixType = Hoppy::HermitianMatrix<TScalar, Args...>;
			using Scalar = typename traits<MatrixType>::Scalar;
			using Base = CoeffReturnProxy<const MatrixType>;

			template <typename... Args2>
			explicit CoeffReturnProxy(Args2&&... args) : Base(std::forward<decltype(args)>(args)...)
			{
				/* NO CODE */
			}


#if defined(CREATE_ASSIGNMENT_OPERATORS)
#undef CREATE_ASSIGNMENT_OPERATORS
#endif
#define CREATE_ASSIGNMENT_OPERATORS(OPERATOR)                                                                          \
	template <typename OtherScalar> /* NOLINTNEXTLINE(*-unconventional-assign-operator) */                             \
	Scalar operator OPERATOR(const OtherScalar s)                                                                      \
	{                                                                                                                  \
		if (this->m_row > this->m_column)                                                                              \
		{                                                                                                              \
			const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)] OPERATOR Eigen::numext::conj(s);    \
		}                                                                                                              \
		else if (this->m_row == this->m_column)                                                                        \
		{                                                                                                              \
			const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)] OPERATOR Eigen::numext::real(s);    \
		}                                                                                                              \
		else                                                                                                           \
		{                                                                                                              \
			const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)] OPERATOR s;                         \
		}                                                                                                              \
		return this->Get();                                                                                            \
	}

			CREATE_ASSIGNMENT_OPERATORS(=)
			CREATE_ASSIGNMENT_OPERATORS(+=)
			CREATE_ASSIGNMENT_OPERATORS(-=)
			CREATE_ASSIGNMENT_OPERATORS(*=)
			CREATE_ASSIGNMENT_OPERATORS(/=)

#undef CREATE_ASSIGNMENT_OPERATORS
		};

		template <typename TScalar, int... Args>
		struct CoeffReturnProxy<const Hoppy::AntiHermitianMatrix<TScalar, Args...>>
		    : CoeffReturnProxyBase<CoeffReturnProxy<const Hoppy::AntiHermitianMatrix<TScalar, Args...>>>
		{
			using MatrixType = const Hoppy::AntiHermitianMatrix<TScalar, Args...>;
			using Scalar = typename traits<MatrixType>::Scalar;
			using Base = CoeffReturnProxyBase<CoeffReturnProxy>;

			template <typename... Args2>
			explicit CoeffReturnProxy(Args2&&... args) : Base(std::forward<decltype(args)>(args)...)
			{
				/* NO CODE */
			}

			Scalar Get() const
			{
				if (this->m_row > this->m_column)
				{
					return -numext::conj(this->m_data[Regin0(this->m_row, this->m_column)]);
				}
				else
				{
					return this->m_data[Regin0(this->m_row, this->m_column)];
				}
			}
		};

		template <typename TScalar, int... Args>
		struct CoeffReturnProxy<Hoppy::AntiHermitianMatrix<TScalar, Args...>>
		    : CoeffReturnProxy<const Hoppy::AntiHermitianMatrix<TScalar, Args...>>
		{
			using MatrixType = Hoppy::AntiHermitianMatrix<TScalar, Args...>;
			using Scalar = typename traits<MatrixType>::Scalar;
			using Base = CoeffReturnProxy<const MatrixType>;

			template <typename... Args2>
			explicit CoeffReturnProxy(Args2&&... args) : Base(std::forward<decltype(args)>(args)...)
			{
				/* NO CODE */
			}


#if defined(CREATE_ASSIGNMENT_OPERATORS)
#undef CREATE_ASSIGNMENT_OPERATORS
#endif
#define CREATE_ASSIGNMENT_OPERATORS(OPERATOR)                                                                          \
	template <typename OtherScalar> /* NOLINTNEXTLINE(*-unconventional-assign-operator) */                             \
	Scalar operator OPERATOR(const OtherScalar s)                                                                      \
	{                                                                                                                  \
		if (this->m_row > this->m_column)                                                                              \
		{                                                                                                              \
			const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)] OPERATOR Eigen::numext::conj(-s);   \
		}                                                                                                              \
		else if (this->m_row == this->m_column)                                                                        \
		{                                                                                                              \
			const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)] OPERATOR s;                         \
			Eigen::numext::real_ref(const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)]) = 0;       \
		}                                                                                                              \
		else                                                                                                           \
		{                                                                                                              \
			const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)] OPERATOR s;                         \
		}                                                                                                              \
		return this->Get();                                                                                            \
	}

			CREATE_ASSIGNMENT_OPERATORS(=)
			CREATE_ASSIGNMENT_OPERATORS(+=)
			CREATE_ASSIGNMENT_OPERATORS(-=)

#undef CREATE_ASSIGNMENT_OPERATORS

#define CREATE_ASSIGNMENT_OPERATORS(OPERATOR)                                                                          \
	template <typename OtherScalar> /* NOLINTNEXTLINE(*-unconventional-assign-operator) */                             \
	Scalar operator OPERATOR(const OtherScalar s)                                                                      \
	{                                                                                                                  \
		if (this->m_row > this->m_column)                                                                              \
		{                                                                                                              \
			const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)] OPERATOR Eigen::numext::conj(s);    \
		}                                                                                                              \
		else if (this->m_row == this->m_column)                                                                        \
		{                                                                                                              \
			const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)] OPERATOR Eigen::numext::real(s);    \
			Eigen::numext::real_ref(const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)]) = 0;       \
		}                                                                                                              \
		else                                                                                                           \
		{                                                                                                              \
			const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)] OPERATOR s;                         \
		}                                                                                                              \
		return this->Get();                                                                                            \
	}

			CREATE_ASSIGNMENT_OPERATORS(*=)
			CREATE_ASSIGNMENT_OPERATORS(/=)

#undef CREATE_ASSIGNMENT_OPERATORS
		};

		template <typename TScalar, int... Args>
		struct CoeffReturnProxy<const Hoppy::SymmetricMatrix<TScalar, Args...>>
		    : CoeffReturnProxyBase<CoeffReturnProxy<const Hoppy::SymmetricMatrix<TScalar, Args...>>>
		{
			using MatrixType = const Hoppy::SymmetricMatrix<TScalar, Args...>;
			using Scalar = typename traits<MatrixType>::Scalar;
			using Base = CoeffReturnProxyBase<CoeffReturnProxy>;

			template <typename... Args2>
			explicit CoeffReturnProxy(Args2&&... args) : Base(std::forward<decltype(args)>(args)...)
			{
				/* NO CODE */
			}

			Scalar Get() const
			{
				return this->m_data[Regin0(this->m_row, this->m_column)];
			}
		};

		template <typename TScalar, int... Args>
		struct CoeffReturnProxy<Hoppy::SymmetricMatrix<TScalar, Args...>>
		    : CoeffReturnProxy<const Hoppy::SymmetricMatrix<TScalar, Args...>>
		{
			using MatrixType = Hoppy::SymmetricMatrix<TScalar, Args...>;
			using Scalar = typename traits<MatrixType>::Scalar;
			using Base = CoeffReturnProxy<const MatrixType>;

			template <typename... Args2>
			explicit CoeffReturnProxy(Args2&&... args) : Base(std::forward<decltype(args)>(args)...)
			{
				/* NO CODE */
			}

#if defined(CREATE_ASSIGNMENT_OPERATORS)
#undef CREATE_ASSIGNMENT_OPERATORS
#endif
#define CREATE_ASSIGNMENT_OPERATORS(OPERATOR)                                                                          \
	template <typename OtherScalar> /* NOLINTNEXTLINE(*-unconventional-assign-operator) */                             \
	Scalar operator OPERATOR(const OtherScalar s)                                                                      \
	{                                                                                                                  \
		const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)] OPERATOR s;                             \
		return this->Get();                                                                                            \
	}

			CREATE_ASSIGNMENT_OPERATORS(=)
			CREATE_ASSIGNMENT_OPERATORS(+=)
			CREATE_ASSIGNMENT_OPERATORS(-=)
			CREATE_ASSIGNMENT_OPERATORS(*=)
			CREATE_ASSIGNMENT_OPERATORS(/=)

#undef CREATE_ASSIGNMENT_OPERATORS
		};

		template <typename TScalar, int... Args>
		struct CoeffReturnProxy<const Hoppy::AntiSymmetricMatrix<TScalar, Args...>>
		    : CoeffReturnProxyBase<CoeffReturnProxy<const Hoppy::AntiSymmetricMatrix<TScalar, Args...>>>
		{
			using MatrixType = const Hoppy::AntiSymmetricMatrix<TScalar, Args...>;
			using Scalar = typename traits<MatrixType>::Scalar;
			using Base = CoeffReturnProxyBase<CoeffReturnProxy>;

			template <typename... Args2>
			explicit CoeffReturnProxy(Args2&&... args) : Base(std::forward<decltype(args)>(args)...)
			{
				/* NO CODE */
			}

			Scalar Get() const
			{
				return this->m_row > this->m_column ? this->m_data[Regin0(this->m_row, this->m_column)]
				                                    : -this->m_data[Regin0(this->m_row, this->m_column)];
			}
		};

		template <typename TScalar, int... Args>
		struct CoeffReturnProxy<Hoppy::AntiSymmetricMatrix<TScalar, Args...>>
		    : CoeffReturnProxy<const Hoppy::AntiSymmetricMatrix<TScalar, Args...>>
		{
			using MatrixType = Hoppy::AntiSymmetricMatrix<TScalar, Args...>;
			using Scalar = typename traits<MatrixType>::Scalar;
			using Base = CoeffReturnProxy<const MatrixType>;

			template <typename... Args2>
			explicit CoeffReturnProxy(Args2&&... args) : Base(std::forward<decltype(args)>(args)...)
			{
				/* NO CODE */
			}

#if defined(CREATE_ASSIGNMENT_OPERATORS)
#undef CREATE_ASSIGNMENT_OPERATORS
#endif
#define CREATE_ASSIGNMENT_OPERATORS(OPERATOR)                                                                          \
	template <typename OtherScalar> /* NOLINTNEXTLINE(*-unconventional-assign-operator) */                             \
	Scalar operator OPERATOR(const OtherScalar s)                                                                      \
	{                                                                                                                  \
		if (this->m_row > this->m_column)                                                                              \
		{                                                                                                              \
			const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)] OPERATOR s;                         \
		}                                                                                                              \
		else if (this->m_row == this->m_column)                                                                        \
		{                                                                                                              \
			const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)] = Scalar(0);                        \
		}                                                                                                              \
		else                                                                                                           \
		{                                                                                                              \
			const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)] OPERATOR(-s);                       \
		}                                                                                                              \
		return this->Get();                                                                                            \
	}

			CREATE_ASSIGNMENT_OPERATORS(=)
			CREATE_ASSIGNMENT_OPERATORS(+=)
			CREATE_ASSIGNMENT_OPERATORS(-=)

#undef CREATE_ASSIGNMENT_OPERATORS

#define CREATE_ASSIGNMENT_OPERATORS(OPERATOR)                                                                          \
	template <typename OtherScalar> /* NOLINTNEXTLINE(*-unconventional-assign-operator) */                             \
	Scalar operator OPERATOR(const OtherScalar s)                                                                      \
	{                                                                                                                  \
		const_cast<Scalar*>(this->m_data)[Regin0(this->m_row, this->m_column)] OPERATOR s;                             \
		return this->Get();                                                                                            \
	}

			CREATE_ASSIGNMENT_OPERATORS(*=)
			CREATE_ASSIGNMENT_OPERATORS(/=)

#undef CREATE_ASSIGNMENT_OPERATORS
		};

		template <typename MatrixType>
		struct evaluator<Hoppy::PlainTriangularCompressedObjectBase<MatrixType>>
		    : evaluator_base<Hoppy::PlainTriangularCompressedObjectBase<MatrixType>>
		{
			using Scalar = typename traits<MatrixType>::Scalar;
			using CoeffReturnType = CoeffReturnProxy<const MatrixType>;
			using RefCoeffReturnType = CoeffReturnProxy<MatrixType>;
			static constexpr int CoeffReadCost = NumTraits<Scalar>::ReadCost;
			static constexpr int Flags = traits<MatrixType>::Flags;

			explicit evaluator(const MatrixType& m) : cr_matrix(m)
			{
				/* NO CODE */
			}

			CoeffReturnType coeff(Index row, Index column) const
			{
				return CoeffReturnType{cr_matrix, row, column};
			}

			RefCoeffReturnType CoeffRef(Index row, Index column)
			{
				return RefCoeffReturnType{cr_matrix, row, column};
			}

		private:
			const MatrixType& cr_matrix;
		};

		using namespace Hoppy;
#if defined(CREARE_EVALUATOR_FOR_PALINTRIANGULAROBJECTBASE_BASED_CLASS)
#undef CREARE_EVALUATOR_FOR_PALINTRIANGULAROBJECTBASE_BASED_CLASS
#endif
#define CREARE_EVALUATOR_FOR_PALINTRIANGULAROBJECTBASE_BASED_CLASS(CLASS_NAME)                                         \
	template <typename _Scalar, int... _Args>                                                                          \
	struct evaluator<CLASS_NAME<_Scalar, _Args...>>                                                                    \
	    : evaluator<PlainTriangularCompressedObjectBase<CLASS_NAME<_Scalar, _Args...>>>                                \
	{                                                                                                                  \
	public:                                                                                                            \
		using MatrixType = CLASS_NAME<_Scalar, _Args...>;                                                              \
		using Base = evaluator<PlainTriangularCompressedObjectBase<MatrixType>>;                                       \
                                                                                                                       \
		template <typename... Args2>                                                                                   \
		explicit evaluator(Args2&&... args) : Base(std::forward<decltype(args)>(args)...)                              \
		{                                                                                                              \
			/* NO CODE */                                                                                              \
		}                                                                                                              \
	};

		CREARE_EVALUATOR_FOR_PALINTRIANGULAROBJECTBASE_BASED_CLASS(LowerTriangularMatrix)
		CREARE_EVALUATOR_FOR_PALINTRIANGULAROBJECTBASE_BASED_CLASS(UpperTriangularMatrix)
		CREARE_EVALUATOR_FOR_PALINTRIANGULAROBJECTBASE_BASED_CLASS(HermitianMatrix)
		CREARE_EVALUATOR_FOR_PALINTRIANGULAROBJECTBASE_BASED_CLASS(AntiHermitianMatrix)
		CREARE_EVALUATOR_FOR_PALINTRIANGULAROBJECTBASE_BASED_CLASS(SymmetricMatrix)
		CREARE_EVALUATOR_FOR_PALINTRIANGULAROBJECTBASE_BASED_CLASS(AntiSymmetricMatrix)

#undef CREARE_EVALUATOR_FOR_PALINTRIANGULAROBJECTBASE_BASED_CLASS


		template <typename... Args>
		struct evaluator<Map<Args...>> : evaluator_base<Args...>
		{
		private:
			struct EvaluatorData;

		public:
			using MapType = Map<Args...>;
			using MappedObjectType = typename MapType::MappedObjectType;
			using Scalar = typename MapType::Scalar;
			static constexpr bool IsTriangularType =
			        std::is_base_of<TriangularBase<MappedObjectType>, MappedObjectType>::value;
			using CoeffReturnType = typename std::conditional<IsTriangularType,
			                                                  CoeffReturnProxy<const MappedObjectType>,
			                                                  typename MapType::Scalar>::type;
			// using CoeffReturnType = typename MapType::Scalar;
			using RefCoeffReturnType = typename std::
			        conditional<IsTriangularType, CoeffReturnProxy<MappedObjectType>, typename MapType::Scalar&>::type;

			explicit evaluator(const MapType& mapObject) : cr_object(mapObject)
			{
				/* NO CODE */
			}

			[[nodiscard]] Scalar coeff(Index row, Index col) const
			{
				return Coeff_Impl(row, col);
			}

			RefCoeffReturnType CoeffRef(Index row, Index col)
			{
				return CoeffRef_Impl(row, col);
			}


		private:
			template <bool KIsTriangularType = IsTriangularType>
			[[nodiscard]] typename std::enable_if<KIsTriangularType, Scalar>::type Coeff_Impl(Index row,
			                                                                                  Index col) const
			{
				return CoeffReturnType{cr_object.data(), row, col};
			}

			template <bool KIsTriangularType = IsTriangularType>
			[[nodiscard]] typename std::enable_if<!KIsTriangularType, Scalar>::type Coeff_Impl(Index row,
			                                                                                   Index col) const
			{
				return cr_object.data()[row * cr_object.OuterStride() + col];
			}

			template <bool KIsTriangularType = IsTriangularType>
			typename std::enable_if<KIsTriangularType, RefCoeffReturnType>::type CoeffRef_Impl(Index row, Index col)
			{
				return RefCoeffReturnType{cr_object.data(), row, col};
			}

			template <bool KIsTriangularType = IsTriangularType>
			typename std::enable_if<!KIsTriangularType, RefCoeffReturnType>::type CoeffRef_Impl(Index row, Index col)
			{
				return const_cast<Scalar*>(cr_object.data())[row * cr_object.OuterStride() + col];
			}

		private:
			const MapType& cr_object;
		};


		template <typename CoeffReturnProxy>
		struct CoeffReturnProxyBase
		{
			using Scalar = typename traits<CoeffReturnProxy>::Scalar;
			using MatrixType = typename traits<CoeffReturnProxy>::MatrixType;

			CoeffReturnProxyBase() = delete;

			CoeffReturnProxyBase(const Scalar* data, const Index row, const Index column)
			    : m_data(data), m_row(row), m_column(column)
			{
				/* NO CODE */
			}

			CoeffReturnProxyBase(const MatrixType& matrix, const Index row, const Index column)
			    : m_data(matrix.data()), m_row(row), m_column(column)
			{
				/* NO CODE */
			}


			// ReSharper disable once CppNonExplicitConversionOperator
			operator Scalar() const  // NOLINT(*-explicit-constructor)
			{
				return static_cast<const CoeffReturnProxy*>(this)->Get();
			}

			friend std::ostream& operator<<(std::ostream& os, const CoeffReturnProxyBase& c)
			{
				return os << static_cast<const CoeffReturnProxy&>(c).Get();
			}


#if defined(CREATE_EVALUATION_OPERATORS)
#undef CREATE_EVALUATION_OPERATORS
#endif
#define CREATE_EVALUATION_OPERATORS(OPERATOR)                                                                          \
	template <typename T>                                                                                              \
	friend inline auto operator OPERATOR(const CoeffReturnProxyBase& lhs, const T& rhs)                                \
	{                                                                                                                  \
		return static_cast<Scalar>(lhs) OPERATOR rhs;                                                                  \
	}                                                                                                                  \
	template <typename T>                                                                                              \
	friend inline auto operator OPERATOR(const T& lhs, const CoeffReturnProxyBase& rhs)                                \
	{                                                                                                                  \
		return lhs OPERATOR static_cast<Scalar>(rhs);                                                                  \
	}

			CREATE_EVALUATION_OPERATORS(+)
			CREATE_EVALUATION_OPERATORS(-)
			CREATE_EVALUATION_OPERATORS(*)
			CREATE_EVALUATION_OPERATORS(/)

#undef CREATE_EVALUATION_OPERATORS

			// COMPARISON
#if defined(CREATE_COMPARISON_OPERATORS)
#undef CREATE_COMPARISON_OPERATORS
#endif
#define CREATE_COMPARISON_OPERATORS(OPERATOR)                                                                          \
	template <typename T>                                                                                              \
	friend inline bool operator OPERATOR(const CoeffReturnProxyBase& lhs, const T& rhs)                                \
	{                                                                                                                  \
		return static_cast<Scalar>(lhs) OPERATOR rhs;                                                                  \
	}                                                                                                                  \
	template <typename T>                                                                                              \
	friend inline bool operator OPERATOR(const T& lhs, const CoeffReturnProxyBase& rhs)                                \
	{                                                                                                                  \
		return lhs OPERATOR static_cast<Scalar>(rhs);                                                                  \
	}

			CREATE_COMPARISON_OPERATORS(>)
			CREATE_COMPARISON_OPERATORS(<)
			CREATE_COMPARISON_OPERATORS(==)
			CREATE_COMPARISON_OPERATORS(>=)
			CREATE_COMPARISON_OPERATORS(<=)

#undef CREATE_COMPARISON_OPERATORS

		protected:
			const Scalar* m_data;
			const Index m_row;
			const Index m_column;
		};
	}  // namespace internal
}  // namespace Eigen
