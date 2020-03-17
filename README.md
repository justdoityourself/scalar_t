# scalar_t
Template Scalar Integer Library

## Not a Big Integer Library

scalar_t is is a template library that allows you to define and construct integers of a custom yet fixed sized based on the native types avalable to your target architecture. 

A Big Integer Library on the other hand gives you access to an unsized integer.

## Overflow

Addition overflow of the highest element is discarded.

Multiplication overflow is also discarded as follows:

Finite Multiplication

			    6 7 4
			 X  1 1 1

			6 6 6
			  7 7 7
			    4 4 4

			    6
			    7 7
			    4 4 4
