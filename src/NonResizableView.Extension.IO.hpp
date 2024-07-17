//
// Created by Andy on 7/13/2024.
//

#pragma once

#include "NonResizableView.hpp"
#include <SecUtility/IO/File.hpp>


namespace SecUtility::IO
{
	template <typename Derived>
	struct FileReadWriteStrategy<Eigen::NonResizableView<Derived>>
	{
		template <typename TFile>
		static UInt64 ReadFromFile(const IReadable<TFile>& file,
								   Eigen::NonResizableView<Derived>& out_matrix,
								   Int64 offset,
								   const IO::DynamicBufferStorageMode mode)
		{
			if (mode == IO::DynamicBufferStorageMode::StoreBufferDataWithSizeInfo)
			{
				Int64 rows;
				Int64 cols;
				offset = static_cast<Int64>(file.Read(rows, offset));
				offset = static_cast<Int64>(file.Read(cols, offset));

				eigen_assert(out_matrix.rows() == rows && out_matrix.cols() == cols
							 && "Cannot read size mismatched data to Eigen::NonResizableView<Derived>");
			}

			return file.Read(out_matrix.nestedExpression().data(), out_matrix.nestedExpression().size(), offset);
		}

		template <typename TFile>
		static UInt64 WriteToFile(const IWritable<TFile>& file,
								  const Eigen::NonResizableView<Derived>& matrix,
								  Int64 offset,
								  const IO::DynamicBufferStorageMode mode)
		{
			return file.Write(matrix.nestedExpression(), offset, mode);
		}
	};
}  // namespace SecUtility::IO
