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

		Eigen::MatrixXcd one = hermi * inverse1;
		for (int i = 0; i < one.rows(); i++)
		{
			for (int j = 0; j < one.cols(); j++)
			{
				if (i != j)
				{
					CHECK(std::abs(one(i, j)) < 1e-12);
				}
				else
				{
					CHECK(std::abs(one(i, j) - 1.0) < 1e-12);
				}
			}
		}
	}

	std::cout << hermi << std::endl;
	std::cout << "=======================================================" << std::endl;
}