//
// Created by Andy on 7/13/2024.
//

#pragma once

#include "BlockDiagonalMatrix.Forward.hpp"
#include "BlockDiagonalMatrix.Traits.hpp"
#include "NonResizableView.hpp"

#include "ConstexprMacros.hpp"
#include "SubscriptBasedIterator.hpp"
#include <numeric>

namespace Hoppy
{
	namespace Info
	{
		template <typename Derived>
		Index DimensionOf(const MatrixBase<Derived>& block)
		{
			if CONSTEXPR17 (internal::traits<Derived>::RowsAtCompileTime == 1
			                || internal::traits<Derived>::ColsAtCompileTime == 1)
			{
				return block.size();
			}
			else
			{
				eigen_assert(block.rows() == block.cols()
				             && "We can't define dimension for non-square matrix in current context");
				return block.rows();
			}
		}

		template <typename Derived>
		Index ValidElementCountOf(const MatrixBase<Derived>& block)
		{
			return block.size();
		}
	}  // namespace Info


	template <typename Derived>
	class BlockDiagonalMatrixExpr : public EigenBase<BlockDiagonalMatrixExpr<Derived>>
	{
	public:
		using Scalar = typename internal::traits<BlockDiagonalMatrixExpr>::Scalar;
		static constexpr int RowsAtCompileTime = Dynamic;
		static constexpr int ColsAtCompileTime = Dynamic;

		const Derived& derived() const
		{
			return static_cast<const Derived&>(*this);
		}

		Derived& derived()
		{
			return static_cast<Derived&>(*this);
		}

		Index blockCount() const
		{
			return derived().blockCount();
		}

		Index totalDimension() const
		{
			Index dim = 0;

			for (Index i = 0; i < blockCount(); i++)
			{
				dim += dimensionOfBlock(i);
			}

			return dim;
		}

		Index totalValidElementCount() const
		{
			Index count = 0;

			for (const auto& b : *this)
			{
				count += Info::ValidElementCountOf(b);
			}

			return count;
		}

		Index rows() const
		{
			return derived().rows();
		}

		Index cols() const
		{
			return derived().cols();
		}

		decltype(auto) operator[](const Index index)
		{
			return derived()[index];
		}

		decltype(auto) operator[](const Index index) const
		{
			return derived()[index];
		}

		Index dimensionOfBlock(const Index index) const
		{
			return Info::DimensionOf((*this)[index]);
		}

		using Iterator = SecUtility::SubscriptBasedIterator<BlockDiagonalMatrixExpr, Index, Index>;
		using ConstIterator = SecUtility::SubscriptBasedIterator<const BlockDiagonalMatrixExpr, Index, Index>;

		Iterator begin()
		{
			return {*this, 0};
		}

		Iterator end()
		{
			return {*this, blockCount()};
		}

		ConstIterator begin() const
		{
			return {*this, 0};
		}

		ConstIterator end() const
		{
			return {*this, blockCount()};
		}

		ConstIterator cbegin() const
		{
			return {*this, 0};
		}

		ConstIterator cend() const
		{
			return {*this, blockCount()};
		}

		auto eval() const
		{
			return static_cast<BlockDiagonalMatrix<MatrixX<Scalar>>>(derived());
		}

		auto toDense() const
		{
			return static_cast<MatrixX<Scalar>>(*this);
		}

		template <typename Dest>
		void evalTo(Dest& dst) const
		{
			const auto totalDimension = this->totalDimension();
			dst.resize(totalDimension, totalDimension);
			dst.setZero();

			Index offset = 0;
			for (Index i = 0; i < blockCount(); i++)
			{
				const auto& block = (*this)[i];
				const Index dim = block.rows();

				dst.block(offset, offset, dim, dim) = block;

				offset += dim;
			}
		}

		friend std::ostream& operator<<(std::ostream& os, const BlockDiagonalMatrixExpr& e)
		{
			for (Index i = 0; i < e.blockCount(); i++)
			{
				os << e[i] << "\n\n";
			}

			return os;
		}

		template <typename OtherDerived>
		bool isShapeLike(const BlockDiagonalMatrixExpr<OtherDerived>& other) const
		{
			if (blockCount() != other.blockCount())
			{
				return false;
			}

			auto iterator0 = begin();
			auto iterator1 = other.begin();

			while (iterator0 != end())
			{
				if (Info::DimensionOf(*iterator0) != Info::DimensionOf(*iterator1))
				{
					return false;
				}

				iterator0++;
				iterator1++;
			}

			return true;
		}

		template <typename OtherDerived>
		bool isShapeAs(const BlockDiagonalMatrixExpr<OtherDerived>& other) const
		{
			if (blockCount() != other.blockCount())
			{
				return false;
			}

			auto iterator0 = begin();
			auto iterator1 = other.begin();

			while (iterator0 != end())
			{
				if (iterator0->rows() != iterator1->rows() || iterator0->cols() != iterator1->cols())
				{
					return false;
				}

				iterator0++;
				iterator1++;
			}

			return true;
		}


	public:
		template <typename OtherMatrix>
		decltype(auto) transformBy(const OtherMatrix& transformer)
		{
			return transformer * (*this) * transformer.adjoint();
		}

		template <typename OtherMatrix>
		decltype(auto) backTransformBy(const OtherMatrix& transformer)
		{
			return transformer.adjoint() * (*this) * transformer;
		}


	private:
		friend Derived;
		BlockDiagonalMatrixExpr() = default;
	};

	template <typename TBlock>
	class BlockDiagonalMatrix : public BlockDiagonalMatrixExpr<BlockDiagonalMatrix<TBlock>>
	{
	public:
		using BlockType = TBlock;
		using Scalar = typename internal::traits<BlockDiagonalMatrix>::Scalar;
		using RealScalar = typename NumTraits<Scalar>::Real;
		static constexpr int Flags = 0;
		static constexpr bool IsVectorAtCompileTime = BlockType::IsVectorAtCompileTime;
		using Nested = BlockDiagonalMatrix;
		using PlainObject = BlockDiagonalMatrix;
		using NestedExpression = BlockDiagonalMatrix;


		explicit BlockDiagonalMatrix(std::vector<BlockType> blocks) : _blocks(std::move(blocks))
		{
			/* NO CODE */
		}

		BlockDiagonalMatrix(std::initializer_list<BlockType>&& blocks) : _blocks(blocks)
		{
			/* NO CODE */
		}

		template <typename BlockingInfo>
		static BlockDiagonalMatrix CreateFromBlockingInfo(const BlockingInfo& blockingInfo)
		{
			return CreateFromBlockingInfo(std::begin(blockingInfo), std::end(blockingInfo));
		}

		static BlockDiagonalMatrix CreateFromBlockingInfo(const std::initializer_list<Index>& blockingInfo)
		{
			return CreateFromBlockingInfo(std::begin(blockingInfo), std::end(blockingInfo));
		}

		template <typename ForwardIterator>
		static BlockDiagonalMatrix CreateFromBlockingInfo(ForwardIterator blockingInfoBegin,
		                                                  ForwardIterator blockingInfoEnd)
		{
			BlockDiagonalMatrix result{};
			result._blocks.reserve(static_cast<std::size_t>(blockingInfoEnd - blockingInfoBegin));

			Index offset = 0;
			for (/* NO CODE */; blockingInfoBegin != blockingInfoEnd; blockingInfoBegin++)
			{
				result._blocks.emplace_back(*blockingInfoBegin, *blockingInfoBegin);
				offset += *blockingInfoBegin;
			}

			return result;
		}

		template <typename BlockingInfo>
		static BlockDiagonalMatrix Constant(const BlockingInfo& blockingInfo, Scalar constant)
		{
			auto result = CreateFromBlockingInfo(blockingInfo);
			result.setConstant(constant);
			return result;
		}

		template <typename BlockingInfo>
		static BlockDiagonalMatrix Zero(const BlockingInfo& blockingInfo)
		{
			return Constant(blockingInfo, Scalar{0});
		}

		template <typename BlockingInfo>
		static BlockDiagonalMatrix Ones(const BlockingInfo& blockingInfo)
		{
			return Constant(blockingInfo, Scalar{1});
		}

		template <typename BlockingInfo>
		static BlockDiagonalMatrix Random(const BlockingInfo& blockingInfo)
		{
			auto result = CreateFromBlockingInfo(blockingInfo);
			result.setRandom();
			return result;
		}

		static BlockDiagonalMatrix Constant(const std::initializer_list<Index>& blockingInfo, Scalar constant)
		{
			auto result = CreateFromBlockingInfo(blockingInfo);
			result.setConstant(constant);
			return result;
		}

		static BlockDiagonalMatrix Zero(const std::initializer_list<Index>& blockingInfo)
		{
			return Constant(blockingInfo, Scalar{0});
		}

		static BlockDiagonalMatrix Ones(const std::initializer_list<Index>& blockingInfo)
		{
			return Constant(blockingInfo, Scalar{1});
		}

		static BlockDiagonalMatrix Random(const std::initializer_list<Index>& blockingInfo)
		{
			auto result = CreateFromBlockingInfo(blockingInfo);
			result.setRandom();
			return result;
		}

		template <typename OtherMatrix, typename BlockingInfo>
		static BlockDiagonalMatrix ExtractFrom(const OtherMatrix& denseMatrix, const BlockingInfo& blockingInfo)
		{
			return ExtractFrom(denseMatrix, std::begin(blockingInfo), std::end(blockingInfo));
		}

		template <typename OtherMatrix>
		static BlockDiagonalMatrix ExtractFrom(const OtherMatrix& denseMatrix,
		                                       const std::initializer_list<Index>& blockingInfo)
		{
			return ExtractFrom(denseMatrix, std::begin(blockingInfo), std::end(blockingInfo));
		}

		template <typename OtherMatrix, typename ForwardIterator>
		static BlockDiagonalMatrix ExtractFrom(const OtherMatrix& denseMatrix,
		                                       ForwardIterator blockingInfoBegin,
		                                       ForwardIterator blockingInfoEnd)
		{
			eigen_assert(denseMatrix.rows() == denseMatrix.cols() && "Input matrix must be square");
			assert(std::accumulate(blockingInfoBegin, blockingInfoEnd, Eigen::Index{0}) == denseMatrix.rows()
			       && "Can't partition given dense matrix with given blocking pattern");

			BlockDiagonalMatrix result{};
			result._blocks.reserve(blockingInfoEnd - blockingInfoBegin);

			Index offset = 0;
			for (/* NO CODE */; blockingInfoBegin != blockingInfoEnd; blockingInfoBegin++)
			{
				result._blocks.emplace_back(denseMatrix.block(offset, offset, *blockingInfoBegin, *blockingInfoBegin));
				offset += *blockingInfoBegin;
			}

			return result;
		}

		template <typename Derived>
		static BlockDiagonalMatrix ShapeLike(const BlockDiagonalMatrixExpr<Derived>& reference)
		{
			BlockDiagonalMatrix result{};
			const auto n = reference.blockCount();
			result._blocks.reserve(n);

			for (Index i = 0; i < n; i++)
			{
				const auto dim = reference.dimensionOfBlock(i);
				result._blocks.emplace_back(dim, dim);
			}

			return result;
		}

		BlockDiagonalMatrix& setConstant(Scalar constant)
		{
			for (/*PROXY*/ auto b : *this)
			{
				b.setConstant(constant);
			}

			return *this;
		}

		BlockDiagonalMatrix& setRandom()
		{
			for (/*PROXY*/ auto b : *this)
			{
				b.setRandom();
			}

			return *this;
		}

		const BlockType& operator[](const Index index) const
		{
			return _blocks[index];
		}

		NonResizableView<BlockType> operator[](const Index index)
		{
			return NonResizableView<BlockType>{_blocks[index]};
		}

		Index blockCount() const
		{
			return static_cast<Index>(_blocks.size());
		}

		Index rows() const
		{
			return this->totalDimension();
		}

		Index cols() const
		{
			return this->totalDimension();
		}

		template <typename Derived>
		// ReSharper disable once CppNonExplicitConvertingConstructor
		/*IMPLICIT*/ BlockDiagonalMatrix(const BlockDiagonalMatrixExpr<Derived>& rhs)  // NOLINT(*-explicit-constructor)
		{
			_blocks.reserve(rhs.blockCount());

			for (Index i = 0; i < rhs.blockCount(); i++)
			{
				_blocks.emplace_back(rhs.derived()[i]);
			}
		}

		template <typename Derived>
		BlockDiagonalMatrix& operator=(const BlockDiagonalMatrixExpr<Derived>& rhs)
		{
			if (blockCount() != rhs.blockCount())
			{
				_blocks.clear();
				_blocks.reserve(rhs.blockCount());
			}

			for (Index i = 0; i < rhs.blockCount(); i++)
			{
				_blocks.emplace_back(rhs.derived()[i]);
			}
			return *this;
		}

		template <typename Derived>
		BlockDiagonalMatrix& operator+=(const BlockDiagonalMatrixExpr<Derived>& rhs)
		{
			applyBinaryOperation(rhs, std::plus<>());
			return *this;
		}

		template <typename Derived>
		BlockDiagonalMatrix& operator-=(const BlockDiagonalMatrixExpr<Derived>& rhs)
		{
			applyBinaryOperation(rhs, std::minus<>());
			return *this;
		}

		template <typename Derived>
		BlockDiagonalMatrix& operator*=(const BlockDiagonalMatrixExpr<Derived>& rhs)
		{
			applyBinaryOperation(rhs, std::multiplies<>());
			return *this;
		}

		template <typename OtherScalar>
		typename std::enable_if<internal::is_scalar<OtherScalar>::value, BlockDiagonalMatrix&>::type  //
		operator*=(const OtherScalar scalar)
		{
			applyScalarOperation(scalar, std::multiplies<>());
			return *this;
		}

		template <typename OtherScalar>
		typename std::enable_if<internal::is_scalar<OtherScalar>::value, BlockDiagonalMatrix&>::type  //
		operator/=(const OtherScalar scalar)
		{
			applyScalarOperation(scalar, std::divides<>());
			return *this;
		}


	private:
		template <typename Derived, typename Op>
		void applyBinaryOperation(const BlockDiagonalMatrixExpr<Derived>& rhs, Op op)
		{
			if (blockCount() != rhs.derived().blockCount())
			{
				throw std::runtime_error("Blocking mismatch in binary operation");
			}
			for (Index i = 0; i < blockCount(); i++)
			{
				_blocks[i] = op(_blocks[i], rhs.derived()[i]);
			}
		}

		template <typename OtherScalar, typename Op>
		void applyScalarOperation(OtherScalar scalar, Op op)
		{
			for (Index i = 0; i < blockCount(); i++)
			{
				_blocks[i] = op(_blocks[i], scalar);
			}
		}


	private:
		std::vector<BlockType> _blocks;
	};


	template <typename Scalar>
	using BlockDiagonalMatrixX = BlockDiagonalMatrix<MatrixX<Scalar>>;

	using BlockDiagonalMatrixXi = BlockDiagonalMatrixX<int>;
	using BlockDiagonalMatrixXl = BlockDiagonalMatrixX<long>;
	using BlockDiagonalMatrixXd = BlockDiagonalMatrixX<double>;
	using BlockDiagonalMatrixXf = BlockDiagonalMatrixX<float>;
	using BlockDiagonalMatrixXcd = BlockDiagonalMatrixX<std::complex<double>>;
	using BlockDiagonalMatrixXcf = BlockDiagonalMatrixX<std::complex<float>>;
}  // namespace Hoppy
