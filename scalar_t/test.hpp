/* Copyright (C) 2020 D8DATAWORKS - All Rights Reserved */

#pragma once

#include <array>

#include "../catch.hpp"

#include "int.hpp"

using namespace scalar_t;

TEST_CASE("standing in place multiplication", "[scalar_t::helpers]")
{
	constexpr auto S = 4;
	using U = uint8_t;
	using C = std::array<U, S>;

	{
		//Sym1

		C t1 = { 4,4,4,4 }, t2 = { 1,1,1,1 }, expected = { 16,12,8,4 };

		finite_vector_multiply(t1, t2);

		CHECK(std::equal(t1.begin(), t1.end(), expected.begin()));
	}

	{
		//Sym2

		C t1 = { 4,4,4,4 }, t2 = { 1,2,3,4 }, expected = { 40,36,28,16 };

		finite_vector_multiply(t1, t2);

		CHECK(std::equal(t1.begin(), t1.end(), expected.begin()));
	}

	{
		//Sym3

		C t1 = { 4,3,2,1 }, t2 = { 1,2,3,4 }, expected = { 30,20,11,4 };

		finite_vector_multiply(t1, t2);

		CHECK(std::equal(t1.begin(), t1.end(), expected.begin()));
	}

	{
		//Overflow1

		C t1 = { 40,30,20,10 }, t2 = { 1,2,3,4 }, expected = { 44,200,110,40 };

		finite_vector_multiply(t1, t2);

		CHECK(std::equal(t1.begin(), t1.end(), expected.begin()));
	}

	{
		//Overflow2

		C t1 = { 40,30,20,10 }, t2 = { 10,20,30,40 }, expected = { 191,212,77,144 };

		finite_vector_multiply(t1, t2);

		CHECK(std::equal(t1.begin(), t1.end(), expected.begin()));
	}
}

TEST_CASE("basic multiplication", "[scalar_t::helpers]")
{
	constexpr auto S = 4;
	using U = uint8_t;
	using C = std::array<U, S>;

	{ 
		//Sym1

		C t1 = { 4,4,4,4 }, t2 = { 1,1,1,1 }, expected = { 16,12,8,4 }, result;

		finite_vector_multiply(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}

	{
		//Sym2

		C t1 = { 4,4,4,4 }, t2 = { 1,2,3,4 }, expected = { 40,36,28,16 }, result;

		finite_vector_multiply(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}

	{
		//Sym3

		C t1 = { 4,3,2,1 }, t2 = { 1,2,3,4 }, expected = { 30,20,11,4 }, result;

		finite_vector_multiply(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}

	{
		//Overflow1

		C t1 = { 40,30,20,10 }, t2 = { 1,2,3,4 }, expected = { 44,200,110,40 }, result;

		finite_vector_multiply(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}

	{
		//Overflow2

		C t1 = { 40,30,20,10 }, t2 = { 10,20,30,40 }, expected = { 191,212,77,144 }, result;

		finite_vector_multiply(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}
}


TEST_CASE("basic addition", "[scalar_t::helpers]")
{
	constexpr auto S = 4;
	using U = uint8_t;
	using C = std::array<U, S>;

	{
		//Sym1

		C t1 = { 4,4,4,4 }, t2 = { 1,1,1,1 }, expected = { 5,5,5,5 }, result;

		finite_vector_add(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}

	{
		//Sym2

		C t1 = { 4,4,4,4 }, t2 = { 1,2,3,4 }, expected = { 5,6,7,8 }, result;

		finite_vector_add(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}

	{
		//Sym3

		C t1 = { 4,3,2,1 }, t2 = { 1,2,3,4 }, expected = { 5,5,5,5 }, result;

		finite_vector_add(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}

	{
		//Overflow1

		C t1 = { 255,30,20,10 }, t2 = { 2,2,3,4 }, expected = { 1,32,23,14 }, result;

		finite_vector_add(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}

	{
		//Overflow2

		C t1 = { 40,3,2,1 }, t2 = { 10,252,253,255 }, expected = { 51,0,0,0 },result;

		finite_vector_add(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}
}

TEST_CASE("addition overflow", "[scalar_t::uintv_t]")
{
	{
		uint16_t v = 15;
		uintv_t<uint8_t, 2> v2 = { 15, 0 };

		CHECK(std::memcmp(&v, &v2, 2) == 0);
	}

	{
		uint16_t v = 15;
		uintv_t<uint8_t, 2> v2 = { 0, 15 };

		v += 0xfff2;
		v2 += {0xff, 0xf2};

		//CHECK(!std::memcmp(&v, &v2, 2)); //Big endian
	}
}

TEST_CASE("subtraction overflow", "[scalar_t::uintv_t]")
{
	using U = uintv_t<uint8_t, 2>;
	U v = { 26,99 };
	U v2;

	auto add_inv = v2 - v;
	CHECK((v + add_inv == U{ 0, 0 }));
}


TEST_CASE("Left Shift", "[scalar_t::uintv_t]")
{
	using U = uintv_t<uint8_t, 2>;

	{
		U v; v.Random();
		U v2 = v;

		v <<= 1;
		v2 *= 2;

		CHECK(v == v2);
	}

	{
		U v; v.Random();
		U v2 = v;

		v <<= 2;
		v2 *= 4;

		CHECK(v == v2);
	}

	{
		U v; v.Random();
		U v2 = v;

		v <<= 3;
		v2 *= 8;

		CHECK(v == v2);
	}

	{
		U v; v.Random();
		U v2 = v;

		v <<= 4;
		v2 *= 16;

		CHECK(v == v2);
	}

	{
		U v; v.Random();
		U v2 = v;

		v <<= 5;
		v2 *= 32;

		CHECK(v == v2);
	}

	{
		U v; v.Random();
		U v2 = v;

		v <<= 6;
		v2 *= 64;

		CHECK(v == v2);
	}

	{
		U v; v.Random();
		U v2 = v;

		v <<= 7;
		v2 *= 128;

		CHECK(v == v2);
	}

	{
		U v; v.Random();
		U v2 = v;

		v <<= 8;
		v2 *= {1, 0}; //256

		CHECK(v == v2);
	}

	{
		U v; v.Random();
		U v2 = v;

		v <<= 9;
		v2 *= {2, 0}; //512

		CHECK(v == v2);
	}
}

TEST_CASE("Right Shift", "[scalar_t::uintv_t]")
{
	using U = uintv_t<uint8_t, 2>;

	{
		U v; v.Random();
		U v2 = v;

		v >>= 1;
		v2 /= 2;

		CHECK(v == v2);
	}

	{
		U v; v.Random();
		U v2 = v;

		v >>= 2;
		v2 /= 4;

		CHECK(v == v2);
	}

	{
		U v; v.Random();
		U v2 = v;

		v >>= 3;
		v2 /= 8;

		CHECK(v == v2);
	}

	{
		U v; v.Random();
		U v2 = v;

		v >>= 4;
		v2 /= 16;

		CHECK(v == v2);
	}

	{
		U v; v.Random();
		U v2 = v;

		v >>= 5;
		v2 /= 32;

		CHECK(v == v2);
	}

	{
		U v; v.Random();
		U v2 = v;

		v >>= 6;
		v2 /= 64;

		CHECK(v == v2);
	}

	{
		U v; v.Random();
		U v2 = v;

		v >>= 7;
		v2 /= 128;

		CHECK(v == v2);
	}

	{
		U v; v.Random();
		U v2 = v;

		v >>= 8;
		v2 /= { 1,0 };

		CHECK(v == v2);
	}

	{
		U v; v.Random();
		U v2 = v;

		v >>= 9;
		v2 /= { 2,0 };

		CHECK(v == v2);
	}
}

TEST_CASE("Division", "[scalar_t::uintv_t]")
{
	using U = uintv_t<uint8_t, 2>;

	U n = { 0xfa,0xe0 }, d = { 0x00, 0x02 };

	n /= d;
}



TEST_CASE("MultiplicativeInverse", "[scalar_t::uintv_t]")
{
	using U = uintv_t<uint8_t, 2>;
	U v = { 0x21,0xe7 }, rr = { 0x61, 0xd7 }; // 8679
	
	auto mul_inv = v.MultiplicativeInverse();

	v *= mul_inv;

	CHECK(v == 1);
}

TEST_CASE("MultiplicativeInverse 128bit", "[scalar_t::uintv_t]")
{
	using U = uintv_t<uint64_t, 2>;
	auto constexpr rep = 10;

	for (size_t i = 0; i < rep; i++)
	{
		U v; v.Random();

		if (v % 2 == 0)
			++v;

		auto mul_inv = v.MultiplicativeInverse();

		v *= mul_inv;

		CHECK(v == 1);
	}
}

TEST_CASE("MultiplicativeInverse 256bit", "[scalar_t::uintv_t]")
{
	using U = uintv_t<uint64_t, 4>;
	auto constexpr rep = 10;

	for (size_t i = 0; i < rep; i++)
	{
		U v; v.Random();

		if (v % 2 == 0)
			++v;

		auto mul_inv = v.MultiplicativeInverse();

		v *= mul_inv;

		CHECK(v == 1);
	}
}

TEST_CASE("MultiplicativeInverse 512bit", "[scalar_t::uintv_t]")
{
	using U = uintv_t<uint64_t, 8>;
	auto constexpr rep = 10;

	for (size_t i = 0; i < rep; i++)
	{
		U v; v.Random();

		if (v % 2 == 0)
			++v;

		auto mul_inv = v.MultiplicativeInverse();

		v *= mul_inv;

		CHECK(v == 1);
	}
}

TEST_CASE("MultiplicativeInverse 1024bit", "[scalar_t::uintv_t]")
{
	using U = uintv_t<uint64_t, 16>;
	auto constexpr rep = 1;

	for (size_t i = 0; i < rep; i++)
	{
		U v; v.Random();

		if (v % 2 == 0)
			++v;

		auto mul_inv = v.MultiplicativeInverse();

		v *= mul_inv;

		CHECK(v == 1);
	}
}

TEST_CASE("MultiplicativeInverse 2048bit", "[scalar_t::uintv_t]")
{
	using U = uintv_t<uint64_t, 32>;
	auto constexpr rep = 1;

	for (size_t i = 0; i < rep; i++)
	{
		U v; v.Random();

		if (v % 2 == 0)
			++v;

		auto mul_inv = v.MultiplicativeInverse();

		v *= mul_inv;

		CHECK(v == 1);
	}
}

TEST_CASE("MultiplicativeInverse 4096bit", "[scalar_t::uintv_t]")
{
	using U = uintv_t<uint64_t, 64>;
	auto constexpr rep = 1;

	for (size_t i = 0; i < rep; i++)
	{
		U v; v.Random();

		if (v % 2 == 0)
			++v;

		auto mul_inv = v.MultiplicativeInverse();

		v *= mul_inv;

		CHECK(v == 1);
	}
}