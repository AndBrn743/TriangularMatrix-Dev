//
// Created by Andy on 7/13/2024.
//

#pragma once

#include "BlockDiagonalMatrix.Impl.hpp"
#include "NonResizableView.Extension.IO.hpp"
#include <SecUtility/IO/File.hpp>

namespace SecUtility::IO
{
	template <typename BlockType>
	struct FileReadWriteStrategy<Hoppy::BlockDiagonalMatrix<BlockType>>
	{
		template <typename TFile>
		static UInt64 ReadFromFile(const IReadable<TFile>& file,
		                           Hoppy::BlockDiagonalMatrix<BlockType>& out_matrix,
		                           Int64 offset,
		                           const IO::DynamicBufferStorageMode mode)
		{
			if (mode == IO::DynamicBufferStorageMode::StoreBufferDataWithSizeInfo)
			{
				std::vector<Int64> dimensions;
				offset = static_cast<Int64>(file.Read(dimensions, offset, mode));
				out_matrix = Hoppy::BlockDiagonalMatrix<BlockType>::CreateFromBlockingInfo(dimensions);
			}

			for (/*PROXY*/ auto b : out_matrix)
			{
				offset = file.Read(b, offset, IO::DynamicBufferStorageMode::StoreBufferDataOnly);
			}

			return offset;
		}

		template <typename TFile>
		static UInt64 WriteToFile(const IWritable<TFile>& file,
		                          const Hoppy::BlockDiagonalMatrix<BlockType>& matrix,
		                          Int64 offset,
		                          const IO::DynamicBufferStorageMode mode)
		{
			if (mode == IO::DynamicBufferStorageMode::StoreBufferDataWithSizeInfo)
			{
				offset = file.Write(matrix.blockCount(), offset);
				for (const auto& b : matrix)
				{
					offset = file.Write(b.rows(), offset);
				}
			}

			for (const auto& b : matrix)
			{
				offset = file.Write(b, offset, IO::DynamicBufferStorageMode::StoreBufferDataOnly);
			}

			return offset;
		}
	};
}  // namespace SecUtility::IO
