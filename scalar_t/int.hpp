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


#include "d8u/random.hpp"

#include "helper.hpp"

namespace scalar_t
{
	using namespace helper;

	template<typename T, size_t S> class uintv_t : public std::array<T,S>
	{
		using B = std::array<T, S>;
		using U = uintv_t<T, S>;

	public:

		std::string string()
		{
			std::stringstream s;

			for (auto& e : *this)
				s << std::hex << +e;

			return s.str();
		}

		explicit operator bool() const
		{
			for (auto& e : *this)
				if (e)
					return true;

			return false;
		}

		void Random()
		{
			for (auto& e : *this)
				e = (T)d8u::random::Integer();
		}

		void BinaryInvert()
		{
			for (auto& e : *this)
				e = ~e;
		}

		/*U MultiplicativeInverseStack()
		{ 
			//Get the Q and M base on MAX_VALUE + 1
			U quo(1), rem, den = *this;

			while (!finite_vector_add(den, *this))
				++quo;

			rem = (*this) * quo;
			rem.BinaryInvert();
			++rem;

			auto [gdc, x, y] = _e_gcd(rem, *this);

			return (y - quo * x);
		}*/

		/*U MultiplicativeInverseHeap()
		{
			//Get the Q and M base on MAX_VALUE + 1
			U quo(1), rem, den = *this;

			while (!finite_vector_add(den, *this))
				++quo;

			rem = (*this) * quo;
			rem.BinaryInvert();
			++rem;

			if (rem < *this);
			else
				std::cout << "here" << std::endl;

			d8u::vector::BlockVector < 64, std::pair<U, U> > blk;

			auto [x, y] = _e_gcd_heap(rem, *this,blk);

			return (y - quo * x);
		}*/

		U MultiplicativeInverse()
		{
			//Get the Q and M base on MAX_VALUE + 1
			U quo(1), rem, den = *this;

			while (!finite_vector_add(den, *this))
				++quo;

			rem = (*this) * quo;
			rem.BinaryInvert();
			++rem;

			auto [x, y] = _e_gcd_loop(rem, *this);

			return (y - quo * x);
		}

		uintv_t() : B{} {}

		uintv_t(T t) : B{} 
		{
			B::back() = t;
		}

		template <typename... TL> uintv_t(T t, TL... ts) : B{ t, static_cast<T>(ts)... } {}
		
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

		U& operator = (const T& t)
		{
			for (auto& e : *this)
				e = 0;

			B::back() = t;

			return *this;
		}

		U operator / (const U& r) const
		{
			auto [q,m] = finite_vector_div(*this, r);

			return q;
		}

		U& operator /= (const U& r)
		{
			auto [q, m] = finite_vector_div(*this, r);

			return *this = q;
		}

		U operator % (const U& r) const
		{
			auto [q, m] = finite_vector_div(*this, r);

			return m;
		}

		T operator% (T m) const
		{
			return B::back() % m;
		}

		U operator << (size_t b) const
		{
			U result = *this;
			vls<T>(result, b);

			return result;
		}

		U& operator <<= (size_t b)
		{
			vls<T>(*this, b);

			return *this;
		}

		U operator >> (size_t b) const
		{
			U result = *this;
			vrs<T>(result, b);

			return result;
		}

		U& operator >>= (size_t b)
		{
			vrs<T>(*this, b);

			return *this;
		}

		bool operator == (const U& r) const
		{
			return std::equal(B::begin(), B::end(), r.begin());
		}

		auto Divide(const U& r) const
		{
			return finite_vector_div(*this, r);
		}

		U& DivideSimple(const U& r)
		{
			auto [q,m] = finite_vector_div_simple(*this, r);

			*this = q;

			return *this;
		}

		size_t Bits() const
		{
			size_t i = 0;
			for (; i < B::size(); i++)
				if ((*this)[i])
					break;

			return (B::size() - 1 - i) * bits<T>() + greatest_bit((*this)[i]);
		}

		void SetBit(size_t bit)
		{
			auto q = bit / bits<T>();
			auto m = bit % bits<T>();

			(*this)[B::size() - 1 - q] |= (T(1) << m);
		}
	};
}