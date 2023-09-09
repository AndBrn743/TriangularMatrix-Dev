//
// Created by Andy on 9/9/2023.
//

#include <iostream>
#include <Eigen/Eigen>

#include "src/TriangularMatrix.hpp"


int main()
{
	Hoppy::HermitianMatrix<double> hermi(5);
	hermi.FillWithRandom();
	hermi.Diagonal() *= 100;
	std::cout << hermi << '\n' << std::endl;
	
	Hoppy::AntiSymmetricMatrix<double> amy(5);
	amy.FillWithRandom();
	std::cout << amy << '\n' << std::endl;
	
	Hoppy::LowerTriangularMatrix<double> donny(5);
	donny.FillWithRandom();
	std::cout << donny << '\n' << std::endl;
	
	auto bertie = donny.Block(1, 1, 2, 3);
	std::cout << bertie << '\n' << std::endl;
	bertie(1, 0) += 100;
	std::cout << bertie << '\n' << std::endl;
	std::cout << donny << '\n' << std::endl;
	
	std::cout << "Tester terminated normally" << std::endl;
}
