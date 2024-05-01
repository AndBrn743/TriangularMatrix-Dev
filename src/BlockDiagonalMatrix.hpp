//
// Created by Andy on 4/30/2024.
//

#pragma once
#include "NonResizableView.hpp"
#include <memory>

namespace Hoppy
{
	template <typename BlockType, template <typename TBlock, typename TAlloc> class BlockContainer>
	class BlockDiagonalMatrix;


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
	}  // namespace internal


	template <typename BlockType,
	          template <typename TBlock, typename TAlloc = std::allocator<TBlock>> class BlockContainer =
	                  internal::BlockMatrix::DefaultBlockContainer>
	class BlockDiagonalMatrix
	{
		static_assert(!(BlockType::RowsAtCompileTime == 1 && BlockType::ColsAtCompileTime == 1),
		              "Compile time scalar object cannot be use as BlockDiagonalMatrix blocks");

	public:
		BlockDiagonalMatrix() = default;
		BlockDiagonalMatrix(const BlockDiagonalMatrix&) = default;
		BlockDiagonalMatrix(BlockDiagonalMatrix&&) noexcept = default;
		BlockDiagonalMatrix& operator=(const BlockDiagonalMatrix&) = default;
		BlockDiagonalMatrix& operator=(BlockDiagonalMatrix&&) noexcept = default;

		template <typename BlockSizeList>
		explicit BlockDiagonalMatrix(const BlockSizeList& blockSizes)
		{
			m_Blocks.reserve(blockSizes.size());

			for (const auto size : blockSizes)
			{
				static_assert(std::is_integral<decltype(size)>::value, "Invalid block size list");
				m_Blocks.emplace_back(internal::BlockMatrix::DiagonalBlockCreator<BlockType>::Create(size));
			}
		}

		template <typename T>
		BlockDiagonalMatrix(const std::initializer_list<T>& blockSizes)
		{
			m_Blocks.reserve(blockSizes.size());

			for (const auto size : blockSizes)
			{
				static_assert(std::is_integral<decltype(size)>::value, "Invalid block size list");
				m_Blocks.emplace_back(internal::BlockMatrix::DiagonalBlockCreator<BlockType>::Create(size));
			}
		}

		explicit BlockDiagonalMatrix(BlockType&& block)
		{
			m_Blocks.emplace_back(block);
		}

		explicit BlockDiagonalMatrix(const BlockType& block)
		{
			m_Blocks.emplace_back(block);
		}

		template <typename Derived>
		explicit BlockDiagonalMatrix(Eigen::MatrixBase<Derived>&& block)
		{
			m_Blocks.emplace_back(block);
		}

		template <typename Derived>
		explicit BlockDiagonalMatrix(const Eigen::MatrixBase<Derived>& block)
		{
			m_Blocks.emplace_back(block);
		}

		template <typename OtherBlockType, template <typename, typename> class OtherBlockContainer>
		static BlockDiagonalMatrix PartitionedAs(const BlockDiagonalMatrix<OtherBlockType, OtherBlockContainer>& other)
		{
			BlockDiagonalMatrix result{};

			result.m_Blocks.reserve(other.BlockCount());

			for (const auto& b : other)
			{
				result.m_Blocks.emplace_back(internal::BlockMatrix::DiagonalBlockCreator<BlockType>::Create(
				        internal::BlockMatrix::DimensionOf(b)));
			}

			return result;
		}

		Eigen::NonResizableView<BlockType> operator[](const Eigen::Index blockIndex)
		{
			return Eigen::NonResizableView<BlockType>{m_Blocks[blockIndex]};
		}

		Eigen::NonResizableView<const BlockType> operator[](const Eigen::Index blockIndex) const
		{
			return Eigen::NonResizableView<const BlockType>{m_Blocks[blockIndex]};
		}

		Eigen::Index BlockCount() const
		{
			return m_Blocks.size();
		}

		Eigen::Index TotalDimension() const
		{
			Eigen::Index dim = 0;

			for (const auto& b : m_Blocks)
			{
				dim += internal::BlockMatrix::DimensionOf(b);
			}

			return dim;
		}

		Eigen::Index TotalRows() const
		{
			if (BlockType::CompileTimeRows == 1)
			{
				return 1;
			}

			Eigen::Index dim = 0;

			for (const auto& b : m_Blocks)
			{
				dim += b.rows();
			}

			return dim;
		}

		Eigen::Index TotalColumns() const
		{
			if (BlockType::CompileTimeColumn == 1)
			{
				return 1;
			}

			Eigen::Index dim = 0;

			for (const auto& b : m_Blocks)
			{
				dim += b.cols(0);
			}

			return dim;
		}

		Eigen::Index TotalSize() const
		{
			return TotalRows() * TotalColumns();
		}

		Eigen::Index TotalValidElementCount() const
		{
			Eigen::Index count = 0;

			for (const auto& b : m_Blocks)
			{
				count += internal::BlockMatrix::ValidElementCount(b);
			}

			return count;
		}

		Eigen::MatrixX<typename BlockType::Scalar> ToFullMatrix() const
		{
			Eigen::MatrixX<typename BlockType::Scalar> result =
			        Eigen::MatrixX<typename BlockType::Scalar>::Zero(TotalDimension(), TotalDimension());

			Eigen::Index i0 = 0;
			Eigen::Index j0 = 0;
			for (const auto& b : m_Blocks)
			{
				for (Eigen::Index i = 0; i < b.rows(); i++)
				{
					for (Eigen::Index j = 0; j < b.cols(); j++)
					{
						result(i + i0, j + j0) = b(i, j);
					}
				}

				if (BlockType::CompileTimeRows != 1)
				{
					i0 += b.rows();
				}
				if (BlockType::CompileTimeCols != 1)
				{
					j0 += b.cols();
				}
			}

			return result;
		}

		template <typename OtherBlockType, template <typename, typename> class OtherBlockContainer>
		bool IsPartitionedAs(const BlockDiagonalMatrix<OtherBlockType, OtherBlockContainer>& other)
		{
			if (BlockCount() != other.BlockCount())
			{
				return false;
			}

			for (size_t i = 0; i < BlockCount(); i++)
			{
				if (internal::BlockMatrix::DimensionOf((*this)[i]) != internal::BlockMatrix::DimensionOf(other[i]))
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
				if (internal::BlockMatrix::DimensionOf((*this)[i]) != partitionList.begin()[i])
				{
					return false;
				}
			}

			return true;
		}

		class Iterator
		{
		public:
			Iterator(BlockDiagonalMatrix& matrix, const Eigen::Index index) : r_Matrix(matrix), m_Index(index)
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
			BlockDiagonalMatrix& r_Matrix;
			Eigen::Index m_Index;
		};

		class ConstIterator
		{
		public:
			ConstIterator(const BlockDiagonalMatrix& matrix, const Eigen::Index index)
			    : r_Matrix(matrix), m_Index(index)
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
			const BlockDiagonalMatrix& r_Matrix;
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


		friend std::ostream& operator<<(std::ostream& os, const BlockDiagonalMatrix& matrix)
		{
			for (const auto& b : matrix)
			{
				os << b << '\n';
			}

			return os;
		}


	protected:
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
		    : m_eivec(), m_eivalues(), m_info(InvalidInput), m_isInitialized(false), m_eigenvectorsOk(false)
		{
			/* NO CODE */
		}

		template <typename InputType>
		EIGEN_DEVICE_FUNC explicit SelfAdjointEigenSolver(const EigenBase<InputType>& matrix,
								  int options = ComputeEigenvectors)
		    : m_eivec(), m_eivalues(), m_info(ComputationInfo::Success), m_isInitialized(false), m_eigenvectorsOk(false)
		{
			compute(matrix.derived(), options);
		}

		template <typename BlockType2, template <typename TBlock2, typename TAlloc2> class BlockContainer2>
		SelfAdjointEigenSolver& compute(const Hoppy::BlockDiagonalMatrix<BlockType2, BlockContainer2>& matrix,
						int options = ComputeEigenvectors)
		{
			m_info = ComputationInfo::Success;
			m_eivec = EigenvectorsType::PartitionedAs(matrix);
			m_eivalues = RealVectorType::PartitionedAs(matrix);

			for (size_t i = 0; i < matrix.BlockCount(); i++)
			{
				SelfAdjointEigenSolver<BlockType> es(matrix[i], options);
				m_eivec[i] = es.eigenvectors();
				m_eivalues[i] = es.eigenvalues();
				m_info = std::max(m_info, es.info());
			}

			return *this;
		}


		using EigenvectorsType = Hoppy::BlockDiagonalMatrix<BlockType, BlockContainer>;
		using RealVectorType =
			Hoppy::BlockDiagonalMatrix<VectorX<typename NumTraits<typename BlockType::Scalar>::Real>>;


	private:
		EigenvectorsType m_eivec;
		RealVectorType m_eivalues;
		ComputationInfo m_info;
		bool m_isInitialized;
		bool m_eigenvectorsOk;
	};
}  // namespace Eigen
