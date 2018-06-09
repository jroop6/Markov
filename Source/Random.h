// A wrapper class for the std::rand pseudorandom number generator.

#pragma once

#include <ctime>
#include <cstdlib>
class Random
{
public:
	// Easy constructor.
	Random();
	
	// Explicit constructor.
	Random(int seed);

	// Computes an pseudorandom integer between 0 and maxValue - 1, inclusive.
	int nextInt(int maxVal);
};

