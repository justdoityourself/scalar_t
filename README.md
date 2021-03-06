# scalar_t
Template Scalar Integer Library

## Introduction

[![scalar_t](http://img.youtube.com/vi/NP1FFL8TeUA/0.jpg)](https://www.youtube.com/watch?v=NP1FFL8TeUA "scalar_t")

## Implemented For

https://github.com/justdoityourself/d88

## Not a Big Integer Library

scalar_t is is a template library that allows you to define and construct integers of a custom yet fixed sized based on the native types avalable to your target architecture. 

A Big Integer Library on the other hand gives you access to an unsized integer.

## Overflow

Addition overflow of the highest element is discarded.

Multiplication overflow is also discarded as follows:


			    6 7 4
			 X  1 1 1

			6 6 6
			  7 7 7
			    4 4 4

			    6
			    7 7
			    4 4 4
			  
## Usage

```C++
#include "scalar_t/int.hpp"

using uint128_t = scalar_t::uintv_t<uint64_t,2>;
using uint1024_t = scalar_t::uintv_t<uint64_t,16>;

void test()
{
	uint128_t construct_from_string("cb645cdfeec89666 914da98986504d99");
	
	uint128_t construct_from_uint(69);
	
	uint128_t construct_from_initializer_list{ 12345,54321 };
	
	uint1024_t random_int;
	random_int.Random();
	
	auto random_int_as_string = random_int.string();
	
	auto inv = random_int.MultiplicativeInverse();
	
	auto value_of_one = random_int * inv;
	
	auto doubled_int = random_int << 1;
	auto also_doubled_int = random_int * 2;
	
	bool equal_ints = doubled_int == also_doubled_int;
	
	auto [q,m] = random_int.Divide(10);
}

```

## Additional Details

Please see scalar_t/test.hpp for a comprehensive view of how to use this library.
