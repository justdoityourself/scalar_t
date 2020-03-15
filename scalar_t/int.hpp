/* Copyright (C) 2020 D8DATAWORKS - All Rights Reserved */

#pragma once

#include <initializer_list>
#include <string>
#include <vector>
#include <array>

#include <cstdint>
#include <type_traits>

#include <intrin.h>
#pragma intrinsic(_umul128)


namespace scalar_t
{
	/*
		Finite Multiplication

		    6 7 4
		 X  1 1 1

		6 6 6
		  7 7 7
		    4 4 4

			6
			7 7
			4 4 4
	*/

	//Little Endian
	template <typename S, typename L> auto mut(const S& s1, const S& s2)
	{
		union
		{
			S s[2];
			L l;
		};

		l = (L)s1 * (L)s2;

		return std::make_pair(s[1], s[0]);
	}

	template < typename T > auto mul(const T& t1, const T& t2)
	{
		if constexpr (std::is_same<T, uint8_t>())
			return mut<T, uint16_t>(t1, t2);
		if constexpr (std::is_same<T, uint16_t>())
			return mut<T, uint32_t>(t1, t2);
		if constexpr (std::is_same<T, uint32_t>())
			return mut<T, uint64_t>(t1, t2);
		if constexpr (std::is_same<T, uint64_t>())
		{
			uint64_t lw, hh;
			lw = _umul128(t1, t2, &hh);

			return std::make_pair(hh, lw);
		}
	}

	template < typename T > bool add(T& t1, const T& t2)
	{
		t1 += t2;
		return t2 > t1;
	}

	template < typename C, typename T > void vad(C & c, size_t i, const T & v)
	{
		bool carry = add(c[i],v);

		while (carry && --i != -1)
			carry = add(c[i], T(1));
	}

	template < typename T > bool sub(T& t1, const T& t2)
	{
		bool carry = t2 > t1;
		t1 -= t2;
		return carry;
	}

	template < typename C, typename T > void vsb(C& c, size_t i, const T& v)
	{
		bool carry = sub(c[i], v);

		while (carry && --i != -1)
			carry = sub(c[i], T(1));
	}

	template <typename C1, typename C2, typename R> void finite_vector_subtract(const C1& v1, const C2& v2, R& result)
	{
		std::copy(v1.begin(), v1.end(), result.begin());

		for (size_t i = 0; i < v1.size(); i++)
			vsb(result, i, v2[i]);
	}

	template <typename C1, typename C2> void finite_vector_subtract(C1& v1, const C2& v2)
	{
		for (size_t i = 0; i < v1.size(); i++)
			vsb(v1, i, v2[i]);
	}

	template <typename C1, typename C2, typename R> void finite_vector_multiply(const C1& v1, const C2& v2, R& result)
	{
		for (size_t j = 0, k = v1.size() - 1; j < v1.size(); j++, k--)
		{
			result[j] = 0;
			result[0] += v1[j] * v2[k];
		}
		
		for (size_t i = 1; i < v1.size(); i++)
		{
			for (size_t j = i, k = v1.size() - 1; j < v1.size(); j++, k--)
			{
				auto [h,l] = mul(v1[j], v2[k]);

				vad(result, i, l);
				vad(result, i - 1, h);
			}
		}
	}

	template <typename C1, typename C2, typename R> void finite_vector_add(const C1& v1, const C2& v2, R & result)
	{
		std::copy(v1.begin(), v1.end(), result.begin());

		for (size_t i = 0; i < v1.size(); i++)
			vad(result, i, v2[i]);
	}

	template <typename C1, typename C2> void finite_vector_add(C1& v1, const C2& v2)
	{
		for (size_t i = 0; i < v1.size(); i++)
			vad(v1, i, v2[i]);
	}

	template<typename T, size_t S> class uintv_t : public std::array<T,S>
	{
		using B = std::array<T, S>;
		using U = uintv_t<T, S>;
	public:

		uintv_t() : B{} {}

		uintv_t(T t)
		{
			for (auto& e : *this)
				e = 0;

			B::back() = t;
		}

		template <typename... TL> uintv_t(TL... ts) : B{ static_cast<T>(ts)... } {}
		
		U operator + (const U& r) const
		{
			U result;

			finite_vector_add(*this, r, result);

			return result;
		}

		U operator - (const U& r) const
		{
			U result;

			finite_vector_subtract(*this, r, result);

			return result;
		}

		U operator * (const U& r) const
		{
			U result;

			finite_vector_multiply(*this, r, result);

			return result;
		}

		U& operator++()
		{
			vad(*this, B::size() - 1, 1);

			return *this;
		}

		U& operator += (const U& r)
		{
			finite_vector_add(*this, r);

			return *this;
		}

		U& operator -= (const U& r)
		{
			finite_vector_subtract(*this, r);

			return *this;
		}

		U& operator *= (const U& r)
		{
			U result;

			finite_vector_multiply(*this, r, result);

			return *this = result;
		}

		U& operator = (const U& r)
		{
			std::copy(r.begin(), r.end(), B::begin());

			return *this;
		}

		T operator% (T m)
		{
			return B::back() % m;
		}

		bool operator == (const T& t) const
		{
			for (size_t i = 0; i < B::size() - 1; i++)
				if ((*this)[i])
					return false;

			return B::back() == t;

			return true;
		}

		bool operator == (const U& r) const
		{
			return std::equal(B::begin(), B::end(), r.begin());
		}
	};
}