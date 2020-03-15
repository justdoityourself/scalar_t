/* Copyright (C) 2020 D8DATAWORKS - All Rights Reserved */

#pragma once

#include <array>

#include "../catch.hpp"

#include "int.hpp"

using namespace scalar_t;

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

		finite_vector_add<U>(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}

	{
		//Sym2

		C t1 = { 4,4,4,4 }, t2 = { 1,2,3,4 }, expected = { 5,6,7,8 }, result;

		finite_vector_add<U>(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}

	{
		//Sym3

		C t1 = { 4,3,2,1 }, t2 = { 1,2,3,4 }, expected = { 5,5,5,5 }, result;

		finite_vector_add<U>(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}

	{
		//Overflow1

		C t1 = { 255,30,20,10 }, t2 = { 2,2,3,4 }, expected = { 1,32,23,14 }, result;

		finite_vector_add<U>(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}

	{
		//Overflow2

		C t1 = { 40,3,2,1 }, t2 = { 10,252,253,255 }, expected = { 51,0,0,0 },result;

		finite_vector_add<U>(t1, t2, result);

		CHECK(std::equal(result.begin(), result.end(), expected.begin()));
	}
}