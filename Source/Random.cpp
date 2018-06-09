/**************************************************************************************************
 * Author: Jonathan Roop                                                                          *
 *                                                                                                *
 * A wrapper class for the std::rand pseudorandom number generator. The generator can be          *
 * initialized with an explicit seed value or automatically initialized using the current system  *
 * time in seconds. This wrapper class also provides a convenient nextInt method for generating a *
 * bounded pseudorandom integer.                                                                  *
 **************************************************************************************************/

#include "Random.h"

 /**************************************************************************************************
  * Easy constructor. Seeds the pseudorandom number generator using the current system time. Note: *
  * std::time(nullptr) usually returns the current time since the Epoch *in seconds*. This means   *
  * that a unique seed would be produced by this constructor only once per second.                 *
  **************************************************************************************************/
Random::Random(){
	std::srand((int)std::time(nullptr));
}

/**************************************************************************************************
 * Explicit constructor. Seeds the pseudorandom number generator using the seed parameter.        *
 **************************************************************************************************/
Random::Random(int seed){
	std::srand(seed);
}

/**************************************************************************************************
 * Computes an pseudorandom integer between 0 and maxValue-1, inclusive. The std::rand class is   *
 * used to generate a random integer, which is then truncated using the modulo operator. Note:    *
 * the upper bound on std::rand() is RAND_MAX, which is not guaranteed to be as large as INT_MAX  *
 * (although it probably is anyways).                                                             *
 *   Inputs:                                                                                      *
 *      maxValue: The upper bound on the pseudorandom number to be generated.                     *
 *   return value: A pseudorandom integer between 0 and maxValue-1, inclusive.
 **************************************************************************************************/
int Random::nextInt(int maxValue){
	return std::rand() % maxValue;
}