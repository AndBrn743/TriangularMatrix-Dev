//
// Created by Andy on 9/9/2023.
//

#include <iostream>
#include <complex>

#include "Hoppy.hpp"


int main()
{
	// Hoppy::HermitianMatrix<double> hermi(5);
	// hermi.FillWithRandom();
	// hermi.Diagonal() *= 100;
	// std::cout << hermi << '\n' << std::endl;
	//
	// Hoppy::AntiSymmetricMatrix<double> amy(5);
	// amy.FillWithRandom();
	// std::cout << amy << '\n' << std::endl;
	//
	// Hoppy::LowerTriangularMatrix<double> donny(5);
	// donny.FillWithRandom();
	// std::cout << donny << '\n' << std::endl;
	//
	// auto bertie = donny.Block(1, 1, 2, 3);
	// std::cout << bertie << '\n' << std::endl;
	// bertie(1, 0) += 100;
	// std::cout << bertie << '\n' << std::endl;
	// std::cout << donny << '\n' << std::endl;

	Hoppy::HermitianMatrix<std::complex<double>, Eigen::Dynamic, 0> hermi(5);
	hermi.FillWithRandom();
	std::cout << hermi(4, 2) << std::endl;
	hermi(4, 2) = std::complex<double>{6.9, 4.2};
	std::cout << hermi(4, 2) << std::endl;
	std::cout << hermi(2, 4) << std::endl;
	// std::cout << hermi.RowCount() << " x " << hermi.ColumnCount() << std::endl;
	std::cout << hermi.rows() << " x " << hermi.cols() << std::endl;
	std::cout << hermi.size() <<": " << hermi.rows() << " x " << hermi.cols() << std::endl;
	std::cout << hermi << std::endl;

	Eigen::MatrixXd matt = Eigen::MatrixXd::Random(5, 5);
	Eigen::Inverse<Eigen::MatrixXcd> inv(hermi.ToFullMatrix());
	// Eigen::Inverse<Hoppy::HermitianMatrix<std::complex<double>, Eigen::Dynamic, 0>> inv2(hermi);
	Eigen::MatrixXcd imv = inv;
	std::cout << imv << std::endl;

	std::cout << matt << std::endl;
	std::cout << hermi * matt << std::endl;
	std::cout << matt * hermi << std::endl;
	std::cout << hermi * hermi << std::endl;
	std::cout << hermi.TransformedBy(matt) << std::endl;


	const auto sqrt = hermi.Sqrt();
	std::cout << "hermi:\n" << hermi << std::endl;
	std::cout << "sqrt:\n" << sqrt << std::endl;
	std::cout << "sqrt * sqrt:\n" << sqrt * sqrt << std::endl;

	Hoppy::LowerTriangularMatrix<double> lucky(6);
	lucky.FillWithRandom();
	std::cout << "lucky:\n" << lucky << std::endl;
	// std::cout << "lucky:\n" << lucky.Sqrt() << std::endl;


	std::cout << "Tester terminated normally" << std::endl;
}
