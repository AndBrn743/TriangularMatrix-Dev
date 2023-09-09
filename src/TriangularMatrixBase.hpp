//
// Created by Andy on 9/2/2023.
//

#pragma once

#include <Eigen/Dense>
#include <fstream>
#include <random>

#include "TriangularDenseStorage.hpp"


namespace Hoppy
{
	template <typename T>
	struct traits;


	template <typename Derived>
	class TriangularMatrixBase
	{
	public:
		friend Derived;
		using Scalar = typename traits<Derived>::Scalar;


	public:
		TriangularMatrixBase() = default;

		explicit TriangularMatrixBase(long dimension) : m_DataStorage(dimension)
		{
			/* NO CODE */
		}

		inline void FillWith(Scalar s)
		{
			m_DataStorage.FillWith(s);
		}
		
		void FillWithRandom()
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<double> dist(-1, 1);
			
			for (long i = 0; i < Dimension(); ++i)
			{
				for (long j = 0; j < Dimension(); ++j)
				{
					(*static_cast<Derived*>(this))(i, j) = dist(gen);
				}
			}
		}

		inline void Resize(long dimension)
		{
			m_DataStorage.Resize(dimension);
		}

		inline void ConservativeResize(long dimension)
		{
			m_DataStorage.ConservativeResize(dimension);
		}

		inline long Dimension() const
		{
			return m_DataStorage.Dimension();
		}

		inline long RowCount() const
		{
			return m_DataStorage.Dimension();
		}

		inline long ColumnCount() const
		{
			return m_DataStorage.Dimension();
		}


	public:
		Eigen::MatrixX<Scalar> ToFullMatrix() const
		{
			long dim = m_DataStorage.Dimension();
			Eigen::MatrixX<Scalar> result(dim, dim);

			for (long i = 0; i < dim; i++)
			{
				for (long j = 0; j < dim; j++)
				{
					result(i, j) = (*this)(i, j);
				}
			}

			return result;
		}

		inline void operator+=(Scalar s)
		{
			m_DataStorage += s;
		}

		inline void operator-=(Scalar s)
		{
			m_DataStorage -= s;
		}

		inline void operator*=(Scalar s)
		{
			m_DataStorage *= s;
		}

		inline void operator/=(Scalar s)
		{
			m_DataStorage /= s;
		}

		inline void operator+=(const TriangularMatrixBase& m)
		{
			m_DataStorage += m;
		}

		inline void operator-=(const TriangularMatrixBase& m)
		{
			m_DataStorage -= m;
		}

		template <typename OtherDerived>
		friend inline auto operator*(const TriangularMatrixBase& lhs, const TriangularMatrixBase<OtherDerived>& rhs)
		{
			return lhs.ToFullMatrix() * rhs.ToFullMatrix();
		}

		template <typename OtherDerived>
		friend inline auto operator*(const TriangularMatrixBase& lhs, const Eigen::DenseBase<OtherDerived>& rhs)
		{
			return lhs.ToFullMatrix() * rhs;
		}

		template <typename OtherDerived>
		friend inline auto operator*(const Eigen::DenseBase<OtherDerived>& lhs, const TriangularMatrixBase& rhs)
		{
			return lhs * rhs.ToFullMatrix();
		}

		friend inline std::ostream& operator<<(std::ostream& os, const TriangularMatrixBase& m)
		{
			return os << m.ToFullMatrix();
		}

		long Write(std::ofstream& fileStream) const
		{
			auto begin = fileStream.tellp();
			fileStream.write(reinterpret_cast<const char*>(&this->Dimension()), sizeof(decltype(this->Dimension())));
			fileStream.write(reinterpret_cast<const char*>(this->Data()), this->m_DataStorage.Size());
			return static_cast<long>(fileStream.tellp() - begin);
		}

		long Read(std::ifstream& fileStream) const
		{
			auto begin = fileStream.tellg();
			long dim;
			fileStream.read(reinterpret_cast<char*>(&dim), sizeof(long));
			this->Resize(dim);
			fileStream.read(reinterpret_cast<char*>(this->Data()), this->m_DataStorage.Size());
			return static_cast<long>(fileStream.tellg() - begin);
		}



	public:
		struct ScalarProxy;

		inline ScalarProxy operator()(long row, long column)
		{
			return {*static_cast<Derived*>(this), row, column};
		}

		inline Scalar operator()(long row, long column) const
		{
			return static_cast<const Derived*>(this)->Get(row, column);
		}

		struct ColumnProxy;

		inline ColumnProxy Column(long column)
		{
			return {column, *static_cast<Derived*>(this)};
		}

		inline Eigen::VectorX<Scalar> Column(long column) const
		{
			return (Eigen::VectorX<Scalar>){column, *static_cast<Derived*>(this)};
		}

		struct RowProxy;

		inline RowProxy Row(long row)
		{
			return {row, *static_cast<Derived*>(this)};
		}

		inline Eigen::RowVectorX<Scalar> Row(long row) const
		{
			return (Eigen::RowVectorX<Scalar>){row, *static_cast<Derived*>(this)};
		}

		struct DiagonalProxy;

		inline DiagonalProxy Diagonal()
		{
			return DiagonalProxy{*static_cast<Derived*>(this)};
		}

		inline typename Eigen::MatrixX<Scalar>::DiagonalReturnType Diagonal() const
		{
			return (typename Eigen::MatrixX<Scalar>::DiagonalReturnType){*static_cast<Derived*>(this)};
		}

		inline Scalar* Data()
		{
			return m_DataStorage.data();
		}

		inline const Scalar* Data() const
		{
			return m_DataStorage.data();
		}


	public:
		struct ScalarProxy
		{
			ScalarProxy(Derived& matrix, long i, long j) : r_Matrix(matrix), i(i), j(j)
			{
				/* NO CODE */
			}

			inline ScalarProxy& operator=(Scalar value) noexcept
			{
				r_Matrix.Set(i, j, value);
				return *this;
			}

			inline void operator+=(Scalar value) noexcept
			{
				r_Matrix.AdditionAssignment(i, j, value);
			}

			inline void operator-=(Scalar value) noexcept
			{
				r_Matrix.SubtractionAssignment(i, j, value);
			}

			inline void operator*=(Scalar value) noexcept
			{
				r_Matrix.MultiplicationAssignment(i, j, value);
			}

			inline void operator/=(Scalar value) noexcept
			{
				r_Matrix.DivisionAssignment(i, j, value);
			}

			// NOLINTNEXTLINE(google-explicit-constructor)
			inline constexpr operator Scalar() const noexcept
			{
				return r_Matrix.Get(i, j);
			}

			inline constexpr Scalar Eval() const noexcept
			{
				return *this;
			}


		private:
			Derived& r_Matrix;
			long i, j;
		};

		struct RowProxy
		{
			RowProxy(Derived& matrix, long row) : r_Matrix(matrix), m_Row(row)
			{
				/* NO CODE */
			}

			inline RowProxy& operator=(const Eigen::RowVectorX<Scalar>& value) noexcept
			{
				assert(r_Matrix.Dimension() == value.size() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.Set(m_Row, i, value(i));
				}
				return *this;
			}

			inline void operator+=(const Eigen::RowVectorX<Scalar>& value) noexcept
			{
				assert(r_Matrix.Dimension() == value.size() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.AdditionAssignment(m_Row, i, value(i));
				}
			}

			inline void operator-=(const Eigen::RowVectorX<Scalar>& value) noexcept
			{
				assert(r_Matrix.Dimension() == value.size() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.SubtractionAssignment(m_Row, i, value(i));
				}
			}

			inline RowProxy& operator=(const RowProxy& value) noexcept
			{
				assert(r_Matrix.Dimension() == value.r_Matrix.Dimension() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.Set(m_Row, i, value(i));
				}
				return *this;
			}

			inline void operator+=(const RowProxy& value) noexcept
			{
				assert(r_Matrix.Dimension() == value.r_Matrix.Dimension() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.AdditionAssignment(m_Row, i, value(i));
				}
			}

			inline void operator-=(const RowProxy& value) noexcept
			{
				assert(r_Matrix.Dimension() == value.r_Matrix.Dimension() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.SubtractionAssignment(m_Row, i, value(i));
				}
			}

			inline void operator*=(Scalar value) noexcept
			{
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.MultiplicationAssignment(m_Row, i, value);
				}
			}

			inline void operator/=(Scalar value) noexcept
			{
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.DivisionAssignment(m_Row, i, value);
				}
			}

			friend inline Eigen::RowVectorX<Scalar> operator+(const RowProxy& lhs, const RowProxy& rhs)
			{
				return lhs.Eval() + rhs.Eval();
			}

			friend inline Eigen::RowVectorX<Scalar> operator-(const RowProxy& lhs, const RowProxy& rhs)
			{
				return lhs.Eval() - rhs.Eval();
			}

			inline constexpr Scalar operator()(long column) const noexcept
			{
				return r_Matrix.Get(m_Row, column);
			}

			inline constexpr ScalarProxy operator()(long column) noexcept
			{
				return {r_Matrix, m_Row, column};
			}

			// NOLINTNEXTLINE(google-explicit-constructor)
			inline constexpr operator Eigen::RowVectorX<Scalar>() const noexcept
			{
				Eigen::RowVectorX<Scalar> row(r_Matrix.Dimension());
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					row(i) = r_Matrix.Get(m_Row, i);
				}
				return row;
			}

			inline constexpr Eigen::RowVectorX<Scalar> Eval() const noexcept
			{
				return *this;
			}


		private:
			Derived& r_Matrix;
			long m_Row;
		};

		struct ColumnProxy
		{
			ColumnProxy(Derived& matrix, long column) : r_Matrix(matrix), m_Column(column)
			{
				/* NO CODE */
			}

			inline ColumnProxy& operator=(const Eigen::VectorX<Scalar>& value) noexcept
			{
				assert(r_Matrix.Dimension() == value.size() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.Set(i, m_Column, value(i));
				}
				return *this;
			}

			inline void operator+=(const Eigen::VectorX<Scalar>& value) noexcept
			{
				assert(r_Matrix.Dimension() == value.size() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.AdditionAssignment(i, m_Column, value(i));
				}
			}

			inline void operator-=(const Eigen::VectorX<Scalar>& value) noexcept
			{
				assert(r_Matrix.Dimension() == value.size() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.SubtractionAssignment(i, m_Column, value(i));
				}
			}

			inline ColumnProxy& operator=(const ColumnProxy& value) noexcept
			{
				assert(r_Matrix.Dimension() == value.r_Matrix.Dimension() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.Set(i, m_Column, value(i));
				}
				return *this;
			}

			inline void operator+=(const ColumnProxy& value) noexcept
			{
				assert(r_Matrix.Dimension() == value.r_Matrix.Dimension() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.AdditionAssignment(i, m_Column, value(i));
				}
			}

			inline void operator-=(const ColumnProxy& value) noexcept
			{
				assert(r_Matrix.Dimension() == value.r_Matrix.Dimension() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.SubtractionAssignment(i, m_Column, value(i));
				}
			}

			inline void operator*=(Scalar value) noexcept
			{
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.MultiplicationAssignment(i, m_Column, value);
				}
			}

			inline void operator/=(Scalar value) noexcept
			{
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.DivisionAssignment(i, m_Column, value);
				}
			}

			friend inline Eigen::VectorX<Scalar> operator+(const ColumnProxy& lhs, const ColumnProxy& rhs)
			{
				return lhs.Eval() + rhs.Eval();
			}

			friend inline Eigen::VectorX<Scalar> operator-(const ColumnProxy& lhs, const ColumnProxy& rhs)
			{
				return lhs.Eval() - rhs.Eval();
			}

			inline constexpr Scalar operator()(long row) const noexcept
			{
				return r_Matrix.Get(row, m_Column);
			}

			inline ScalarProxy operator()(long row) noexcept
			{
				return {r_Matrix, row, m_Column};
			}

			// NOLINTNEXTLINE(google-explicit-constructor)
			inline constexpr operator Eigen::VectorX<Scalar>() const noexcept
			{
				Eigen::VectorX<Scalar> column(r_Matrix.Dimension());
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					column(i) = r_Matrix.Get(i, m_Column);
				}
				return column;
			}

			inline constexpr Eigen::VectorX<Scalar> Eval() const noexcept
			{
				return *this;
			}

		private:
			Derived& r_Matrix;
			long m_Column;
		};

		struct DiagonalProxy
		{
			explicit DiagonalProxy(Derived& matrix) : r_Matrix(matrix)
			{
				/* NO CODE */
			}

			inline DiagonalProxy& operator=(
			        const typename Eigen::VectorX<Scalar>::DiagonalReturnType& diagonal) noexcept
			{
				assert(r_Matrix.Dimension() == diagonal.size() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.Set(i, i, diagonal(i));
				}
				return *this;
			}

			inline void operator+=(const typename Eigen::VectorX<Scalar>::DiagonalReturnType& diagonal) noexcept
			{
				assert(r_Matrix.Dimension() == diagonal.size() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.AdditionAssignment(i, i, diagonal(i));
				}
			}

			inline void operator-=(const typename Eigen::VectorX<Scalar>::DiagonalReturnType& diagonal) noexcept
			{
				assert(r_Matrix.Dimension() == diagonal.size() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.SubtractionAssignment(i, i, diagonal(i));
				}
			}

			inline DiagonalProxy& operator=(const DiagonalProxy& diagonal) noexcept
			{
				assert(r_Matrix.Dimension() == diagonal.r_Matrix.Dimension() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.Set(i, i, diagonal(i));
				}
			}

			inline void operator+=(const DiagonalProxy& diagonal) noexcept
			{
				assert(r_Matrix.Dimension() == diagonal.r_Matrix.Dimension() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.AdditionAssignment(i, i, diagonal(i));
				}
			}

			inline void operator-=(const DiagonalProxy& diagonal) noexcept
			{
				assert(r_Matrix.Dimension() == diagonal.r_Matrix.Dimension() && "Shape mismatch");
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.SubtractionAssignment(i, i, diagonal(i));
				}
			}

			inline void operator*=(Scalar value) noexcept
			{
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.MultiplicationAssignment(i, i, value);
				}
			}

			inline void operator/=(Scalar value) noexcept
			{
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					r_Matrix.DivisionAssignment(i, i, value);
				}
			}

			inline constexpr Scalar operator()(long i) const noexcept
			{
				return r_Matrix.Get(i, i);
			}

			inline constexpr ScalarProxy operator()(long i) noexcept
			{
				return {r_Matrix, i, i};
			}

			// NOLINTNEXTLINE(google-explicit-constructor)
			inline operator typename Eigen::VectorX<Scalar>::DiagonalReturnType() const noexcept
			{
				Eigen::VectorX<Scalar> diag(r_Matrix.Dimension());
				for (long i = 0; i < r_Matrix.Dimension(); i++)
				{
					diag(i) = r_Matrix.Get(i, i);
				}
				return diag.asDiagonal();
			}

			inline constexpr Eigen::DiagonalMatrix<Scalar, Eigen::Dynamic> Eval() const noexcept
			{
				return *this;
			}


		private:
			Derived& r_Matrix;
		};


	private:
		TriangularDenseStorage<Scalar> m_DataStorage;
	};
}  // namespace Hoppy
