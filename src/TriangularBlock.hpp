//
// Created by Andy on 2/15/2024.
//

#pragma once
#include "MapBase4TriangularCompressed.hpp"
#include "TriangularMatrices.hpp"

namespace Eigen
{


	namespace internal
	{
		template <typename XprType, int BlockRows, int BlockCols/*, bool InnerPanel = true*/>
		class triangular_compressed_block_impl
		    : public Hoppy::TriangularMatrixBase<XprType>
		    // : public Hoppy::TriangularBase<triangular_compressed_block_impl<XprType, BlockRows, BlockCols>>
		    // : public Hoppy::MapBase4TriangularCompressed<triangular_compressed_block_impl<XprType, BlockRows, BlockCols>>
		{
		public:
			using Scalar = typename traits<XprType>::Scalar;
			using _MatrixTypeNested = typename internal::remove_all<typename XprType::Nested>::type;
			using NestedExpression = typename internal::remove_all<typename XprType::Nested>::type;
			// using BlockType = Block<XprType, BlockRows, BlockCols, true>;
			// using Base = Hoppy::TriangularBase<BlockType>;
			// using Base = Hoppy::MapBase4TriangularCompressed<triangular_compressed_block_impl<XprType, BlockRows, BlockCols>>;
			using Base = Hoppy::TriangularMatrixBase<XprType>;
			// using Base = Hoppy::TriangularBase<triangular_compressed_block_impl>;
			using Base::derived;

			triangular_compressed_block_impl(
			        XprType& matrix, const int startRow, const int startCol, int blockRows, int blockCols)
			    : r_matrix(matrix), m_startRow(startRow), m_startCol(startCol), m_blockRows(blockRows),
			      m_blockCols(blockCols)
			{
				/* NO CODE */
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


		protected:
			typename internal::ref_selector<XprType>::non_const_type r_matrix;
			const Index m_startRow;
			const Index m_startCol;
			const internal::variable_if_dynamic<Index, BlockRows> m_blockRows;
			const internal::variable_if_dynamic<Index, BlockCols> m_blockCols;
		};

	}  // namespace internal

}  // namespace Eigen
