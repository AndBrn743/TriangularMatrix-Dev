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
					return {dimension};
				}
			};

			template <typename T>
			struct DiagonalBlockCreator<Eigen::RowVectorX<T>>
			{
				static Eigen::RowVectorX<T> Create(const Eigen::Index dimension)
				{
					return {dimension};
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

		BlockDiagonalMatrix(const std::initializer_list<Eigen::Index>& blockSizes)
		{
			m_Blocks.reserve(blockSizes.size());

			for (const auto size : blockSizes)
			{
				static_assert(std::is_integral<decltype(size)>::value, "Invalid block size list");
				m_Blocks.emplace_back(internal::BlockMatrix::DiagonalBlockCreator<BlockType>::Create(size));
			}
		}

		template <typename SingleBlock>
		explicit BlockDiagonalMatrix(BlockType&& block)
		{
			m_Blocks.emplace_back(static_cast<BlockType>(std::forward<decltype(block)>(block)));
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
			Eigen::Index dim = 0;

			for (const auto& b : m_Blocks)
			{
				dim += b.rows();
			}

			return dim;
		}

		Eigen::Index TotalColumns() const
		{
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

				i0 += b.rows();
				j0 += b.cols();
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

		auto begin() const
		{
			return m_Blocks.begin();
		}

		auto end() const
		{
			return m_Blocks.end();
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
