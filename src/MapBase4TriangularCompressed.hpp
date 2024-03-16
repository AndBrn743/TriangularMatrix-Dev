//
// Created by Andy on 2/15/2024.
//

#pragma once

namespace Hoppy
{
	template <typename Derived>
	class MapBase4TriangularCompressed : public TriangularCompressedMatrixBase<Derived>
	{
	public:
		using Scalar = typename Eigen::internal::traits<Derived>::Scalar;
		using Base = TriangularCompressedMatrixBase<Derived>;
		using Base::FillWith;
		using Base::IsShapeAs;
		using Base::ResizeAs;
		using Base::operator();
		using Base::operator=;
		typedef const MapBase4TriangularCompressed& Nested;
		static constexpr int DimensionAtCompileTime = Eigen::internal::traits<Derived>::DimensionAtCompileTime;

		MapBase4TriangularCompressed() = default;

		explicit MapBase4TriangularCompressed(Scalar* data, const Eigen::Index dimension)
		    : m_data(data), m_dimension(dimension)
		{
			/* NO CODE */
		}

		explicit MapBase4TriangularCompressed(Scalar* data, const Eigen::Index rows, const Eigen::Index cols)
		    : m_data(data), m_dimension(rows)
		{
			if (rows != cols)
			{
				throw std::runtime_error("Triangular compressed object can only be square");
			}
		}

		[[nodiscard]] Eigen::Index rows() const
		{
			return m_dimension;
		}

		[[nodiscard]] Eigen::Index cols() const
		{
			return m_dimension;
		}

		[[nodiscard]] Eigen::Index size() const
		{
			return rows() * cols();
		}

		[[nodiscard]] Eigen::Index Dimension() const
		{
			return m_dimension;
		}

		[[nodiscard]] const Scalar* data() const
		{
			return m_data;
		}

		Scalar* data()
		{
			return m_data;
		}

		friend std::ostream& operator<<(std::ostream& s, const MapBase4TriangularCompressed& m)
		{
			// NOTE: For reason I haven't yet figure out we need to override operator << defined by TriangularBase here
			Eigen::internal::print_matrix(s, m, EIGEN_DEFAULT_IO_FORMAT);
			return s;
		}


	private:
		Scalar* m_data = nullptr;
		Eigen::internal::variable_if_dynamic<Eigen::Index, DimensionAtCompileTime> m_dimension;
	};

}  // namespace Hoppy
