/* Copyright (C) 2020 D8DATAWORKS - All Rights Reserved */

#pragma once


#include <chrono>
#include <array>

#include "../catch.hpp"

#include "int.hpp"

using namespace scalar_t;

TEST_CASE("fuse invert", "[scalar_t::uintv_t]")
{
	{
		using U = uintv_t<uint8_t, 4>; 

		uint32_t test_r = 0xda11d269;
		uint32_t test_m = 0x1500985d;

		uint32_t test_v = test_r;
		uint32_t test_inv = 0;
		test_inv -= test_m;
		test_v += test_inv;

		U r{ 0xda,0x11,0xd2,0x69 };
		U m4{ 0x15,0x00,0x98,0x5d };

		auto v1 = r;
		auto v2 = r;

		auto inv = U(0) - m4;
		v1 += inv;

		v2.INVADD(m4);

		CHECK(v2 == v1);
	}

	{
		using U = uintv_t<uint8_t, 4>; 

		uint32_t test_r = 0xb15f00ec;
		uint32_t test_m = 0xc6e90013;

		uint32_t test_v = test_r;
		uint32_t test_inv = 0;
		test_inv -= test_m;
		test_v += test_inv;

		U r{ 0xb1,0x5f,0x00,0xec };
		U m4{ 0xc6,0xe9,0x00,0x13 };

		auto v1 = r;
		auto v2 = r;

		auto inv = U(0) - m4;
		v1 += inv;

		v2.INVADD(m4);

		CHECK(v2 == v1);
	}

	{
		using U = uintv_t<uint8_t, 4>; //32bit

		U r; r.Random();
		U m4; m4.Random();

		auto v1 = r;
		auto v2 = r;

		auto inv = U(0) - m4;
		v1 += inv;

		v2.INVADD(m4);

		CHECK(v2 == v1);
	}

	{
		using U = uintv_t<uint8_t, 4>; //32bit

		U r; r.Random();
		U m4; m4.Random();

		m4[3] = 0; //Test alter carry pattern.

		auto v1 = r;
		auto v2 = r;

		auto inv = U(0) - m4;
		v1 += inv;

		v2.INVADD(m4);

		CHECK(v2 == v1);
	}

	{
		using U = uintv_t<uint64_t, 16>; //1024bit

		U r; r.Random();
		U m4; m4.Random();

		auto v1 = r;
		auto v2 = r;

		auto inv = U(0) - m4;
		v1 += inv;

		v2.INVADD(m4);

		CHECK(v2 == v1);
	}

	{
		using U = uintv_t<uint64_t, 16>;

		U r; r.Random();
		U m4; m4.Random();

		m4[15] = 0; //Test alter carry pattern.

		auto v1 = r;
		auto v2 = r;

		auto inv = U(0) - m4;
		v1 += inv;

		v2.INVADD(m4);

		CHECK(v2 == v1);
	}

	for (size_t i = 0; i < 65*1000; i++)
	{
		using U = uintv_t<uint8_t, 4>;

		U r; r.Random();
		U m4; m4.Random();

		auto v1 = r;
		auto v2 = r;

		auto inv = U(0) - m4;
		v1 += inv;

		v2.INVADD(m4);

		if (v2 != v1)
			std::cout << r.string() << " " << m4.string() << std::endl;

		CHECK(v2 == v1);
	}

	for (size_t i = 0; i < 1000; i++)
	{
		using U = uintv_t<uint64_t, 16>;

		U r; r.Random();
		U m4; m4.Random();

		auto v1 = r;
		auto v2 = r;

		auto inv = U(0) - m4;
		v1 += inv;

		v2.INVADD(m4);

		CHECK(v2 == v1);
	}
}

TEST_CASE("finite_vector_fuse_multiply3_invadd", "[scalar_t::uintv_t]")
{
	{
		using U = uintv_t<uint8_t, 3>;

		U r{ 0,0,0 };
		U m1{ 0x1,0x1,0x1 };
		U m2{ 0x1,0x1,0x1 };
		U m3{ 0x1,0x1,0x1 };

		auto v1 = r;
		auto v2 = r;


		v2 += U(0) - m1 * m2 * m3;

		v1.FM3IAD(m1, m2, m3);

		CHECK(v2 == v1);
	}

	{
		using U = uintv_t<uint64_t, 16>;

		U r{ 0,0,0,0 };
		U m1{ 0x1,0x1,0x1, 8 };
		U m2{ 0x1,8,0x1,45 };
		U m3{ 46,65,0x1,32 };

		auto v1 = r;
		auto v2 = r;


		v2 += U(0) - m1 * m2 * m3;

		v1.FM3IAD(m1, m2, m3);

		CHECK(v2 == v1);
	}

	{
		using U = uintv_t<uint8_t, 4>;

		U r{ 0xd0,0x14,0xef,0x0c };
		U m1{ 0x21,0x6d,0xfd, 0x2b };
		U m2{ 0xc0,0x98,0xc7,0x79 };
		U m3{ 0x3d,0x9f,0x72,0x7d };

		auto v1 = r;
		auto v2 = r;
		auto v3 = r;


		v2 += U(0) - m1 * m2 * m3;

		v1.FM3IAD(m1, m2, m3);
		v3.FM3IAD_basic(m1, m2, m3);

		CHECK(v3 == v1);
		CHECK(v2 == v1);
	}

	{
		using U = uintv_t<uint8_t, 4>;

		U r{ 0xca,0x92 ,0xd9 ,0xd5 };
		U m1{ 0x95 ,0x8f ,0x54 ,0xe1 };
		U m2{ 0xd1 ,0xa8 ,0x9b ,0x15 };
		U m3{ 0x97 ,0x9b ,0xdc ,0x89 };

		auto v1 = r;
		auto v2 = r;


		v2 += U(0) - m1 * m2 * m3;

		v1.FM3IAD(m1, m2, m3);

		CHECK(v2 == v1);
	}

	{
		using U = uintv_t<uint8_t, 4>;

		U r{ 0xd0,0x14,0xef,0x0c };
		U m1{ 0x21,0x6d,0xfd, 0x2b };
		U m2{ 0xc0,0x98,0xc7,0x79 };
		U m3{ 0x3d,0x9f,0x72,0x7d };

		auto v1 = r;
		auto v2 = r;


		v2 += U(0) - m1 * m2 * m3;

		v1.FM3IAD(m1, m2, m3);

		CHECK(v2 == v1);
	}

	{
		using U = uintv_t<uint8_t, 4>;

		U r{ 0x31 ,0x74 ,0x3c ,0x31 };
		U m1{ 0x78 ,0x3c ,0xf5 ,0x0a };
		U m2{ 0xf3 ,0x19 ,0xff ,0x4f };
		U m3{ 0xde ,0x66,0xce ,0xb1 };

		auto v1 = r;
		auto v2 = r;
		auto v3 = r;


		v2 += U(0) - m1 * m2 * m3;

		v1.FM3IAD(m1, m2, m3);
		v3.FM3IAD_basic(m1, m2, m3);

		CHECK(v3 == v1);
		CHECK(v2 == v1);
	}

	for(size_t i = 0; i < 65 * 1000; i++)
	{
		using U = uintv_t<uint8_t, 4>;

		U r; r.Random();
		U m1; m1.Random();
		U m2; m2.Random();
		U m3; m3.Random();

		auto v1 = r;
		auto v2 = r;

		v2 += U(0) - m1 * m2 * m3;

		v1.FM3IAD(m1, m2, m3);

		if (v2 != v1)
			std::cout << r.string() << " " << m1.string() << " " << m2.string() << " " << m3.string() << std::endl;

		CHECK(v2 == v1);
	}
}

TEST_CASE("invadd bench", "[scalar_t::uintv_t]")
{
	using U = uintv_t<uint64_t, 16>;

	U r; r.Random();
	U m1; m1.Random();

	auto v1 = r;
	auto v2 = r;

	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		for (size_t i = 0; i < 100; i++)
			v2 += U(0) - m1;

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout << "Default = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
	}

	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		for (size_t i = 0; i < 100; i++)
			v1.INVADD(m1);

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout << "INVADD = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
	}

	CHECK(v2 == v1);
}

TEST_CASE("finite_vector_fuse_multiply3_invadd bench loop", "[scalar_t::uintv_t]")
{
	using U = uintv_t<uint64_t, 16>;

	U r; r.Random();
	U m1; m1.Random();
	U m2; m2.Random();
	U m3; m3.Random();

	auto v1 = r;
	auto v2 = r;
	auto v3 = r;

	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		for(size_t i = 0; i < 100; i ++)
			v2 += U(0) - m1 * m2 * m3;

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout << "Default = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
	}

	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		for (size_t i = 0; i < 100; i++)
			v3.FM3IAD_basic(m1, m2, m3);

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout << "FMA_basic = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
	}

	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		for (size_t i = 0; i < 100; i++)
			v1.FM3IAD(m1, m2, m3);

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout << "FMA = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
	}

	CHECK(v2 == v1);
}

TEST_CASE("rolling over carry order", "[scalar_t::uintv_t]")
{
	{
		using U = uintv_t<uint8_t, 2>;

		uint16_t z1 = 0;
		U z2;

		z1--;
		z2 -= {0, 1};

		CHECK((z2 == U(uint8_t(z1 >> 8), uint8_t(z1 & 0xff))));
	}

	{
		using U = uintv_t<uint8_t, 2>;

		uint16_t z1 = 0;
		U z2;

		z1-=2;
		z2 -= {0, 2};

		CHECK((z2 == U(uint8_t(z1 >> 8), uint8_t(z1 & 0xff))));
	}

	{
		using U = uintv_t<uint8_t, 2>;

		uint16_t z1 = 0;
		U z2;

		z1 -= 4097;
		z2 -= {0x10, 0x01};

		CHECK((z2 == U(uint8_t(z1 >> 8), uint8_t(z1 & 0xff))));
	}

	{
		using U = uintv_t<uint8_t, 2>;

		uint16_t z1 = 0;
		U z2;

		z1 -= 4096;
		z2 -= {0x10, 0x00};

		CHECK((z2 == U(uint8_t(z1 >> 8), uint8_t(z1 & 0xff))));
	}

	{
		using U = uintv_t<uint8_t, 2>;

		uint16_t z1 = 0;
		U z2;

		z1 -= 61680;
		z2 -= {0xf0, 0xf0};

		CHECK((z2 == U(uint8_t(z1 >> 8), uint8_t(z1 & 0xff))));
	}

	{
		using U = uintv_t<uint8_t, 4>;

		uint32_t z1 = 0;
		U z2;

		z1 -= 65281;
		z2 -= {0,0,0xff, 0x01};

		CHECK(true);
	}

	{
		using U = uintv_t<uint8_t, 4>;

		uint32_t z1 = 0;
		U z2;

		z1 -= 0xffff;
		z2 -= {0, 0, 0xff, 0xff};

		CHECK(true);
	}
}

TEST_CASE("finite_vector_fuse_multiply_add", "[scalar_t::uintv_t]")
{
	using U = uintv_t<uint64_t, 32>; //1024bit
	
	U r; r.Random();
	U m1; m1.Random();
	U m2; m2.Random();

	auto v1 = r;
	auto v2 = r;

	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		v2 += m1 * m2;

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout << "Default = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
	}

	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		v1.FMADD(m1, m2);

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout << "FMA = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
	}

	CHECK(v2 == v1);
}

TEST_CASE("finite_vector_fuse_multiply_add loop", "[scalar_t::uintv_t]")
{
	using U = uintv_t<uint64_t, 32>; //1024bit
	auto constexpr lim = 10;

	U r; r.Random();
	U m1; m1.Random();
	U m2; m2.Random();

	auto v1 = r;
	auto v2 = r;

	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		for(size_t i = 0; i < lim; i++)
			v2 += m1 * m2;

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout << "Default = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
	}

	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		for (size_t i = 0; i < lim; i++)
			v1.FMADD(m1, m2);

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout << "FMA = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
	}

	CHECK(v2 == v1);
}

TEST_CASE("standing in place multiplication", "[scalar_t::helpers]")
{
	constexpr auto S = 4;
	using U = uint8_t;
	using C = std::array<U, S>;

	{
		//Sym1

		C t1 = { 4,4,4,4 }, t2 = { 1,1,1,1 }, expected = { 16,12,8,4 };

		finite_vector_multiply<U>(t1, t2);

		CHECK(std::equal(t1.begin(), t1.end(), expected.begin()));
	}

	{
		//Sym2

		C t1 = { 4,4,4,4 }, t2 = { 1,2,3,4 }, expected = { 40,36,28,16 };

		finite_vector_multiply<U>(t1, t2);

		CHECK(std::equal(t1.begin(), t1.end(), expected.begin()));
	}

	{
		//Sym3

		C t1 = { 4,3,2,1 }, t2 = { 1,2,3,4 }, expected = { 30,20,11,4 };

		finite_vector_multiply<U>(t1, t2);

		CHECK(std::equal(t1.begin(), t1.end(), expected.begin()));
	}

	{
		//Overflow1

		C t1 = { 40,30,20,10 }, t2 = { 1,2,3,4 }, expected = { 44,200,110,40 };

		finite_vector_multiply<U>(t1, t2);

		CHECK(std::equal(t1.begin(), t1.end(), expected.begin()));
	}

	{
		//Overflow2

		C t1 = { 40,30,20,10 }, t2 = { 10,20,30,40 }, expected = { 191,212,77,144 };

		finite_vector_multiply<U>(t1, t2);

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

		finite_vector_multiply<U>(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}

	{
		//Sym2

		C t1 = { 4,4,4,4 }, t2 = { 1,2,3,4 }, expected = { 40,36,28,16 }, result;

		finite_vector_multiply<U>(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}

	{
		//Sym3

		C t1 = { 4,3,2,1 }, t2 = { 1,2,3,4 }, expected = { 30,20,11,4 }, result;

		finite_vector_multiply<U>(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}

	{
		//Overflow1

		C t1 = { 40,30,20,10 }, t2 = { 1,2,3,4 }, expected = { 44,200,110,40 }, result;

		finite_vector_multiply<U>(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}

	{
		//Overflow2

		C t1 = { 40,30,20,10 }, t2 = { 10,20,30,40 }, expected = { 191,212,77,144 }, result;

		finite_vector_multiply<U>(t1, t2, result);

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

TEST_CASE("Specific Misses", "[scalar_t::uintv_t]")
{
	{
		using U = uintv_t<uint64_t, 2>;
		U v("cb645cdfeec89666 914da98986504d99");

		auto r = v.MultiplicativeInverse();

		v *= r;

		CHECK(v == 1);
	}
}

TEST_CASE("MultiplicativeInverse 128bit", "[scalar_t::uintv_t]")
{
	using U = uintv_t<uint64_t, 2>;
	auto constexpr rep = 10;

	for (size_t i = 0; i < rep; i++)
	{
		U v; v.Random();
		U c = v;

		if (v % 2 == 0)
			++v;

		auto mul_inv = v.MultiplicativeInverse();

		v *= mul_inv;

		if (v != 1)
			std::cout << c.string() << std::endl;

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