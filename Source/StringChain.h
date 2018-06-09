// A class for constructing a Markov chain from one or more input texts and generating gibberish
// from the Markov chain.

#pragma once

#include "Prefix.h"
#include "Suffix.h"
#include "Random.h"
#include <map>
#include <list>
#include <string>
#include <fstream>

#define NONWORD L""
class StringChain
{
	const int markovOrder;
	std::map<Prefix*,Suffix*,Prefix::cmpPointees> prefixSuffixMap;
	std::list<std::wstring> currentPrefix;
	std::wstring nextToken;
	int multiples = 0;

public:
	// Constructor.
	StringChain(int order); 

	// Adds all Prefixes and Suffixes from the given input filestream to the Markov Chain.
	void AddItems(std::wifstream & filestream, std::wstring tokenType);
	
	// Generates a string of gibberish from the Markov Chain.
	std::wstring generate(int n, int order, std::wstring tokenType, Random rand);
	
	// Frees all memmory that was allocated for the <Prefix, Suffix> map.
	void deleteMap();
	
	// Constructs a single string containing all tokens of the current prefix, separated by spaces.
	std::wstring PrintCurrentPrefix();
	
	// Constructs a string to display nextToken.
	std::wstring StringChain::printnextToken();

	// Constructs a string containing a readable representation of the entire prefix-suffx map.
	std::wstring printMap();	
};