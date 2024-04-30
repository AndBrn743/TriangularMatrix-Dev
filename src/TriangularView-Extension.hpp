//
// Created by Andy on 4/29/2024.
// This is an extension to the Eigen library, which can be obtained from
// https://eigen.tuxfamily.org/index.php?title=Main_Page
//
// Copyright (C) 2024 Andy Brown <AndBrwn1933@outlook.com>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//

#pragma once

namespace Eigen
{
	template <typename _MatrixType, unsigned int _Mode>
	class TriangularViewImpl<_MatrixType, _Mode, Hoppy::TriangularCompressed>
	    : public TriangularViewImpl<_MatrixType, _Mode, Dense>
	{
	protected:
		EIGEN_DEFAULT_COPY_CONSTRUCTOR(TriangularViewImpl)
		EIGEN_DEFAULT_EMPTY_CONSTRUCTOR_AND_DESTRUCTOR(TriangularViewImpl)
	};
}  // namespace Eigen
