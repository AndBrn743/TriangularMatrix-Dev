//
// Created by Andy on 2/15/2024.
//

#pragma once
#include "MapBase4TriangularCompressed.hpp"
#include "TriangularCompressedMatrices.hpp"

namespace Eigen
{
	namespace internal
	{
		template <typename XprType, int BlockRows, int BlockCols /*, bool InnerPanel = true*/>
		class triangular_compressed_block_impl
		    : public Hoppy::TriangularCompressedMatrixBase<Block<XprType, BlockRows, BlockCols>>
		{
		public:
			using Base = TriangularCompressedMatrixBase<Block<XprType, BlockRows, BlockCols>>;
			using Base::derived;
			using Scalar = typename traits<XprType>::Scalar;
			using NestedExpression = typename internal::remove_all<typename XprType::Nested>::type;
			using BlockType = Block<XprType, BlockRows, BlockCols, true>;


			triangular_compressed_block_impl() = delete;


			triangular_compressed_block_impl(XprType& matrix,
			                                 const Index startRow,
			                                 const Index startCol,
			                                 const Index blockRows,
			                                 const Index blockCols)
			    : r_matrix(matrix), m_startRow(startRow), m_startCol(startCol), m_blockRows(blockRows),
			      m_blockCols(blockCols)
			{
				if (startRow < 0 || startCol < 0 || startRow > matrix.rows() || startCol > matrix.cols())
				{
					throw std::runtime_error("Block start row and/or column in is not valid");
				}
				if (blockRows < 1 || blockCols < 1 || startRow + blockRows > matrix.rows()
				    || startCol + blockCols > matrix.cols())
				{
					throw std::runtime_error(
					        "Block row and/or column count in is not valid (non-postive or too large)");
				}
			}


			constexpr auto coeff(const Index row, const Index col) const
			{
				// std::cout << __FILE__ << ":" << __FUNCTION__ << "(" << row << " + " << startRow() << ", " << col
				//           << " + " << startCol() << ")" << std::endl;
				return r_matrix.coeff(row + startRow(), col + startCol());
			}

			constexpr auto coeffRef(const Index row, const Index col)
			{
				// std::cout << __FILE__ << ":" << __FUNCTION__ << "(" << row << " + " << startRow() << ", " << col
				//           << " + " << startCol() << ")" << std::endl;
				return r_matrix.coeffRef(row + startRow(), col + startCol());
			}

			constexpr auto operator()(const Index row, const Index col) const
			{
				assert(row >= 0 && row < derived().rows() && "Row index out of valid range");
				assert(col >= 0 && col < derived().cols() && "Column index out of valid range");
				return coeff(row, col);
			}

			constexpr auto operator()(const Index row, const Index col)
			{
				assert(row >= 0 && row < derived().rows() && "Row index out of valid range");
				assert(col >= 0 && col < derived().cols() && "Column index out of valid range");
				return coeffRef(row, col);
			}

			constexpr Index rows() const
			{
				return m_blockRows;
			}

			constexpr Index cols() const
			{
				return m_blockCols;
			}

			constexpr Index startRow() const
			{
				return m_startRow;
			}

			constexpr Index startCol() const
			{
				return m_startCol;
			}

			constexpr Index blockRows() const
			{
				return m_blockRows;
			}

			constexpr Index blockCols() const
			{
				return m_blockCols;
			}

			Matrix<Scalar, BlockRows, BlockCols> ToFullMatrix() const
			{
				Matrix<Scalar, BlockRows, BlockCols> result(rows(), cols());

				for (Index i = 0; i < rows(); i++)
				{
					for (Index j = 0; j < cols(); j++)
					{
						result.coeffRef(i, j) = coeff(i, j);
					}
				}

				return result;
			}


		protected:
			typename internal::ref_selector<XprType>::non_const_type r_matrix;
			const Index m_startRow;
			const Index m_startCol;
			const internal::variable_if_dynamic<Index, BlockRows> m_blockRows;
			const internal::variable_if_dynamic<Index, BlockCols> m_blockCols;
		};


		template <typename T, int BlockRows, int BlockCols, int InnerPanel>
		struct eval<Block<T, BlockRows, BlockCols, InnerPanel>, Hoppy::TriangularCompressed>
		{
			using type = const Block<T, BlockRows, BlockCols, InnerPanel>&;  // FIXME: should be Eigen::Matrix instead
		};

	}  // namespace internal


	template <typename TriangularCompressedMatrix, int KBlockRows, int KBlockCols, bool InnerPanel>
	class BlockImpl<TriangularCompressedMatrix, KBlockRows, KBlockCols, InnerPanel, Hoppy::TriangularCompressed>
	    : public internal::triangular_compressed_block_impl<TriangularCompressedMatrix, KBlockRows, KBlockCols>
	{
	public:
		using Base = internal::triangular_compressed_block_impl<TriangularCompressedMatrix, KBlockRows, KBlockCols>;
	};

}  // namespace Eigen
