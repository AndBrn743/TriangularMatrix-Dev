//
// Created by Andy on 9/9/2023.
//

#include <complex>
#include <iostream>

#include "Hoppy.hpp"
#include "devtools/Tools.hpp"
#include "src/BlockDiagonalMatrix.hpp"

#include "src/BlockDiagonalMatrix.hpp"




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
	std::cout << hermi.size() << ": " << hermi.rows() << " x " << hermi.cols() << std::endl;
	std::cout << TypeName<decltype(hermi)>() << std::endl;
	std::cout << TypeName<decltype(hermi.derived())>() << std::endl;
	std::cout << hermi << std::endl;

	Eigen::MatrixXd matt = Eigen::MatrixXd::Random(5, 5);
	Eigen::MatrixXd mark = Eigen::MatrixXd::Random(6, 6);
	Eigen::MatrixXd manny = Eigen::MatrixXd::Random(6, 9);
	Eigen::Inverse<Eigen::MatrixXcd> inv(hermi.ToFullMatrix());
	Eigen::MatrixXcd imv = inv;
	std::cout << imv << std::endl;

	std::cout << "matt:\n" << matt << std::endl;
	std::cout << "hermi * matt:\n" << hermi * matt << std::endl;
	std::cout << "hermi * matt:\n" << hermi * matt << std::endl;
	std::cout << "-hermi * matt:\n" << -hermi * matt << std::endl;
	std::cout << TypeName<decltype((-hermi * matt).eval())>() << std::endl;
	std::cout << "(-hermi * matt).eval():\n" << (-hermi * matt).eval() << std::endl;
	std::cout << "matt * hermi:\n" << matt * hermi << std::endl;
	std::cout << "-matt * hermi:\n" << -matt * hermi << std::endl;
	std::cout << TypeName<decltype((-matt * hermi).eval())>() << std::endl;
	std::cout << "(-matt * hermi).eval():\n" << (-matt * hermi).eval() << std::endl;
	std::cout << "hermi * hermi:\n" << hermi * hermi << std::endl;
	std::cout << TypeName<decltype((hermi * hermi).eval())>() << std::endl;
	std::cout << "(hermi * hermi).eval():\n" << (hermi * hermi).eval() << std::endl;
	std::cout << TypeName<decltype((-hermi * hermi).eval())>() << std::endl;
	std::cout << "(hermi * hermi).eval():\n" << (-hermi * hermi).eval() << std::endl;
	std::cout << "hermi.TransformedBy(matt):\n" << hermi.TransformedBy(matt) << std::endl;


	const auto sqrt = hermi.Sqrt();
	std::cout << "hermi:\n" << hermi << std::endl;
	std::cout << "sqrt:\n" << sqrt << std::endl;
	std::cout << "sqrt * sqrt:\n" << sqrt * sqrt << std::endl;

	Hoppy::LowerTriangularMatrixXd lucky(6);
	lucky.FillWithRandom();
	std::cout << "lucky:\n" << lucky << std::endl;




	Eigen::ArrayXcd buffer = Eigen::ArrayXcd::Random(128);
	std::cout << "BUFFER: " << buffer.transpose() << std::endl;
	Hoppy::Map<Hoppy::HermitianMatrixXcd> mate(buffer.data(), 5);
	std::cout << "mate: (" << mate.rows() << " x " << mate.cols() << ")\n"
	          << mate(0, 0) << "  " << mate(0, 1) << "\n"
	          << mate(1, 0) << "  " << mate(1, 1) << std::endl;
	mate(1, 2) = std::complex<double>(1, 2);
	std::cout << "mate:\n" << mate << std::endl;

	Eigen::internal::triangular_compressed_block_impl<decltype(hermi), Eigen::Dynamic, Eigen::Dynamic> bob(
	        hermi, 1, 2, 4, 3);

	std::cout << TypeName<Eigen::internal::eval<Eigen::Block<Hoppy::HermitianMatrixXcd>>::type>() << std::endl;
	std::cout << TypeName<Eigen::internal::eval<Eigen::Block<Eigen::MatrixXcd>>::type>() << std::endl;

	Eigen::MatrixXcd bevo = bob.ToFullMatrix();
	std::cout << "bevo:\n" << bevo << std::endl;
	{
		auto bevo2 = bob.eval();
		std::cout << "bevo2:\n" << bevo2 << std::endl;
	}
	{
		Eigen::MatrixXcd bevo2 = bob;
		std::cout << "bevo2:\n" << bevo2 << std::endl;
	}
	std::cout << TypeName<Eigen::Block<decltype(bevo)>::PointerType>() << std::endl;
	std::cout << TypeName<Eigen::Block<decltype(bevo)>::PointerType>() << std::endl;


	std::cout << "bob: " << bob.rows() << " x " << bob.cols() << ":\n"
	          << bob << "\n"                         //
	          << static_cast<Eigen::MatrixXcd>(bob)  //
	          << " |$| " << bob(0, 0)                //
	          << " |$| " << bob.coeff(0, 0)          //
	          << std::endl;

	Eigen::internal::BlockImpl_dense<Eigen::MatrixXd> robert(matt, 1, 2, 4, 3);
	std::cout << "robert: " << robert.rows() << " x " << robert.cols() << ":\n"
	          << robert                         //
	          << " |%| " << robert(0, 0)        //
	          << " |%| " << robert.coeff(0, 0)  //
	          << std::endl;

	std::cout << TypeName<decltype(hermi.eval())>() << std::endl;
	std::cout << TypeName<decltype(mate.eval())>() << std::endl;

	std::cout
	        << TypeName<Eigen::internal::eval<Eigen::Product<Hoppy::HermitianMatrix<double>, Eigen::MatrixXd>>::type>()
	        << std::endl;
	std::cout << TypeName<Eigen::internal::eval<
	        Eigen::Product<Hoppy::HermitianMatrix<double>, Hoppy::HermitianMatrix<double>>>::type>()
	          << std::endl;
	std::cout << TypeName<
	        Eigen::internal::eval<Eigen::Product<Hoppy::HermitianMatrixXcd, Hoppy::HermitianMatrixXcd>>::type>()
	          << std::endl;
	std::cout << TypeName<
	        Eigen::internal::eval<Eigen::Product<Hoppy::HermitianMatrixXcd, Hoppy::HermitianMatrixXcd, 0>>::type>()
	          << std::endl;

	hermi = matt;
	matt.operator=(mark);
	std::cout << "hermi <- matt:\n" << hermi << std::endl;

	hermi = mark;
	std::cout << "hermi <- mark:\n" << hermi << std::endl;

	hermi.FillWithRandom();

	std::cout << ">> " << TypeName<Eigen::internal::eval<Eigen::Transpose<decltype(matt)>>::type>() << std::endl;
	std::cout << "matt.transpose():\n" << matt.transpose() << std::endl;
	std::cout << "hermi(0, 0):\n" << hermi.coeff(0, 0) << std::endl;
	std::cout << ">> " << TypeName<Eigen::internal::eval<Eigen::Transpose<decltype(hermi)>>::type>() << std::endl;
	std::cout << ">>> " << TypeName<Eigen::internal::ref_selector<decltype(hermi)>::non_const_type>() << std::endl;
	std::cout << ">>> " << TypeName<decltype(hermi)&>() << std::endl;

	std::cout << "decltype(matt.transpose()): " << TypeName<decltype(matt.transpose())>() << std::endl;
	std::cout << "decltype(hermi.Transpose()): " << TypeName<decltype(hermi.transpose())>() << std::endl;
	Eigen::Transpose<decltype(hermi)> ht3(hermi);
	std::cout << "ht3.nestedExpression() is " << ht3.nestedExpression().rows() << " x " << ht3.nestedExpression().cols()
	          << std::endl;
	std::cout << "ht3.nestedExpression():\n" << ht3.nestedExpression() << std::endl;
	std::cout << "hermi.Transpose3()(0, 0):\n" << ht3.coeff(0, 0) << std::endl;
	std::cout << "hermi.Transpose3():\n" << ht3.rows() << " x " << ht3.cols() << ":\n" << ht3 << std::endl;

	std::cout << matt.conjugate() << std::endl;
	std::cout << "hermi.conjugate().coeff(0, 0) = " << hermi.conjugate().coeff(0, 0) << std::endl;
	std::cout << "hermi.conjugate().coeff(0, 1) = " << hermi.conjugate().coeff(0, 1) << std::endl;
	std::cout << "hermi.conjugate().coeff(1, 0) = " << hermi.conjugate().coeff(1, 0) << std::endl;
	std::cout << "hermi.conjugate().coeff(1, 1) = " << hermi.conjugate().coeff(1, 1) << std::endl;
	std::cout << TypeName<decltype(hermi.conjugate())>() << std::endl;
	std::cout << TypeName<Eigen::internal::eval<decltype(hermi.conjugate())>::type>() << std::endl;
	// std::cout << "hermi.conjugate().eval().coeff(1, 1) = " << hermi.conjugate().eval().coeff(1, 1) << std::endl;


	try
	{
		hermi = manny;
		std::cout << "hermi <- manny:\n" << hermi << std::endl;

		class UnexpectedExceptionMissException : std::exception
		{
			/* NO CODE */
		};
		throw UnexpectedExceptionMissException();
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "Hit expected exception: " << e.what() << std::endl;
	}

	std::cout << TypeName<Eigen::internal::evaluator_traits<Hoppy::HermitianMatrix<double>>::Shape>() << std::endl;
	std::cout << TypeName<Eigen::internal::AssignmentKind<
	        Eigen::DenseShape,
	        Eigen::internal::evaluator_traits<Hoppy::HermitianMatrix<double>>::Shape>::Kind>()
	          << std::endl;

	Eigen::MatrixXcd headingSouth(6, 9);
	headingSouth = hermi;
	std::cout << "headingSouth:\n" << headingSouth << std::endl;
	std::cout << "static_cast<Eigen::MatrixXcd>(hermi):\n" << static_cast<Eigen::MatrixXcd>(hermi) << std::endl;

	std::cout << TypeName<Eigen::internal::traits<decltype(hermi)>::StorageKind>() << std::endl;
	// std::cout << TypeName<Eigen::internal::traits<decltype(bob)>::StorageKind>() << std::endl;
	// std::cout << TypeName<Eigen::internal::evaluator_traits<decltype(bob)>::Shape>() << std::endl;
	// headingSouth = bob;



	// Hoppy::BlockDiagonalMatrix2<Eigen::MatrixXd> bdm({3, 4, 5});
	// for (auto b : bdm)
	// {
	// 	// b.setRandom()
	// 	b.setOnes();
	// }
	// std::cout << "bdm:\n" << bdm << std::endl;
	//
	// Hoppy::BlockVector<Eigen::VectorXd> bv = Hoppy::BlockVector<Eigen::VectorXd>::PartitionedAs(bdm);
	// // std::cout << std::boolalpha << Eigen::VectorXd::RowsAtCompileTime
	// std::cout << "bv:\n" << bv << std::endl;
	//
	// Hoppy::BlockVector<Eigen::RowVectorXd> brv = Hoppy::BlockVector<Eigen::RowVectorXd>::PartitionedAs(bdm);
	// // std::cout << std::boolalpha << Eigen::VectorXd::RowsAtCompileTime
	// std::cout << "brv:\n" << brv << std::endl;

	Hoppy::BlockDiagonalMatrix<Eigen::MatrixXd> bdm({3, 4, 5});
	for (auto b : bdm)
	{
		b.setRandom();
		b = b.transpose().eval();
	}
	std::cout << "bdm:\n" << bdm << std::endl;

	Hoppy::BlockDiagonalMatrix<Hoppy::HermitianMatrixXd> bdm2({3, 4, 5});
	for (auto b : bdm2)
	{
		b.FillWithRandom();
	}
	std::cout << "bdm2:\n" << bdm2 << std::endl;

	Eigen::SelfAdjointEigenSolver<Hoppy::BlockDiagonalMatrix<Eigen::MatrixXd>> es;  // (bdm2);
	es.compute(bdm2);
	std::cout << "es.eigenvalues():\n" << es.eigenvalues() << std::endl;
	std::cout << "es.eigenvectors():\n" << es.eigenvectors() << std::endl;

	// static_assert(Eigen::VectorXd::IsVectorAtCompileTime, "fyujk");
	// static_assert(Eigen::RowVectorXd::IsVectorAtCompileTime, "fyujk");
	// static_assert(Eigen::RowVectorXd::RowsAtCompileTime, "fyujk");
	//
	// Hoppy::BlockVector<Eigen::VectorXd> bdv({4, 5, 6});
	// for (auto b : bdv)
	// {
	// 	b.setRandom();
	// 	std::cout << b.transpose() << std::endl;
	// }
	// for (const auto& b : bdv)
	// {
	// 	std::cout << b.transpose() << std::endl;
	// }
	// std::cout << bdv << std::endl;
	//
	// // Hoppy::HermitianMatrixXd h0 = bdm2[0];
	// // std::cout << "h0:\n" << h0 << std::endl;
	// // std::cout << "h0  :\n" << Eigen::NonResizableView<Hoppy::HermitianMatrixXd>(h0) << std::endl;
	// Eigen::VectorXd diag(6);
	// std::cout << diag.diagonal().transpose() << std::endl;
	// Eigen::NonResizableView<decltype(diag)> nsd(diag);

	std::cout << "Tester terminated normally" << std::endl;
}
