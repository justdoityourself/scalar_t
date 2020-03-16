#pragma once

#include <tuple>

#include "d8u/vector.hpp"

namespace scalar_t
{
	namespace helper
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

		//WARNING: Little Endian
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

		template < typename T1, typename T2 > bool add(T1& t1, const T2& t2)
		{
			t1 += t2;
			return t2 > t1;
		}

		template < typename C, typename T > bool vad(C& c, size_t i, const T& v)
		{
			bool carry = add(c[i], v);

			while (carry && --i != -1)
				carry = add(c[i], T(1));

			return carry;
		}

		template < typename T1, typename T2 > bool sub(T1& t1, const T2& t2)
		{
			bool carry = t2 > t1;
			t1 -= t2;
			return carry;
		}

		template < typename C, typename T > bool vsb(C& c, size_t i, const T& v)
		{
			bool carry = sub(c[i], v);

			while (carry && --i != -1)
				carry = sub(c[i], T(1));

			return carry;
		}

		template< typename T > size_t bits()
		{
			if constexpr (std::is_same<T, uint8_t>())
				return 8;
			if constexpr (std::is_same<T, uint16_t>())
				return 16;
			if constexpr (std::is_same<T, uint32_t>())
				return 32;
			if constexpr (std::is_same<T, uint64_t>())
				return 64;
		}

		template < typename T > T lsh(T& t, const T & pcarry, size_t b)
		{
			T carry = t >> (bits<T>() - b);
			t <<= b;
			t |= pcarry;

			return carry;
		}

		template < typename T, typename C > T vls(C& c, size_t _b)
		{
			auto q = _b / bits<T>();
			auto b = _b % bits<T>();

			T carry = 0; //Carry greater than bits<T>() is discarded

			for (size_t i = q; q && i < c.size(); i++)
				c[i - q] = c[i];

			for (size_t i = c.size() - q; i < c.size(); i++)
				c[i] = 0;

			if(b)
				for (size_t i = c.size() - 1; i != -1; i--)
					carry = lsh(c[i], carry, b);

			return carry;
		}

		template < typename T > T* rsh(T& t, T*pnext, size_t b)
		{
			T carry = t << (bits<T>() - b);
			t >>= b;

			if(pnext)
				*pnext |= carry;

			return &t;
		}

		template < typename T, typename C > void vrs(C& c, size_t _b)
		{
			auto q = _b / bits<T>();
			auto b = _b % bits<T>();

			for (size_t i = c.size() - q - 1; q && i != -1; i--)
				c[i + q] = c[i];

			for (size_t i = 0; i < q; i++)
				c[i] = 0;

			T* pnext = nullptr;

			if(b)
				for (size_t i = c.size() - 1; i != -1; i--)
					pnext = rsh(c[i], pnext, b);
		}

		template <typename C1, typename C2, typename R> bool finite_vector_subtract(const C1& v1, const C2& v2, R& result)
		{
			bool overflow = false;
			std::copy(v1.begin(), v1.end(), result.begin());

			for (size_t i = 0; i < v1.size(); i++)
				if (vsb(result, i, v2[i]))
					overflow = true;

			return overflow;
		}

		template <typename C1, typename C2> bool finite_vector_subtract(C1& v1, const C2& v2)
		{
			bool overflow = false;

			for (size_t i = 0; i < v1.size(); i++)
				if (vsb(v1, i, v2[i]))
					overflow = true;

			return overflow;
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
					auto [h, l] = mul(v1[j], v2[k]);

					vad(result, i, l);
					vad(result, i - 1, h);
				}
			}
		}

		template <typename C1, typename C2> void finite_vector_multiply(C1& v1, const C2& v2)
		{
			for (size_t j = 0, k = v1.size() - 1; j < v1.size(); j++, k--)
			{
				if (!j)
					v1[0] = v1[j] * v2[k];
				else
					v1[0] += v1[j] * v2[k];
			}

			for (size_t i = 1; i < v1.size(); i++)
			{
				for (size_t j = i, k = v1.size() - 1; j < v1.size(); j++, k--)
				{
					auto [h, l] = mul(v1[j], v2[k]);

					if (j == i)
						v1[j] = 0;

					vad(v1, i, l);
					vad(v1, i - 1, h);
				}
			}
		}

		template <typename C1, typename C2, typename R> bool finite_vector_add(const C1& v1, const C2& v2, R& result)
		{
			bool overflow = false;

			std::copy(v1.begin(), v1.end(), result.begin());

			for (size_t i = 0; i < v1.size(); i++)
				if (vad(result, i, v2[i]))
					overflow = true;

			return overflow;
		}

		template <typename C1, typename C2> bool finite_vector_add(C1& v1, const C2& v2)
		{
			bool overflow = false;

			for (size_t i = 0; i < v1.size(); i++)
				if (vad(v1, i, v2[i]))
					overflow = true;

			return overflow;
		}

		template <typename C1, typename C2> bool finite_vector_greater(const C1& v1, const C2& v2)
		{
			for (size_t i = 0; i < v1.size(); i++)
			{
				if (v1[i] > v2[i])
					return true;
				else if (v1[i] != v2[i])
					return false;
			}

			return false;
		}

		template <typename C1, typename C2> bool finite_vector_greater_equal(const C1& v1, const C2& v2)
		{
			for (size_t i = 0; i < v1.size(); i++)
			{
				if (v1[i] && v1[i] >= v2[i])
					return true;
				else if (v1[i] != v2[i])
					return false;
			}

			return false;
		}


		template <typename T> size_t greatest_bit(T t)
		{
			size_t r = 0;

			while (t >>= 1)
				r++;

			return r;
		}

		//This was a stop gap algorithm intended as a proof of concept.
			//
		template <typename T> auto finite_vector_div_simple(const T& _num, const T& den)
		{
			T num = _num;
			T quo;

			while (num && finite_vector_greater_equal(num, den))
			{
				++quo;
				num -= den;
			}

			return std::make_pair(quo, num);
		}

		template <typename T> auto finite_vector_div(const T& _num, const T& _den)
		{
			T num = _num, den = _den;
			T quo;

			if (finite_vector_greater_equal(num, den))
			{
				size_t bits = num.Bits() - den.Bits();

				den <<= bits;

				for (; bits != -1; bits--)
				{
					if (finite_vector_greater_equal(num, den))
					{
						num -= den;
						quo.SetBit(bits);
					}

					den >>= 1;
				}
			}

			return std::make_pair(quo, num);
		}

		template<typename T> std::tuple<T, T, T> _e_gcd(const T & a, const T & b)
		{
			if (a == 0)			
				return std::make_tuple(b, 0, 1);		

			T gcd, x, y;

			auto [q,m] = b.Divide(a);

			auto [gcd, x, y] = _e_gcd<T>(m, a);

			return std::make_tuple(gcd, (y - q * x), x);
		}

		//The smaller stack helps but still is not a good choice when dealing with larger scalars.
		//

		template<typename T> std::pair<T, T> & _e_gcd_heap(const T& a, const T& b, d8u::dynamic::BlockVector < 64, std::pair<T, T> > & v)
		{
			if (a == 0)
			{
				auto& r = v.Allocate();
				r = std::make_pair(T(0), T(1));
				return r;
			}	

			auto& i = v.Allocate();
			i = b.Divide(a);

			auto & j = _e_gcd_heap<T>(i.second, a,v);

			auto& r = v.Allocate();
			r = std::make_pair((j.second - i.first * j.first), j.first);

			return r;
		}

		/*template<typename T> std::pair<T, T> _e_gcd_loop(T a, T b)
		{
			if (a == 0)
				return std::make_pair(T(0), T(1));

			T up(1), vp, uc, vc(1);

			while (a)
			{
				auto [q,m] = b.Divide(a);

				b = a;
				a = m;

				T un = (up - q * uc);
				T vn = (vp - q * vc);

				up = uc;
				vp = vc;
				uc = un;
				vc = vn;
			}

			return std::make_pair(up, vp);
		}*/

		template<typename T> std::pair<T, T> _e_gcd_loop(T a, T b)
		{
			T aa[2] = { 1,0 }, bb[2] = { 0,1 },q;

			while (true) 
			{
				std::tie(q,a) = a.Divide(b);

				aa[0] = aa[0] - q * aa[1];  bb[0] = bb[0] - q * bb[1];

				if (a == 0) 
					return std::make_pair(aa[1], bb[1]);

				std::tie(q, b) = b.Divide(a);

				aa[1] = aa[1] - q * aa[0];  bb[1] = bb[1] - q * bb[0];

				if (b == 0)
					return std::make_pair(aa[0], bb[0]);
			};

		}






	/*def extended_gcd(a, b) :
		if a == 0 :
			return GCD_Result(b, 0, 1)

		unPrev = 1
		vnPrev = 0
		unCur = 0
		vnCur = 1

		while b != 0:
			bn = a // b
			newB = a % b

			a = b
			b = newB

			# Update coefficients
			unNew = unPrev - bn * unCur
			vnNew = vnPrev - bn * vnCur

			# Shift coefficients
			unPrev = unCur
			vnPrev = vnCur
			unCur = unNew
			vnCur = vnNew

			return GCD_Result(a, unPrev, vnPrev)*/
	}
}