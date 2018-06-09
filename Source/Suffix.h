// A class for storing "suffixes" for Markov chain generation.

#pragma once

#include "Random.h"
#include <string>
#include <list>

class Suffix{
	std::list<std::wstring> possibleSuffixes;
public:
	// Constructor
	Suffix(std::list<std::wstring> suffixSetter);

	// A method for adding a word to the list of possible suffixes. 
	void AddSuffix(std:: wstring newSuffix);

	// Retrieves a random word (or character) from the list of possible suffixes.
	std::wstring GetRandomSuffix(Random rand);

	// Constructs a string containing all the words or characters in the suffix list.
	std::wstring GetAllSuffixes();
};
