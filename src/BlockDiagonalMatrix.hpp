//
// Created by Andy on 4/30/2024.
//

#pragma once
#include "NonResizableView.hpp"
#include <memory>

namespace Hoppy
{
	template <typename Derived>
	class BlockMatrixBase;

	template <typename Derived>
	class OneDimensionalBlockMatrixBase;

	template <typename BlockType, template <typename TBlock, typename TAlloc> class BlockContainer>
	class BlockDiagonalMatrix;

	template <typename BlockType, template <typename TBlock, typename TAlloc> class BlockContainer>
	class BlockVector;


	namespace internal
	{
		namespace BlockMatrix
		{
			template <typename BlockType, typename Allocator = std::allocator<BlockType>>
			class DefaultBlockContainer : protected std::vector<BlockType, Allocator>
			{
			public:
				using Base = std::vector<BlockType, Allocator>;

				DefaultBlockContainer() = default;
				DefaultBlockContainer(const DefaultBlockContainer&) = default;
				DefaultBlockContainer(DefaultBlockContainer&&) noexcept = default;
				DefaultBlockContainer& operator=(const DefaultBlockContainer&) = default;
				DefaultBlockContainer& operator=(DefaultBlockContainer&&) noexcept = default;

				template <typename... Args>
				explicit DefaultBlockContainer(Args&&... args) : Base(std::forward<decltype(args)>(args)...)
				{
					/* NO CODE */
				}

				DefaultBlockContainer(std::initializer_list<typename Base::value_type> list,
				                      const typename Base::allocator_type& allocator = typename Base::allocator_type())
				    : Base(list, allocator)
				{
					// NO CODE
				}

				BlockType& operator[](const Eigen::Index index)
				{
					return Base::operator[](index);
				}

				const BlockType& operator[](const Eigen::Index index) const
				{
					return Base::operator[](index);
				}

				using Base::begin;
				using Base::cbegin;
				using Base::cend;
				using Base::emplace_back;
				using Base::end;
				using Base::push_back;
				using Base::reserve;
				using Base::resize;
				using Base::size;
			};

			template <typename T>
			struct DiagonalBlockCreator
			{
				static T Create(const Eigen::Index dimension)
				{
					return T{dimension};
				}
			};

			template <typename T>
			struct DiagonalBlockCreator<Eigen::MatrixX<T>>
			{
				static Eigen::MatrixX<T> Create(const Eigen::Index dimension)
				{
					return {dimension, dimension};
				}
			};

			template <typename T>
			struct DiagonalBlockCreator<Eigen::VectorX<T>>
			{
				static Eigen::VectorX<T> Create(const Eigen::Index dimension)
				{
					return Eigen::VectorX<T>{dimension};
				}
			};

			template <typename T>
			struct DiagonalBlockCreator<Eigen::RowVectorX<T>>
			{
				static Eigen::RowVectorX<T> Create(const Eigen::Index dimension)
				{
					return Eigen::RowVectorX<T>{dimension};
				}
			};

			template <typename T>
			Eigen::Index DimensionOf(const Eigen::DenseBase<T>& dense)
			{
				if (dense.row() == 1)
				{
					return dense.cols();
				}
				if (dense.cols() == 1)
				{
					return dense.rows();
				}
				if (dense.rows() == dense.cols())
				{
					return dense.rows();
				}

				throw std::runtime_error("Dimensions of non-square matrix is undefined");
			}

			template <typename T>
			Eigen::Index DimensionOf(const Hoppy::TriangularCompressedBase<T>& triangularCompressed)
			{
				return triangularCompressed.rows();
			}

			template <typename T>
			Eigen::Index ValidElementCount(const Hoppy::TriangularCompressedBase<T>& triangularCompressed)
			{
				return Regin1<2>(triangularCompressed.rows());
			}
		}  // namespace BlockMatrix

		template <typename T>
		struct traits;
	}  // namespace internal


	template <typename Derived>
	class BlockMatrixBase
	{
	public:
		using BlockType = typename internal::traits<Derived>::BlockType;
		using Scalar = typename BlockType::Scalar;
		using BlockContainer = typename internal::traits<Derived>::BlockContainer;
		static bool constexpr IsVector = internal::traits<Derived>::IsVector;
		static bool constexpr IsRowVector = internal::traits<Derived>::IsRowVector;

		Derived& AsDerived()
		{
			return *static_cast<Derived*>(this);
		}

		const Derived& AsDerived() const
		{
			return *static_cast<const Derived*>(this);
		}

		Eigen::NonResizableView<BlockType> operator[](const Eigen::Index blockIndex)
		{
			return Eigen::NonResizableView<BlockType>{Blocks()[blockIndex]};
		}

		Eigen::NonResizableView<const BlockType> operator[](const Eigen::Index blockIndex) const
		{
			return Eigen::NonResizableView<const BlockType>{Blocks()[blockIndex]};
		}

		Eigen::Index BlockCount() const
		{
			return Blocks().size();
		}

		Eigen::Index TotalRows() const
		{
			return AsDerived().TotalRows();
		}

		Eigen::Index TotalColumns() const
		{
			return AsDerived().TotalColumns();
		}

		Eigen::Index TotalSize() const
		{
			return TotalRows() * TotalColumns();
		}

		Eigen::Index TotalValidElementCount() const
		{
			Eigen::Index count = 0;

			for (const auto& b : AsDerived())
			{
				count += internal::BlockMatrix::ValidElementCount(b);
			}

			return count;
		}

		Eigen::Matrix<Scalar, IsRowVector ? 1 : Eigen::Dynamic, IsVector ? 1 : Eigen::Dynamic> ToFullMatrix() const
		{
			return AsDerived().ToFullMatrix();
		}

		template <typename OtherDerived>
		bool IsPartitionedAs(const BlockMatrixBase<OtherDerived>& other)
		{
			return AsDerived().IsPartitionedAs(other);
		}

		template <typename PartitionList>
		bool IsPartitionedAs(const PartitionList& partitionList)
		{
			return AsDerived().IsPartitionedAs(partitionList);
		}

		class Iterator
		{
		public:
			Iterator(BlockMatrixBase& matrix, const Eigen::Index index) : r_Matrix(matrix), m_Index(index)
			{
				/* NO CODE */
			}

			Eigen::NonResizableView<BlockType> operator*()
			{
				return r_Matrix[m_Index];
			}

			Iterator operator++()
			{
				m_Index++;
				return *this;
			}

			Iterator operator++(int)
			{
				m_Index++;
				return *this;
			}

			bool operator!=(const Iterator& other)
			{
				return m_Index != other.m_Index || &r_Matrix != &other.r_Matrix;
			}

		private:
			BlockMatrixBase& r_Matrix;
			Eigen::Index m_Index;
		};

		class ConstIterator
		{
		public:
			ConstIterator(const BlockMatrixBase& matrix, const Eigen::Index index) : r_Matrix(matrix), m_Index(index)
			{
				/* NO CODE */
			}

			Eigen::NonResizableView<const BlockType> operator*()
			{
				return r_Matrix[m_Index];
			}

			ConstIterator operator++()
			{
				m_Index++;
				return *this;
			}

			ConstIterator operator++(int)
			{
				m_Index++;
				return *this;
			}

			bool operator!=(const ConstIterator& other)
			{
				return m_Index != other.m_Index || &r_Matrix != &other.r_Matrix;
			}


		private:
			const BlockMatrixBase& r_Matrix;
			Eigen::Index m_Index;
		};

		ConstIterator begin() const
		{
			return {*this, 0};
		}

		ConstIterator end() const
		{
			return {*this, BlockCount()};
		}

		Iterator begin()
		{
			return {*this, 0};
		}

		Iterator end()
		{
			return {*this, BlockCount()};
		}


		friend std::ostream& operator<<(std::ostream& os, const BlockMatrixBase& matrix)
		{
			for (const auto& b : matrix)
			{
				os << b << '\n';
			}

			return os;
		}


	protected:
		BlockMatrixBase() noexcept = default;


	private:
		BlockContainer& Blocks()
		{
			return AsDerived().Blocks();
		}

		const BlockContainer& Blocks() const
		{
			return AsDerived().Blocks();
		}
	};


	template <typename Derived>
	struct internal::traits<OneDimensionalBlockMatrixBase<Derived>> : internal::traits<Derived>
	{
		/* NO CODE */
	};

	template <typename Derived>
	class OneDimensionalBlockMatrixBase : public BlockMatrixBase<OneDimensionalBlockMatrixBase<Derived>>
	{
	public:
		using Base = BlockMatrixBase<OneDimensionalBlockMatrixBase>;
		friend Base;
		using BlockType = typename internal::traits<Derived>::BlockType;
		using Scalar = typename BlockType::Scalar;
		using BlockContainer = typename internal::traits<Derived>::BlockContainer;
		static bool constexpr IsVector = internal::traits<Derived>::IsVector;
		static bool constexpr IsRowVector = internal::traits<Derived>::IsRowVector;

		using Base::BlockCount;

		Derived& AsDerived()
		{
			return *static_cast<Derived*>(this);
		}

		const Derived& AsDerived() const
		{
			return *static_cast<const Derived*>(this);
		}

		template <typename OtherDerived>
		static Derived PartitionedAs(const OneDimensionalBlockMatrixBase<OtherDerived>& other)
		{
			return Derived::PartitionedAs(other);
		}

		Eigen::NonResizableView<BlockType> Block(const Eigen::Index blockIndex)
		{
			return Eigen::NonResizableView<BlockType>{Blocks()[blockIndex]};
		}

		Eigen::NonResizableView<const BlockType> Block(const Eigen::Index blockIndex) const
		{
			return Eigen::NonResizableView<const BlockType>{Blocks()[blockIndex]};
		}

		Eigen::Index BlockRows(const Eigen::Index index) const
		{
			return Block(index).rows();
		}

		Eigen::Index BlockColumns(const Eigen::Index index) const
		{
			return Block(index).cols();
		}

		Eigen::Index BlockDimension(const Eigen::Index index) const
		{
			return AsDerived().BlockDimension(index);
		}

		Eigen::Index TotalDimension() const
		{
			Eigen::Index dim = 0;

			for (Eigen::Index i = 0; i < BlockCount(); i++)
			{
				dim += BlockDimension(i);
			}

			return dim;
		}

		template <typename OtherDerived>
		bool IsPartitionedAs(const OneDimensionalBlockMatrixBase<OtherDerived>& other)
		{
			if (BlockCount() != other.BlockCount())
			{
				return false;
			}

			for (size_t i = 0; i < BlockCount(); i++)
			{
				if (BlockDimension(i) != other.BlockDimension(i))
				{
					return false;
				}
			}

			return true;
		}

		template <typename PartitionList>
		bool IsPartitionedAs(const PartitionList& partitionList)
		{
			if (BlockCount() != partitionList.size())
			{
				return false;
			}

			for (size_t i = 0; i < BlockCount(); i++)
			{
				if (BlockDimension(i) != partitionList.begin()[i])
				{
					return false;
				}
			}

			return true;
		}


	protected:
		OneDimensionalBlockMatrixBase() noexcept = default;


	private:
		BlockContainer& Blocks()
		{
			return AsDerived().Blocks();
		}

		const BlockContainer& Blocks() const
		{
			return AsDerived().Blocks();
		}
	};


	template <typename TBlock, template <typename TBlock2, typename TAlloc = std::allocator<TBlock2>> class TContainer>
	struct internal::traits<BlockVector<TBlock, TContainer>>
	{
		using BlockType = TBlock;
		using Scalar = typename TBlock::Scalar;
		using BlockContainer = TContainer<TBlock>;
		static constexpr bool IsVector = TBlock::IsVectorAtCompileTime;
		static constexpr bool IsRowVector = TBlock::ColsAtCompileTime == 1;

		static_assert(IsVector, "efcgvhuiytfcghjb");
	};

	template <typename BlockType,
	          template <typename TBlock, typename TAlloc = std::allocator<TBlock>> class BlockContainer =
	                  internal::BlockMatrix::DefaultBlockContainer>
	class BlockVector : public OneDimensionalBlockMatrixBase<BlockVector<BlockType, BlockContainer>>
	{
	public:
		using Base = OneDimensionalBlockMatrixBase<BlockVector>;
		friend Base;
		using Base::Block;
		using Base::TotalDimension;
		static constexpr bool IsVector = internal::traits<BlockVector>::IsVector;
		static constexpr bool IsRowVector = internal::traits<BlockVector>::IsRowVector;

		BlockVector() = default;
		BlockVector(const BlockVector&) = default;
		BlockVector(BlockVector&&) noexcept = default;
		BlockVector& operator=(const BlockVector&) = default;
		BlockVector& operator=(BlockVector&&) noexcept = default;
		~BlockVector() = default;


		template <typename BlockSizeList>
		explicit BlockVector(const BlockSizeList& blockSizes)
		{
			m_Blocks.reserve(blockSizes.size());

			for (const auto size : blockSizes)
			{
				static_assert(std::is_integral<decltype(size)>::value, "Invalid block size list");
				m_Blocks.emplace_back(size);
			}
		}

		template <typename T>
		BlockVector(const std::initializer_list<T>& blockSizes)
		{
			m_Blocks.reserve(blockSizes.size());

			for (const auto size : blockSizes)
			{
				static_assert(std::is_integral<decltype(size)>::value, "Invalid block size list");
				m_Blocks.emplace_back(size);
			}
		}

		template <typename OtherDerived>
		static BlockVector PartitionedAs(const OneDimensionalBlockMatrixBase<OtherDerived>& other)
		{
			BlockVector result{};

			result.m_Blocks.reserve(other.BlockCount());

			for (Eigen::Index i = 0; i < other.BlockCount(); i++)
			{
				result.m_Blocks.emplace_back(other.BlockDimension(i));
			}

			return result;
		}

		Eigen::Index BlockDimension(const Eigen::Index index) const
		{
			return Block(index).size();
		}

		Eigen::Index TotalRows() const
		{
			return IsRowVector ? 1 : TotalDimension();
		}

		Eigen::Index TotalColumns() const
		{
			return IsRowVector ? TotalDimension() : 1;
		}


	private:
		BlockContainer<BlockType>& Blocks()
		{
			return m_Blocks;
		}

		const BlockContainer<BlockType>& Blocks() const
		{
			return m_Blocks;
		}


	private:
		BlockContainer<BlockType> m_Blocks;
	};


	template <typename TBlock, template <typename TBlock2, typename TAlloc = std::allocator<TBlock2>> class TContainer>
	struct internal::traits<BlockDiagonalMatrix<TBlock, TContainer>>
	{
		using BlockType = TBlock;
		using Scalar = typename TBlock::Scalar;
		using BlockContainer = TContainer<TBlock>;
		static constexpr bool IsVector = TBlock::IsVectorAtCompileTime;
		static constexpr bool IsRowVector = TBlock::IsVectorAtCompileTime && TBlock::RowsAtCompileTime == 1;

		static_assert(!IsVector && !IsRowVector, "gfbdsadfv");
	};

	template <typename TBlock,
	          template <typename TBlock2, typename TAlloc = std::allocator<TBlock2>> class BlockContainer =
	                  internal::BlockMatrix::DefaultBlockContainer>
	class BlockDiagonalMatrix : public OneDimensionalBlockMatrixBase<BlockDiagonalMatrix<TBlock, BlockContainer>>
	{
	public:
		using Base = OneDimensionalBlockMatrixBase<BlockDiagonalMatrix>;
		friend Base;
		using BlockType = typename Base::BlockType;
		using Base::Block;
		using Base::BlockCount;
		using Base::TotalDimension;

		BlockDiagonalMatrix() = default;
		BlockDiagonalMatrix(const BlockDiagonalMatrix&) = default;
		BlockDiagonalMatrix(BlockDiagonalMatrix&&) noexcept = default;
		BlockDiagonalMatrix& operator=(const BlockDiagonalMatrix&) = default;
		BlockDiagonalMatrix& operator=(BlockDiagonalMatrix&&) noexcept = default;
		~BlockDiagonalMatrix() = default;


		template <typename BlockSizeList>
		explicit BlockDiagonalMatrix(const BlockSizeList& blockSizes)
		{
			m_Blocks.reserve(blockSizes.size());

			for (const auto size : blockSizes)
			{
				static_assert(std::is_integral<decltype(size)>::value, "Invalid block size list");
				m_Blocks.emplace_back(size, size);
			}
		}

		template <typename T>
		BlockDiagonalMatrix(const std::initializer_list<T>& blockSizes)
		{
			m_Blocks.reserve(blockSizes.size());

			for (const auto size : blockSizes)
			{
				static_assert(std::is_integral<decltype(size)>::value, "Invalid block size list");
				m_Blocks.emplace_back(size, size);
			}
		}

		template <typename OtherDerived>
		static BlockDiagonalMatrix PartitionedAs(const OneDimensionalBlockMatrixBase<OtherDerived>& other)
		{
			BlockDiagonalMatrix result{};

			result.m_Blocks.reserve(other.BlockCount());

			for (Eigen::Index i = 0; i < other.BlockCount(); i++)
			{
				result.m_Blocks.emplace_back(other.BlockDimension(i), other.BlockDimension(i));
			}

			return result;
		}

		Eigen::Index BlockDimension(const Eigen::Index index) const
		{
			assert(Block(index).rows() == Block(index).cols());
			return Block(index).rows();
		}

		Eigen::Index BlockRows(const Eigen::Index index) const
		{
			return BlockDimension(index);
		}

		Eigen::Index BlockColumns(const Eigen::Index index) const
		{
			return BlockDimension(index);
		}

		Eigen::Index TotalRows() const
		{
			return TotalDimension();
		}

		Eigen::Index TotalColumns() const
		{
			return TotalDimension();
		}


	private:
		BlockContainer<BlockType>& Blocks()
		{
			return m_Blocks;
		}

		const BlockContainer<BlockType>& Blocks() const
		{
			return m_Blocks;
		}


	private:
		BlockContainer<BlockType> m_Blocks;
	};
}  // namespace Hoppy


// ReSharper disable once CppRedundantNamespaceDefinition
namespace Eigen
{
	template <typename BlockType, template <typename TBlock, typename TAlloc> class BlockContainer>
	class SelfAdjointEigenSolver<Hoppy::BlockDiagonalMatrix<BlockType, BlockContainer>>
	{
	public:
		SelfAdjointEigenSolver()
		    : m_eigenvectors(), m_eigenvalues(), m_info(InvalidInput), m_isInitialized(false), m_eigenvectorsOk(false)
		{
			/* NO CODE */
		}

		template <typename InputType>
		EIGEN_DEVICE_FUNC explicit SelfAdjointEigenSolver(const EigenBase<InputType>& matrix,
		                                                  const int options = ComputeEigenvectors)
		    : m_eigenvectors(), m_eigenvalues(), m_info(ComputationInfo::Success), m_isInitialized(false),
		      m_eigenvectorsOk(false)
		{
			compute(matrix.derived(), options);
		}

		template <typename BlockType2, template <typename TBlock2, typename TAlloc2> class BlockContainer2>
		SelfAdjointEigenSolver& compute(const Hoppy::BlockDiagonalMatrix<BlockType2, BlockContainer2>& matrix,
		                                const int options = ComputeEigenvectors)
		{
			m_info = ComputationInfo::Success;
			if (options & ComputeEigenvectors)
			{
				m_eigenvectors = EigenvectorsType::PartitionedAs(matrix);
			}
			m_eigenvalues = RealVectorType::PartitionedAs(matrix);

			for (size_t i = 0; i < matrix.BlockCount(); i++)
			{
				SelfAdjointEigenSolver<BlockType> es(matrix[i], options);
				if (options & ComputeEigenvectors)
				{
					m_eigenvectors[i] = es.eigenvectors();
				}
				m_eigenvalues[i] = es.eigenvalues();
				m_info = std::max(m_info, es.info());
			}

			return *this;
		}


		using EigenvectorsType = Hoppy::BlockDiagonalMatrix<BlockType, BlockContainer>;
		using RealVectorType = Hoppy::BlockVector<VectorX<typename NumTraits<typename BlockType::Scalar>::Real>>;

		const EigenvectorsType& eigenvectors() const
		{
			return m_eigenvectors;
		}

		const RealVectorType& eigenvalues() const
		{
			return m_eigenvalues;
		}


	private:
		EigenvectorsType m_eigenvectors;
		RealVectorType m_eigenvalues;
		ComputationInfo m_info;
		bool m_isInitialized;
		bool m_eigenvectorsOk;
	};
}  // namespace Eigen
