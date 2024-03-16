//
// Created by Andy on 3/16/2024.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include <complex>
#include <iostream>
#include <memory>
#include <string>

#include "../Hoppy.hpp"
#include "../devtools/Tools.hpp"
#include "../src/NonresizableReturnType.hpp"


TEST_CASE("basic", "[BAISC TESTS]")
{
	constexpr int dimension = 5;
	Hoppy::HermitianMatrix<std::complex<double>, Eigen::Dynamic, 0> hermi(dimension);
	SECTION("Demsional Consistency")
	{
		CHECK(hermi.Dimension() == dimension);
		CHECK(hermi.rows() == dimension);
		CHECK(hermi.cols() == dimension);
		CHECK(hermi.size() == dimension * dimension);
	}

	SECTION("Contant Filling")
	{
		hermi.FillWithZero();
		for (int i = 0; i < hermi.rows(); i++)
		{
			for (int j = 0; j < hermi.cols(); j++)
			{
				CHECK(std::abs(hermi(i, j).Get()) < 1e-12);
			}
		}

		hermi.FillWithOne();
		for (int i = 0; i < hermi.rows(); i++)
		{
			for (int j = 0; j < hermi.cols(); j++)
			{
				CHECK(std::abs(hermi(i, j) - 1.0) < 1e-12);
			}
		}

		std::complex<double> magic{3.14, 2.7};
		hermi.FillWith(magic);
		for (int i = 0; i < hermi.rows(); i++)
		{
			for (int j = 0; j < i; j++)
			{
				CHECK(std::abs(hermi(i, j) - magic) < 1e-12);
			}

			CHECK(std::abs(hermi(i, i) - std::real(magic)) < 1e-12);
		}
	}

	SECTION("Random Filling w/ Self Consistent Tests")
	{
		hermi.FillWithRandom();
		for (int i = 0; i < hermi.rows(); i++)
		{
			for (int j = 0; j < hermi.cols(); j++)
			{
				CHECK(std::abs(hermi.coeff(i, j) - std::conj(hermi.coeff(j, i).Get())) < 1e-12);
			}
		}
		for (int i = 0; i < hermi.rows(); i++)
		{
			for (int j = 0; j < hermi.cols(); j++)
			{
				CHECK(std::abs(hermi.coeffRef(i, j) - std::conj(hermi.coeffRef(j, i).Get())) < 1e-12);
			}
		}
		for (int i = 0; i < hermi.rows(); i++)
		{
			for (int j = 0; j < hermi.cols(); j++)
			{
				CHECK(std::abs(hermi.coeffRef(i, j) - std::conj(hermi(j, i).Get())) < 1e-12);
			}
		}
		for (int i = 0; i < hermi.rows(); i++)
		{
			for (int j = 0; j < hermi.cols(); j++)
			{
				CHECK(std::abs(hermi.coeff(i, j) - std::conj(hermi(j, i).Get())) < 1e-12);
			}
		}
	}

	SECTION("Manual Filling")
	{
		const std::complex<double> element42{6.9, 4.2};
		hermi(4, 2) = element42;
		CHECK(std::abs(hermi(4, 2) - element42) < 1e-12);
		CHECK(std::abs(hermi(2, 4) - std::conj(element42)) < 1e-12);
	}

	SECTION("Hermitian-ness Test")
	{
		for (int i = 0; i < hermi.rows(); i++)
		{
			for (int j = 0; j < hermi.cols(); j++)
			{
				CHECK(std::abs(hermi.coeff(i, j) - std::conj(hermi(j, i).Get())) < 1e-12);
			}
		}
	}

	SECTION("To Full Matrix Test: `.ToFullMatrix()`")
	{
		Eigen::MatrixXcd danny = hermi.ToFullMatrix();
		REQUIRE(danny.rows() == hermi.rows());
		REQUIRE(danny.cols() == hermi.cols());
		for (int i = 0; i < hermi.rows(); i++)
		{
			for (int j = 0; j < hermi.cols(); j++)
			{
				CHECK(std::abs(hermi(i, j) - danny(i, j)) < 1e-12);
			}
		}
	}

	SECTION("To Full Matrix Test: `operator=`")
	{
		Eigen::MatrixXcd danny = hermi;
		REQUIRE(danny.rows() == hermi.rows());
		REQUIRE(danny.cols() == hermi.cols());
		for (int i = 0; i < hermi.rows(); i++)
		{
			for (int j = 0; j < hermi.cols(); j++)
			{
				CHECK(std::abs(hermi(i, j) - danny(i, j)) < 1e-12);
			}
		}
	}

	SECTION("To Full Matrix Test: `static_cast<...>`")
	{
		Eigen::MatrixXcd danny = static_cast<Eigen::MatrixXcd>(hermi);
		REQUIRE(danny.rows() == hermi.rows());
		REQUIRE(danny.cols() == hermi.cols());
		for (int i = 0; i < hermi.rows(); i++)
		{
			for (int j = 0; j < hermi.cols(); j++)
			{
				CHECK(std::abs(hermi(i, j) - danny(i, j)) < 1e-12);
			}
		}
	}

	SECTION("Copy From Full Hermitan Matrix Test")
	{
		Eigen::MatrixXcd danny = Eigen::MatrixXcd::Random(7, 7);
		danny += danny.conjugate().transpose().eval();
		hermi = danny;
		CHECK(hermi.rows() == danny.rows());
		CHECK(hermi.cols() == danny.cols());

		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				CHECK(std::abs(hermi(i, j) - danny(i, j)) < 1e-12);
			}
		}
	}

	SECTION("Copy From Full Non-Hermitan Matrix Test")
	{
		Eigen::MatrixXcd danny = Eigen::MatrixXcd::Random(7, 7);
		hermi = danny;
		CHECK(hermi.rows() == danny.rows());
		CHECK(hermi.cols() == danny.cols());

		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < i; j++)
			{
				CHECK(std::abs(hermi(i, j) - danny(i, j)) < 1e-12);
			}
			CHECK(std::abs(hermi(i, i) - std::real(danny(i, i))) < 1e-12);
		}
	}

	SECTION("Conservative Resize (UP) Test")
	{
		REQUIRE(hermi.Dimension() < 12);
		Eigen::MatrixXcd danny = hermi;
		hermi.ConservativeResize(12);
		CHECK(hermi.rows() == 12);
		CHECK(hermi.cols() == 12);
		CHECK(hermi.size() == 12 * 12);

		for (int i = 0; i < danny.rows(); i++)
		{
			for (int j = 0; j < danny.cols(); j++)
			{
				CHECK(std::abs(hermi(i, j) - danny(i, j)) < 1e-12);
			}
		}
	}

	SECTION("Conservative Resize (DOWN) Test")
	{
		REQUIRE(hermi.Dimension() > 4);
		Eigen::MatrixXcd danny = hermi;
		hermi.ConservativeResize(4);
		CHECK(hermi.rows() == 4);
		CHECK(hermi.cols() == 4);
		CHECK(hermi.size() == 4 * 4);

		for (int i = 0; i < hermi.rows(); i++)
		{
			for (int j = 0; j < hermi.cols(); j++)
			{
				CHECK(std::abs(hermi(i, j) - danny(i, j)) < 1e-12);
			}
		}
	}

	SECTION("Resize Test")
	{
		hermi.Resize(9);
		CHECK(hermi.rows() == 9);
		CHECK(hermi.cols() == 9);
		CHECK(hermi.size() == 9 * 9);
	}

	SECTION("Matrix Multiplication Test")
	{
		Eigen::MatrixXcd dennis = hermi;
		Eigen::MatrixXcd left = Eigen::MatrixXcd::Random(10, dimension);
		Eigen::MatrixXcd right = Eigen::MatrixXcd::Random(dimension, 12);
		Eigen::MatrixXcd lm0 = left * dennis;
		Eigen::MatrixXcd rm0 = dennis * right;

		Eigen::MatrixXcd lm1 = left * hermi;
		Eigen::MatrixXcd rm1 = hermi * right;
		CHECK(std::abs((lm0 - lm1).trace()) < 1e-12);
		CHECK(std::abs((rm0 - rm1).trace()) < 1e-12);

		std::cout << "-left * hermi:\n" << -left * hermi << std::endl;
		std::cout << "-hermi * right:\n" << -hermi * right << std::endl;
		Eigen::MatrixXcd nlm1 = -left * hermi;
		Eigen::MatrixXcd nrm1 = -hermi * right;
		CHECK(std::abs((lm0 + nlm1).trace()) < 1e-12);
		CHECK(std::abs((rm0 + nrm1).trace()) < 1e-12);

		Eigen::MatrixXcd square0 = dennis * dennis;
		Eigen::MatrixXcd square1 = hermi * hermi;
		std::cout << "-hermi * hermi:\n" << -hermi * hermi << std::endl;
		Eigen::MatrixXcd nsquare1 = -hermi * hermi;
		CHECK(std::abs((square0 - square1).trace()) < 1e-12);
		CHECK(std::abs((square0 + nsquare1).trace()) < 1e-12);
	}

	SECTION("Calculate Inverse")
	{
		Eigen::MatrixXcd inverse0 = hermi.ToFullMatrix().inverse();
		Eigen::MatrixXcd inverse1 = hermi.Inverse();
		REQUIRE(inverse0.rows() == hermi.rows());
		REQUIRE(inverse0.cols() == hermi.cols());
		CHECK(inverse1.rows() == inverse0.rows());
		CHECK(inverse1.cols() == inverse0.cols());

		for (int i = 0; i < inverse0.rows(); i++)
		{
			for (int j = 0; j < inverse0.cols(); j++)
			{
				CHECK(std::abs(inverse0(i, j) - inverse1(i, j)) < 1e-12);
			}
		}

		Eigen::MatrixXcd zero = hermi * inverse1 - Eigen::MatrixXd::Identity(dimension, dimension);
		CHECK(std::abs(zero.trace()) < 1e-12);
	}

	SECTION("Calculate Matrix Sqrt")
	{
		Eigen::MatrixXcd sqrt = hermi.Sqrt<std::complex<double>>();
		Eigen::MatrixXcd self = sqrt * sqrt;

		for (int i = 0; i < dimension; i++)
		{
			for (int j = 0; j < dimension; j++)
			{
				CHECK(std::abs(hermi(i, j) - self(i, j)) < 1e-12);
			}
		}

		Eigen::MatrixXcd zero = sqrt * hermi.Inverse() * sqrt - Eigen::MatrixXd::Identity(dimension, dimension);
		CHECK(std::abs(zero.trace()) < 1e-12);
	}

	SECTION("Calculate Matrix Inverse Sqrt")
	{
		Eigen::MatrixXcd inverse0 = hermi.Inverse();
		Eigen::MatrixXcd sqrt = hermi.Sqrt<std::complex<double>>();
		Eigen::MatrixXcd inverseSqrt = hermi.InverseSqrt<std::complex<double>>();
		Eigen::MatrixXcd inverse2 = inverseSqrt * inverseSqrt;

		for (int i = 0; i < dimension; i++)
		{
			for (int j = 0; j < dimension; j++)
			{
				CHECK(std::abs(inverse0(i, j) - inverse2(i, j)) < 1e-12);
			}
		}

		Eigen::MatrixXcd zero = sqrt * inverseSqrt - Eigen::MatrixXd::Identity(dimension, dimension);
		CHECK(std::abs(zero.trace()) < 1e-12);

		zero = inverseSqrt * sqrt - Eigen::MatrixXd::Identity(dimension, dimension);
		CHECK(std::abs(zero.trace()) < 1e-12);

		zero = inverseSqrt * hermi * inverseSqrt - Eigen::MatrixXd::Identity(dimension, dimension);
		CHECK(std::abs(zero.trace()) < 1e-12);
	}

	std::cout << hermi << std::endl;
	std::cout << "=======================================================" << std::endl;
}


TEST_CASE("external memory", "[BASIC TESTS]")
{
	Eigen::ArrayXcd buffer = Eigen::ArrayXcd::Random(128);
	std::cout << "BUFFER: " << buffer.transpose() << std::endl;

	constexpr int dimension = 5;
	Hoppy::Map<Hoppy::HermitianMatrixXcd> hermi(buffer.data(), dimension);

	SECTION("Demsional Consistency")
	{
		CHECK(hermi.Dimension() == dimension);
		CHECK(hermi.rows() == dimension);
		CHECK(hermi.cols() == dimension);
		CHECK(hermi.size() == dimension * dimension);
	}
	SECTION("Check If Data was Mapped Correctly")
	{
		double delta = 0;
		for (int i = 0; i < dimension; i++)
		{
			for (int j = i; j < dimension; j++)
			{
				if (i != j)
				{
					delta += std::pow(std::abs(buffer.data()[Regin0(i, j)] - hermi(i, j)), 2);
				}
				else
				{
					delta += std::pow(std::abs(std::real(buffer.data()[Regin0(i, j)]) - hermi(i, j)), 2);
				}
			}
		}
		CHECK(std::sqrt(delta) < 1e-12);
	}

	std::cout << "mate, the matrix that was using external memory:\n" << hermi << std::endl;
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}
