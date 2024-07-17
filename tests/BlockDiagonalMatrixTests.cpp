//
// Created by Andy on 7/12/2024.
//

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

class AssertionException : public std::runtime_error
{
public:
	explicit AssertionException(const std::string& message) : std::runtime_error(message)
	{
		/* NO CODE */
	}
};

#define CUSTOM_ASSERT(condition)                                                                                       \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(condition))                                                                                              \
		{                                                                                                              \
			std::ostringstream os;                                                                                     \
			os << "Assertion failed: (" << #condition << ")";                                                          \
			throw AssertionException(os.str());                                                                        \
		}                                                                                                              \
	} while (false)

#define eigen_assert CUSTOM_ASSERT

// #include "../src/BlockDiagonalMatrix.Extensions.IO.hpp"
#include "../src/BlockDiagonalMatrix.hpp"

#include <Eigen/Dense>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>


template <typename Scalar>
Eigen::MatrixX<Scalar> GenerateRandomUnitaryMatrix(const Eigen::Index n)
{
	Eigen::MatrixX<Scalar> q =
	        Eigen::HouseholderQR<Eigen::MatrixX<Scalar>>(Eigen::MatrixX<Scalar>::Random(n, n)).householderQ();

	for (Eigen::Index i = 0; i < n; i++)
	{
		if (q.col(i).norm() != 0)
		{
			q.col(i) /= q.col(i).norm();
		}
	}

	return q;
}


TEST_CASE("[BASIC TESTS]")
{
	using namespace Hoppy;
	using BlockType = BlockDiagonalMatrixXd::BlockType;

	const std::vector<BlockType> blocksA = {BlockType::Random(2, 2), BlockType::Random(3, 3)};
	const std::vector<BlockType> blocksB = {BlockType::Random(2, 2), BlockType::Random(3, 3)};
	const std::vector<BlockType> blocksC = {BlockType::Random(2, 2), BlockType::Random(3, 3)};
	const std::vector<BlockType> blocksD = {BlockType::Random(2, 2), BlockType::Random(3, 3)};

	const BlockDiagonalMatrixXd a(blocksA);
	const BlockDiagonalMatrixXd b(blocksB);
	const BlockDiagonalMatrixXd c(blocksC);
	const BlockDiagonalMatrixXd d(blocksD);

	std::cout << "=========================================" << std::endl;
	SECTION("ctor from std::vector should work")
	{
		CHECK(a.blockCount() == 2);
		CHECK((a[0] - blocksA[0]).norm() == 0);
		CHECK((a[1] - blocksA[1]).norm() == 0);
		CHECK(a.dimensionOfBlock(0) == 2);
		CHECK(a.dimensionOfBlock(1) == 3);
		CHECK(a.totalValidElementCount() == 2 * 2 + 3 * 3);
		std::cout << a << std::endl;
	}
	SECTION("copy-ctor should work")
	{
		BlockDiagonalMatrixXd e = a;
		CHECK(e.blockCount() == a.blockCount());
		CHECK((a[0] - e[0]).norm() == 0);
		CHECK((a[1] - e[1]).norm() == 0);
		std::cout << e << std::endl;
	}
	SECTION("construction from blocking info should work")
	{
		BlockDiagonalMatrixXd e = BlockDiagonalMatrixXd::CreateFromBlockingInfo({3, 1, 0, 2});
		CHECK(e.blockCount() == 4);
		CHECK(e.totalDimension() == 3 + 1 + 0 + 2);
		CHECK(e.dimensionOfBlock(0) == 3);
		CHECK(e.dimensionOfBlock(1) == 1);
		CHECK(e.dimensionOfBlock(2) == 0);
		CHECK(e.dimensionOfBlock(3) == 2);
		std::cout << e << std::endl;
	}
	SECTION("extractFrom should work")
	{
		MatrixXd src = MatrixXd::Random(5, 5);
		auto b = BlockDiagonalMatrixXd::ExtractFrom(src, {2, 1, 2});
		CHECK(b.totalDimension() == 5);
		CHECK(b.totalValidElementCount() == 2 * 2 + 1 * 1 + 2 * 2);
		CHECK(b.dimensionOfBlock(0) == 2);
		CHECK(b.dimensionOfBlock(1) == 1);
		CHECK(b.dimensionOfBlock(2) == 2);

		CHECK((b[0] - src.block(0, 0, 2, 2)).norm() < 1e-12);
		CHECK((b[1] - src.block(2, 2, 1, 1)).norm() < 1e-12);
		CHECK((b[2] - src.block(3, 3, 2, 2)).norm() < 1e-12);
		std::cout << src << "\n\n" << b << std::endl;
	}
	SECTION("isShapeLike and isShapeAs should work")
	{
		CHECK(b.isShapeLike(a));
		CHECK(b.isShapeAs(a));
		CHECK(a.isShapeLike(b));
		CHECK(a.isShapeAs(b));

		const BlockDiagonalMatrixXd x({BlockType::Random(3, 3), BlockType::Random(2, 2)});
		CHECK(!b.isShapeLike(x));
		CHECK(!b.isShapeAs(x));
	}
	SECTION("ShapeLike constrution should work")
	{
		BlockDiagonalMatrixXd e = BlockDiagonalMatrixXd::ShapeLike(a);
		CHECK(e.isShapeLike(a));
		CHECK(e.isShapeAs(a));
	}
	SECTION("setConstant should work")
	{
		auto e = a;
		e.setConstant(3.14);

		REQUIRE(e.isShapeAs(a));
		CHECK((e[0] - Eigen::MatrixXd::Constant(2, 2, 3.14)).norm() == 0);
		CHECK((e[1] - Eigen::MatrixXd::Constant(3, 3, 3.14)).norm() == 0);
	}
	SECTION("BlockDiagonalMatrixXd::Constant should work")
	{
		auto e = BlockDiagonalMatrixXd::Constant({2, 3}, 3.14);

		CHECK((e[0] - Eigen::MatrixXd::Constant(2, 2, 3.14)).norm() == 0);
		CHECK((e[1] - Eigen::MatrixXd::Constant(3, 3, 3.14)).norm() == 0);
	}
	SECTION("BlockDiagonalMatrixXd::Zero should work")
	{
		auto e = BlockDiagonalMatrixXd::Zero({2, 3});

		CHECK((e[0] - Eigen::MatrixXd::Zero(2, 2)).norm() == 0);
		CHECK((e[1] - Eigen::MatrixXd::Zero(3, 3)).norm() == 0);
	}
	SECTION("BlockDiagonalMatrixXd::Ones should work")
	{
		auto e = BlockDiagonalMatrixXd::Ones({2, 3});

		CHECK((e[0] - Eigen::MatrixXd::Ones(2, 2)).norm() == 0);
		CHECK((e[1] - Eigen::MatrixXd::Ones(3, 3)).norm() == 0);
	}
	SECTION("BlockDiagonalMatrixXd::Random should work")
	{
		auto e1 = BlockDiagonalMatrixXd::Random({2, 3});
		auto e2 = BlockDiagonalMatrixXd::Random({2, 3});

		REQUIRE(e1.isShapeAs(e2));
		CHECK((e1[0] - e2[0]).norm() > 0);
		CHECK((e1[1] - e2[1]).norm() > 0);
	}
	SECTION("total-dimension method should work")
	{
		CHECK(a.totalDimension() == 5);
		CHECK(a.rows() == 5);
		CHECK(a.cols() == 5);

		std::cout << "(no output)" << std::endl;
	}
	SECTION("toDense() method should work")
	{
		MatrixXd ref = MatrixXd::Zero(5, 5);
		ref.block(0, 0, 2, 2) = blocksA[0];
		ref.block(2, 2, 3, 3) = blocksA[1];

		const auto dense = a.toDense();
		CHECK(dense.rows() == 5);
		CHECK(dense.cols() == 5);
		CHECK((ref - dense).norm() < 1e-15);

		std::ostringstream ss1;
		std::ostringstream ss2;
		ss1 << ref;
		ss2 << dense;
		CHECK(ss1.str() == ss2.str());
		std::cout << ss1.str() << std::endl;
	}
	SECTION("unary minus should work")
	{
		BlockDiagonalMatrixXd e = -a;
		CHECK((-a).blockCount() == a.blockCount());
		CHECK(e.blockCount() == a.blockCount());
		CHECK((a[0] + e[0]).norm() == 0);
		CHECK((a[1] + e[1]).norm() == 0);

		std::ostringstream ss1;
		std::ostringstream ss2;
		ss1 << e;
		ss2 << -a;
		CHECK(ss1.str() == ss2.str());
		std::cout << ss1.str() << std::endl;

		MatrixXd ref = MatrixXd::Zero(5, 5);
		ref.block(0, 0, 2, 2) = -blocksA[0];
		ref.block(2, 2, 3, 3) = -blocksA[1];

		const auto dense = (-a).toDense();
		CHECK((dense - ref).norm() < 1e-12);
	}
	SECTION("binary add should work")
	{
		BlockDiagonalMatrixXd e = a + c;
		CHECK(e.blockCount() == a.blockCount());
		CHECK((a[0] + c[0] - e[0]).norm() < 1e-12);
		CHECK((a[1] + c[1] - e[1]).norm() < 1e-12);

		std::ostringstream ss1;
		std::ostringstream ss2;
		ss1 << e;
		ss2 << a + c;
		CHECK(ss1.str() == ss2.str());
		std::cout << ss1.str() << std::endl;
	}
	SECTION("binary minus should work")
	{
		BlockDiagonalMatrixXd e = a - c;
		CHECK(e.blockCount() == a.blockCount());
		CHECK((a[0] - c[0] - e[0]).norm() < 1e-12);
		CHECK((a[1] - c[1] - e[1]).norm() < 1e-12);

		std::ostringstream ss1;
		std::ostringstream ss2;
		ss1 << e;
		ss2 << a - c;
		CHECK(ss1.str() == ss2.str());
		std::cout << ss1.str() << std::endl;
	}
	SECTION("binary multiplication should work")
	{
		BlockDiagonalMatrixXd e = a * c;
		CHECK(e.blockCount() == a.blockCount());
		CHECK((a[0] * c[0] - e[0]).norm() < 1e-12);
		CHECK((a[1] * c[1] - e[1]).norm() < 1e-12);

		std::ostringstream ss1;
		std::ostringstream ss2;
		ss1 << e;
		ss2 << a * c;
		CHECK(ss1.str() == ss2.str());
		std::cout << ss1.str() << std::endl;
	}
	SECTION("scalar multiplication should work", "a * 3.14")
	{
		BlockDiagonalMatrixXd e = a * 3.14;
		CHECK(e.blockCount() == a.blockCount());
		CHECK((a[0] * 3.14 - e[0]).norm() < 1e-12);
		CHECK((a[1] * 3.14 - e[1]).norm() < 1e-12);

		std::ostringstream ss1;
		std::ostringstream ss2;
		ss1 << e;
		ss2 << a * 3.14;
		CHECK(ss1.str() == ss2.str());
		std::cout << ss1.str() << std::endl;
	}
	SECTION("scalar division should work", "a * 3.14")
	{
		BlockDiagonalMatrixXd e = a / 3.14;
		CHECK(e.blockCount() == a.blockCount());
		CHECK((a[0] / 3.14 - e[0]).norm() < 1e-12);
		CHECK((a[1] / 3.14 - e[1]).norm() < 1e-12);

		std::ostringstream ss1;
		std::ostringstream ss2;
		ss1 << e;
		ss2 << a / 3.14;
		CHECK(ss1.str() == ss2.str());
		std::cout << ss1.str() << std::endl;
	}
	SECTION("scalar multiplication should work", "3.14 * a")
	{
		BlockDiagonalMatrixXd e = 3.14 * a;
		CHECK(e.blockCount() == a.blockCount());
		CHECK((a[0] * 3.14 - e[0]).norm() < 1e-12);
		CHECK((a[1] * 3.14 - e[1]).norm() < 1e-12);

		std::ostringstream ss1;
		std::ostringstream ss2;
		ss1 << e;
		ss2 << 3.14 * a;
		CHECK(ss1.str() == ss2.str());
		std::cout << ss1.str() << std::endl;
	}
	SECTION("compound assignment operators should work", "*= scalar")
	{
		BlockDiagonalMatrixXd e = a;
		e *= -3.14;
		CHECK(e.blockCount() == a.blockCount());
		CHECK((a[0] * 3.14 + e[0]).norm() < 1e-12);
		CHECK((a[1] * 3.14 + e[1]).norm() < 1e-12);
		std::cout << e << std::endl;
	}
	SECTION("compound assignment operators should work", "/= scalar")
	{
		BlockDiagonalMatrixXd e = a;
		e /= -3.14;
		CHECK(e.blockCount() == a.blockCount());
		CHECK((a[0] / 3.14 + e[0]).norm() < 1e-12);
		CHECK((a[1] / 3.14 + e[1]).norm() < 1e-12);
		std::cout << e << std::endl;
	}
	SECTION("compound assignment operators should work", "+=")
	{
		BlockDiagonalMatrixXd e = a;
		e += b;
		CHECK(e.blockCount() == a.blockCount());
		CHECK((a[0] + b[0] - e[0]).norm() < 1e-12);
		CHECK((a[1] + b[1] - e[1]).norm() < 1e-12);
		std::cout << e << std::endl;
	}
	SECTION("compound assignment operators should work", "-=")
	{
		BlockDiagonalMatrixXd e = a;
		e -= b;
		CHECK(e.blockCount() == a.blockCount());
		CHECK((a[0] - b[0] - e[0]).norm() < 1e-12);
		CHECK((a[1] - b[1] - e[1]).norm() < 1e-12);
		std::cout << e << std::endl;
	}
	SECTION("compound assignment operators should work", "*=")
	{
		BlockDiagonalMatrixXd e = a;
		e *= b;
		CHECK(e.blockCount() == a.blockCount());
		CHECK((a[0] * b[0] - e[0]).norm() < 1e-12);
		CHECK((a[1] * b[1] - e[1]).norm() < 1e-12);
		std::cout << e << std::endl;
	}
	SECTION("compound expression should work", "a * c + d")
	{
		BlockDiagonalMatrixXd e = a * c + d;
		CHECK(e.blockCount() == a.blockCount());
		CHECK((a[0] * c[0] + d[0] - e[0]).norm() < 1e-12);
		CHECK((a[1] * c[1] + d[1] - e[1]).norm() < 1e-12);


		std::ostringstream ss1;
		std::ostringstream ss2;
		ss1 << e;
		ss2 << a * c + d;
		CHECK(ss1.str() == ss2.str());
		std::cout << ss1.str() << std::endl;
	}
	SECTION("compound expression should work", "a * c - d")
	{
		BlockDiagonalMatrixXd e = a * c - d;
		CHECK(e.blockCount() == a.blockCount());
		CHECK((a[0] * c[0] - d[0] - e[0]).norm() < 1e-12);
		CHECK((a[1] * c[1] - d[1] - e[1]).norm() < 1e-12);

		std::ostringstream ss1;
		std::ostringstream ss2;
		ss1 << e;
		ss2 << a * c - d;
		CHECK(ss1.str() == ss2.str());
		std::cout << ss1.str() << std::endl;
	}
	SECTION("compound expression should work", "a * c * d")
	{
		BlockDiagonalMatrixXd e = a * c * d;
		CHECK(e.blockCount() == a.blockCount());
		CHECK((a[0] * c[0] * d[0] - e[0]).norm() < 1e-12);
		CHECK((a[1] * c[1] * d[1] - e[1]).norm() < 1e-12);

		std::ostringstream ss1;
		std::ostringstream ss2;
		ss1 << e;
		ss2 << a * c * d;
		CHECK(ss1.str() == ss2.str());
		std::cout << ss1.str() << std::endl;
	}
	SECTION("compound expression should work", "-a * c + d")
	{
		BlockDiagonalMatrixXd e = -a * c + d;
		CHECK(e.blockCount() == a.blockCount());
		CHECK((-a[0] * c[0] + d[0] - e[0]).norm() < 1e-12);
		CHECK((-a[1] * c[1] + d[1] - e[1]).norm() < 1e-12);

		std::ostringstream ss1;
		std::ostringstream ss2;
		ss1 << e;
		ss2 << -a * c + d;
		CHECK(ss1.str() == ss2.str());
		std::cout << ss1.str() << std::endl;
	}
	SECTION("eval should returns correct type")
	{
		STATIC_CHECK(std::is_same<decltype((-a * c + d).eval()), BlockDiagonalMatrixXd>::value);
		STATIC_CHECK(std::is_same<decltype((-a * c).eval()), BlockDiagonalMatrixXd>::value);
		STATIC_CHECK(std::is_same<decltype((a * c).eval()), BlockDiagonalMatrixXd>::value);
		STATIC_CHECK(std::is_same<decltype((a * 6.9).eval()), BlockDiagonalMatrixXd>::value);
		STATIC_CHECK(std::is_same<decltype((4.2 * a * 6.9).eval()), BlockDiagonalMatrixXd>::value);
		STATIC_CHECK(std::is_same<decltype((4.2 * a).eval()), BlockDiagonalMatrixXd>::value);
		STATIC_CHECK(std::is_same<decltype((a / 800.85).eval()), BlockDiagonalMatrixXd>::value);
		STATIC_CHECK(std::is_same<decltype((-a + c).eval()), BlockDiagonalMatrixXd>::value);
		STATIC_CHECK(std::is_same<decltype((-a).eval()), BlockDiagonalMatrixXd>::value);
		std::cout << "(no output)" << std::endl;
	}
	SECTION("block-diagonal times dense matrix should work")
	{
		MatrixXd dense = MatrixXd::Random(5, 5);
		MatrixXd ref = dense * a.toDense();

		MatrixXd actual = dense * a;

		CHECK((ref - actual).norm() < 1e-12);
		std::cout << actual << std::endl;
	}
	SECTION("block-diagonal times dense matrix should work with negate")
	{
		MatrixXd dense = MatrixXd::Random(5, 5);
		MatrixXd ref = -dense * a.toDense();

		MatrixXd actual = -dense * a;

		CHECK((ref - actual).norm() < 1e-12);
		std::cout << actual << std::endl;
	}
	SECTION("block-diagonal times dense matrix should work with negate too")
	{
		MatrixXd dense = MatrixXd::Random(5, 5);
		MatrixXd ref = dense * (-a).toDense();

		MatrixXd actual = dense * (-a);

		CHECK((ref - actual).norm() < 1e-12);
		std::cout << actual << std::endl;
	}
	SECTION("block-diagonal times dense matrix should work with negate three")
	{
		MatrixXd dense = MatrixXd::Random(5, 5);
		MatrixXd ref = -dense * (-a).toDense();

		MatrixXd actual = -dense * (-a);

		CHECK((ref - actual).norm() < 1e-12);
		std::cout << actual << std::endl;
	}
	SECTION("block-diagonal time dense matrix should work")
	{
		MatrixXd dense = MatrixXd::Random(5, 5);
		MatrixXd ref = a.toDense() * dense;

		MatrixXd actual = a * dense;

		CHECK((ref - actual).norm() < 1e-12);
		std::cout << actual << std::endl;
	}
	SECTION("block-diagonal time dense matrix should work with negate")
	{
		MatrixXd dense = MatrixXd::Random(5, 5);
		MatrixXd ref = (-a).toDense() * dense;

		MatrixXd actual = (-a) * dense;

		CHECK((ref - actual).norm() < 1e-12);
		std::cout << actual << std::endl;
	}
	SECTION("block-diagonal time dense matrix should work with negate")
	{
		MatrixXd dense = MatrixXd::Random(5, 5);
		MatrixXd ref = a.toDense() * (-dense);

		MatrixXd actual = a * (-dense);

		CHECK((ref - actual).norm() < 1e-12);
		std::cout << actual << std::endl;
	}
	SECTION("block-diagonal time dense matrix should work with negate three")
	{
		MatrixXd dense = MatrixXd::Random(5, 5);
		MatrixXd ref = (-a).toDense() * (-dense);

		MatrixXd actual = (-a) * (-dense);

		CHECK((ref - actual).norm() < 1e-12);
		std::cout << actual << std::endl;
	}
	SECTION("block-diagonal matrix transformation should work")
	{
		MatrixXd dense = Eigen::MatrixXd(5, 5);
		MatrixXd ref = dense * a.toDense() * dense.adjoint();
		MatrixXd actual = dense * a * dense.adjoint();

		CHECK((ref - actual).norm() < 1e-12);
		std::cout << actual << std::endl;
	}
	SECTION("block-diagonal matrix transformation should work, (b1 + b2) * m")
	{
		MatrixXd dense = Eigen::MatrixXd(5, 5);
		MatrixXd ref = (a.toDense() + b.toDense()) * dense.adjoint();
		MatrixXd actual = (a + b) * dense.adjoint();

		CHECK((ref - actual).norm() < 1e-12);
		std::cout << actual << std::endl;
	}
	SECTION("block-diagonal matrix transformation should work, (b1 - b2) * m")
	{
		MatrixXd dense = Eigen::MatrixXd(5, 5);
		MatrixXd ref = (a.toDense() - b.toDense()) * dense.adjoint();
		MatrixXd actual = (a - b) * dense.adjoint();

		CHECK((ref - actual).norm() < 1e-12);
		std::cout << actual << std::endl;
	}
	SECTION("block-diagonal matrix transformation should work, (b1 * scalar) * m")
	{
		MatrixXd dense = Eigen::MatrixXd(5, 5);
		MatrixXd ref = (a.toDense() * 3.14) * dense.adjoint();
		MatrixXd actual = (a * 3.14) * dense.adjoint();

		CHECK((ref - actual).norm() < 1e-12);
		std::cout << actual << std::endl;
	}
	SECTION("block-diagonal matrix transformation should work, (scalar * b1) * m")
	{
		MatrixXd dense = Eigen::MatrixXd(5, 5);
		MatrixXd ref = (3.14 * a.toDense()) * dense.adjoint();
		MatrixXd actual = (3.14 * a) * dense.adjoint();

		CHECK((ref - actual).norm() < 1e-12);
		std::cout << actual << std::endl;
	}
	SECTION("block-diagonal matrix transformation should work, (b1 / scalar) * m")
	{
		MatrixXd dense = Eigen::MatrixXd(5, 5);
		MatrixXd ref = (a.toDense() / 6.9) * dense.adjoint();
		MatrixXd actual = (a / 6.9) * dense.adjoint();

		CHECK((ref - actual).norm() < 1e-12);
		std::cout << actual << std::endl;
	}
	SECTION("block-diagonal matrix transformation should work, (b1 * b2) * m")
	{
		MatrixXd dense = Eigen::MatrixXd(5, 5);
		MatrixXd ref = (a.toDense() * b.toDense()) * dense.adjoint();
		MatrixXd actual = (a * b) * dense.adjoint();

		CHECK((ref - actual).norm() < 1e-12);
		std::cout << actual << std::endl;
	}
	SECTION("block-diagonal matrix transformation should work, (complex)")
	{
		MatrixXd dense = Eigen::MatrixXd(5, 5);
		MatrixXd ref = dense * (a.toDense() * b.toDense() * 3.14 - c.toDense() / 6.89 + d.toDense()) * dense.adjoint();
		MatrixXd actual = dense * (a * b * 3.14 - c / 6.89 + d) * dense.adjoint();

		CHECK((ref - actual).norm() < 1e-12);
		std::cout << actual << std::endl;
	}
	SECTION("assign block-diagonal to dense should work")
	{
		MatrixXd ref = d.toDense();
		MatrixXd actual = d;

		CHECK((actual - ref).norm() < 1e-12);
	}
	SECTION("assign nagtive block-diagonal to dense should work")
	{
		MatrixXd ref = -d.toDense();
		MatrixXd actual = -d;

		CHECK((actual - ref).norm() < 1e-12);
	}
	SECTION("assign `dense + block-diagonal` to dense should work")
	{
		MatrixXd m0 = Eigen::MatrixXd::Random(5, 5);

		MatrixXd ref = m0 + d.toDense();
		MatrixXd actual = m0 + d;

		CHECK((actual - ref).norm() < 1e-12);
		std::cout << ref << "\n\n" << actual << std::endl;
	}
	SECTION("assign `block-diagonal + dense` to dense should work")
	{
		MatrixXd m0 = Eigen::MatrixXd::Random(5, 5);

		MatrixXd ref = d.toDense() + m0;
		MatrixXd actual = d + m0;

		CHECK((actual - ref).norm() < 1e-12);
		std::cout << ref << "\n\n" << actual << std::endl;
	}
	SECTION("compound assign block-diagonal to dense should work")
	{
		MatrixXd m0 = Eigen::MatrixXd::Random(5, 5);

		{
			MatrixXd ref = m0;
			ref += d.toDense();
			MatrixXd actual = m0;
			actual += d;

			CHECK((actual - ref).norm() < 1e-12);
			std::cout << ref << "\n\n" << actual << std::endl;
		}
		{
			MatrixXd ref = m0;
			ref -= d.toDense();
			MatrixXd actual = m0;
			actual -= d;

			CHECK((actual - ref).norm() < 1e-12);
			std::cout << ref << "\n\n" << actual << std::endl;
		}
	}
	SECTION("compound assign `block-diagonal + dense` to dense should work")
	{
		MatrixXd m0 = Eigen::MatrixXd::Random(5, 5);
		MatrixXd m1 = Eigen::MatrixXd::Random(5, 5);

		MatrixXd ref = m0;
		ref += d.toDense() + m1;
		MatrixXd actual = m0;
		actual += d + m1;

		CHECK((actual - ref).norm() < 1e-12);
		std::cout << ref << "\n\n" << actual << std::endl;
	}
	SECTION("compound assign `dense + block-diagonal` to dense should work")
	{
		MatrixXd m0 = Eigen::MatrixXd::Random(5, 5);
		MatrixXd m1 = Eigen::MatrixXd::Random(5, 5);

		MatrixXd ref = m0;
		ref += m1 + d.toDense();
		MatrixXd actual = m0;
		actual += m1 + d;

		CHECK((actual - ref).norm() < 1e-12);
		std::cout << ref << "\n\n" << actual << std::endl;
	}
	SECTION("assign `dense - block-diagonal` to dense should work")
	{
		MatrixXd m0 = Eigen::MatrixXd::Random(5, 5);

		MatrixXd ref = m0 - d.toDense();
		MatrixXd actual = m0 - d;

		CHECK((actual - ref).norm() < 1e-12);
		std::cout << ref << "\n\n" << actual << std::endl;
	}
	SECTION("assign `block-diagonal - dense` to dense should work")
	{
		MatrixXd m0 = Eigen::MatrixXd::Random(5, 5);

		MatrixXd ref = d.toDense() - m0;
		MatrixXd actual = d - m0;

		CHECK((actual - ref).norm() < 1e-12);
		std::cout << ref << "\n\n" << actual << std::endl;
	}
	SECTION("compound assign `block-diagonal - dense` to dense should work")
	{
		MatrixXd m0 = Eigen::MatrixXd::Random(5, 5);
		MatrixXd m1 = Eigen::MatrixXd::Random(5, 5);

		MatrixXd ref = m0;
		ref += d.toDense() - m1;
		MatrixXd actual = m0;
		actual += d - m1;

		CHECK((actual - ref).norm() < 1e-12);
		std::cout << ref << "\n\n" << actual << std::endl;
	}
	SECTION("compound assign `dense - block-diagonal` to dense should work")
	{
		MatrixXd m0 = Eigen::MatrixXd::Random(5, 5);
		MatrixXd m1 = Eigen::MatrixXd::Random(5, 5);

		MatrixXd ref = m0;
		ref += m1 - d.toDense();
		MatrixXd actual = m0;
		actual += m1 - d;

		CHECK((actual - ref).norm() < 1e-12);
		std::cout << ref << "\n\n" << actual << std::endl;
	}
	SECTION("compound assign block-diagonal to dense should work")
	{
		MatrixXd m = MatrixXd::Random(5, 5);
		MatrixXd n = m;

		SECTION("operator +=")
		{
			m += d.toDense();
			n += d;
			CHECK((m - n).norm() < 1e-12);
			std::cout << n << std::endl;
		}

		SECTION("operator -=")
		{
			m += d.toDense();
			n += d;
			CHECK((m - n).norm() < 1e-12);
			std::cout << n << std::endl;
		}

		SECTION("operator *=")
		{
			m *= d.toDense();
			n *= d;
			CHECK((m - n).norm() < 1e-12);
			std::cout << n << std::endl;
		}
	}
	SECTION("resize of sub-block shall not be allowed")
	{
		auto e = a;
		const MatrixXd m2 = MatrixXd::Random(2, 2);

		CHECK_NOTHROW(e[0] = m2);
		CHECK((e[0] - m2).norm() < 1e-12);

		try
		{
			CHECK_THROWS(e[0] = MatrixXd::Random(5, 5));
		}
		catch (const AssertionException& exception)
		{
			const std::string message = exception.what();
			CHECK(message.find("does not actually allow to resize") != std::string::npos);
		}
		std::cout << "(no output)" << std::endl;
	}
	SECTION("const iterator (over each sub-block) should work, with .begin() and .end()")
	{
		auto iterator0 = blocksA.begin();
		auto iterator1 = a.begin();

		while (iterator0 != blocksA.end())
		{
			CHECK((*iterator0 - *iterator1).norm() == 0);
			iterator0++;
			iterator1++;
		}

		CHECK(iterator1 == a.end());
		std::cout << "(no output)" << std::endl;
	}
	SECTION("const iterator (over each sub-block) should work, with .cbegin() and .cend()")
	{
		auto iterator0 = blocksA.cbegin();
		auto iterator1 = a.cbegin();

		while (iterator0 != blocksA.cend())
		{
			CHECK((*iterator0 - *iterator1).norm() == 0);
			iterator0++;
			iterator1++;
		}

		CHECK(iterator1 == a.cend());
		std::cout << "(no output)" << std::endl;
	}
	SECTION("non-const iterator (over each sub-block) should work, with .begin() and .end()")
	{
		auto mutableBlocksA = blocksA;
		auto mutableA = a;

		auto iterator0 = mutableBlocksA.begin();
		auto iterator1 = mutableA.begin();
		auto iterator2 = b.begin();

		while (iterator0 != mutableBlocksA.end())
		{
			CHECK((*iterator0 - *iterator1).norm() == 0);
			*iterator1 = *iterator2;
			CHECK((*iterator2 - *iterator1).norm() == 0);
			iterator0++;
			iterator1++;
			iterator2++;
		}

		CHECK(iterator1 == mutableA.end());
		std::cout << "(no output)" << std::endl;
	}
	// SECTION("File read wirte should work (StoreBufferDataWithSizeInfo)")
	// {
	// 	auto file = SecUtility::IO::File::Random().CreateIfNotExist();
	//
	// 	file.Write(a[0], 4269, SecUtility::IO::DynamicBufferStorageMode::StoreBufferDataWithSizeInfo);
	// 	MatrixXd aZeroToo;
	// 	file.Read(aZeroToo, 4269, SecUtility::IO::DynamicBufferStorageMode::StoreBufferDataWithSizeInfo);
	// 	REQUIRE((a[0] - aZeroToo).norm() == 0);
	//
	// 	file.Write(a, 4269, SecUtility::IO::DynamicBufferStorageMode::StoreBufferDataWithSizeInfo);
	// 	BlockDiagonalMatrixXd aToo{};
	// 	file.Read(aToo, 4269, SecUtility::IO::DynamicBufferStorageMode::StoreBufferDataWithSizeInfo);
	// 	CHECK(aToo.blockCount() == a.blockCount());
	// 	CHECK((a[0] - aToo[0]).norm() == 0);
	// 	CHECK((a[1] - aToo[1]).norm() == 0);
	//
	// 	std::ostringstream ss0;
	// 	std::ostringstream ss1;
	// 	ss0 << a;
	// 	ss1 << aToo;
	// 	CHECK(ss0.str() == ss1.str());
	// 	std::cout << ss1.str() << std::endl;
	//
	// 	file.Delete();
	// }
	// SECTION("File read wirte should work (StoreBufferDataOnly)")
	// {
	// 	auto file = SecUtility::IO::File::Random().CreateIfNotExist();
	//
	// 	file.Write(a, 4269, SecUtility::IO::DynamicBufferStorageMode::StoreBufferDataOnly);
	// 	BlockDiagonalMatrixXd aToo = BlockDiagonalMatrixXd::CreateFromBlockingInfo({2, 3});
	// 	REQUIRE(aToo.blockCount() == a.blockCount());
	// 	REQUIRE(aToo.dimensionOfBlock(0) == a.dimensionOfBlock(0));
	// 	REQUIRE(aToo.dimensionOfBlock(1) == a.dimensionOfBlock(1));
	// 	file.Read(aToo, 4269, SecUtility::IO::DynamicBufferStorageMode::StoreBufferDataOnly);
	// 	CHECK((a[0] - aToo[0]).norm() == 0);
	// 	CHECK((a[1] - aToo[1]).norm() == 0);
	//
	// 	std::ostringstream ss0;
	// 	std::ostringstream ss1;
	// 	ss0 << a;
	// 	ss1 << aToo;
	// 	CHECK(ss0.str() == ss1.str());
	// 	std::cout << ss1.str() << std::endl;
	//
	// 	file.Delete();
	// }
}
