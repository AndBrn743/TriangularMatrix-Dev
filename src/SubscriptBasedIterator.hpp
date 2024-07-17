//
// Created by Andy on 6/23/2024.
//

#pragma once

#include <cstddef>
#include <utility>


namespace SecUtility
{
	template <typename Iteratable, typename Subscript = std::size_t, typename SubscriptOffset = std::ptrdiff_t>
	class SubscriptBasedIterator
	{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = SubscriptOffset;
		using reference = decltype(std::declval<Iteratable>()[std::declval<Subscript>()]);
		using value_type = typename std::remove_reference<typename std::remove_cv<reference>::type>::type;
		using pointer = value_type*;


		constexpr SubscriptBasedIterator(Iteratable& collection, const Subscript index) noexcept
		    : r_Collection(collection), m_Index(index)
		{
			/* NO CODE */
		}

		constexpr decltype(auto) operator*() const
		{
			return const_cast<const Iteratable&>(r_Collection)[m_Index];
		}

		constexpr decltype(auto) operator*()
		{
			return r_Collection[m_Index];
		}

		constexpr auto operator->() const
		{
			return &const_cast<const Iteratable&>(r_Collection)[m_Index];
		}

		constexpr auto operator->()
		{
			return &r_Collection[m_Index];
		}

		constexpr SubscriptBasedIterator operator+=(const SubscriptOffset offset) noexcept
		{
			m_Index += offset;
			return *this;
		}

		constexpr SubscriptBasedIterator operator-=(const SubscriptOffset offset) noexcept
		{
			m_Index -= offset;
			return *this;
		}

		friend constexpr SubscriptBasedIterator operator+(SubscriptBasedIterator iterator,
		                                                  const SubscriptOffset offset) noexcept
		{
			iterator += offset;
			return iterator;
		}

		friend constexpr SubscriptBasedIterator operator-(SubscriptBasedIterator iterator,
		                                                  const SubscriptOffset offset) noexcept
		{
			iterator -= offset;
			return iterator;
		}

		constexpr SubscriptBasedIterator operator++() noexcept
		{
			m_Index++;
			return *this;
		}

		constexpr SubscriptBasedIterator operator--() noexcept
		{
			m_Index--;
			return *this;
		}

		constexpr SubscriptBasedIterator operator++(int) noexcept
		{
			auto old = *this;
			m_Index++;
			return old;
		}

		constexpr SubscriptBasedIterator operator--(int) noexcept
		{
			auto old = *this;
			m_Index--;
			return old;
		}

		friend constexpr bool operator==(const SubscriptBasedIterator& lhs, const SubscriptBasedIterator& rhs) noexcept
		{
			return &lhs.r_Collection == &rhs.r_Collection && lhs.m_Index == rhs.m_Index;
		}

		friend constexpr bool operator!=(const SubscriptBasedIterator& lhs, const SubscriptBasedIterator& rhs) noexcept
		{
			return !(lhs == rhs);
		}

		friend constexpr SubscriptOffset operator-(const SubscriptBasedIterator& lhs,
		                                           const SubscriptBasedIterator& rhs) noexcept
		{
			return lhs.m_Index - rhs.m_Index;
		}


	private:
		Iteratable& r_Collection;
		Subscript m_Index;
	};
}  // namespace SecUtility
