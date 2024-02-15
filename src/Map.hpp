//
// Created by Andy on 2/15/2024.
//

#pragma once

namespace Hoppy
{
	template <typename Derived>
	class Map
	    : public std::conditional<
	              std::is_same<typename Eigen::internal::traits<Derived>::StorageKind, TriangularCompressed>::value,
	              MapBase4TriangularCompressed<Derived>,
	              Eigen::Map<Derived>>::type
	{
	public:
		using Base = typename std::conditional<
		        std::is_same<typename Eigen::internal::traits<Derived>::StorageKind, TriangularCompressed>::value,
		        MapBase4TriangularCompressed<Derived>,
		        Eigen::MapBase<Derived>>::type;

		template <typename... Args>
		explicit Map(Args&&... args) : Base(std::forward<Args>(args)...)
		{
			/* NO CODE */
		}

		// TODO: Add `StrideType m_stride` as protected member
	};
}  // namespace Hoppy
