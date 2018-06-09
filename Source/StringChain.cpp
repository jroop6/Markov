/**************************************************************************************************
 * Author: Jonathan Roop                                                                          *
 *                                                                                                *
 * A class for constructing a Markov chain from one or more input texts and for generating        *
 * gibberish from the Markov chain. A "Markov chain" is an association of "Prefixes" to lists of  *
 * "Suffixes." For example, a prefix for Shakespeare's Hamlet might be "Rosencrantz and" which    *
 * has the associated Suffix "Guildestern." All prefixees in the Markov chain have an order       *
 * associated with them, so the Prefixes in a second-order Markov chain all contain exactly two   *
 * words, the Prefixes in a third-order Markov chain all contain exactly three words, etc. Some   *
 * prefixes may have multiple possible suffixes associated with them. For example, the Prefix "I  *
 * went" may be followed by "forward" or "back" or "away." When reading input, *all* prefixes in  *
 * the text are stored along with a list of their possible Suffixes. When generating gibberish    *
 * from the Markov chain, a random item is chosen from the list of possible Suffixes. This        *
 * generates text can sound authentic, although it is oftentimes gramatically incorrect or        *
 * nonsenscial to a comical effect.                                                               *
 **************************************************************************************************/

#include "StringChain.h"

/**************************************************************************************************
 * Constructor. The "currentPrefix" buffer is initialized with non-word padding.                  *
 *   Inputs:                                                                                      *
 *      order: How many words or characters per Prefix.                                           *
 **************************************************************************************************/
StringChain::StringChain(int order) : markovOrder(order)
{
	for(int i=0; i<order; ++i){
		currentPrefix.push_back(NONWORD); 
	}
}

/**************************************************************************************************
 * Reads words or characters from the given input filestream and adds all of its Prefixes and     *
 * Suffixes to the Markov chain. Tokens (words or characters) are read one at a time.             *
 *   Inputs:                                                                                      *
 *      filestream: An opened input filestream                                                    *
 *      tokenType: A string indicating whether words or characters are being used for the Markov  *
 *                  chain. Allowed values: "words", "characters".                                 *
 *   return value: none                                                                           *
 **************************************************************************************************/
void  StringChain::AddItems(std::wifstream & filestream, std::wstring tokenType)
{
	do{ 
		// read the next token
		if (tokenType == L"words") filestream >> nextToken; // read a word
		else nextToken = std::wstring(1,filestream.get());  // read a character
		
		// If the user specified an empty file, just put in a nonword entry. This ensures that
		// there's at least *something* in the prefixsuffixmap so that generate() won't explode.
		if (filestream.eof() && prefixSuffixMap.empty()) nextToken = NONWORD;
		
		// Check to see whether the prefix already exists in the map:
		Prefix tempPrefix(currentPrefix);
		auto it = prefixSuffixMap.find(&tempPrefix);
		
		// If it doesn't, then add this <Prefix,Suffix> pair to the map:
		if(it == prefixSuffixMap.end()){
			std::list<std::wstring> newSuffixList;
			newSuffixList.push_back(nextToken);
			prefixSuffixMap.insert(std::make_pair(new Prefix(currentPrefix), 
				                                  new Suffix(newSuffixList)));
		}
		
		// If it does, then grab the word list of the suffix and add nextToken to it.
		else{ 
			(*(it->second)).AddSuffix(nextToken);
			multiples++; // for debugging pursposes
		}
	
		// Advance the buffer by 1 token:
		currentPrefix.push_back(nextToken);
		currentPrefix.pop_front();
	} while (!filestream.eof());

	//add nonword padding to the end. 
	nextToken = NONWORD;
	for(int i=0; i<markovOrder; ++i){
		Prefix tempPrefix(currentPrefix);
		auto it = prefixSuffixMap.find(&tempPrefix);
		if(it == prefixSuffixMap.end()){
			std::list<std::wstring> newSuffixList;
			newSuffixList.push_back(nextToken);
			prefixSuffixMap.insert(std::make_pair(new Prefix(currentPrefix), 
				                                  new Suffix(newSuffixList)));
		}
		else{ 
			(*(it->second)).AddSuffix(nextToken);
			multiples++;
		}
		currentPrefix.push_back(nextToken);
		currentPrefix.pop_front();
	}
}

/**************************************************************************************************
 * Generates a string of gibberish from the Markov Chain. Beginning with a random Prefix, A word  *
 * is chosen at random from the list of that Prefix's possible Suffixes and added to the output.  *
 * The first word of the current Prefix is then discarded and the chosen Suffix becomes the last  *
 * token of the current Prefix for the next randomly-chosen word.                                 *
 *   Inputs:                                                                                      *
 *      numGen: The number of words or characters to be generated.                                *
 *      order: The order of the Markov chain. That is, the number of words/characters per Prefix  *
 *      tokenType: A string indicating whether words or characters are being used for the Markov  *
 *                  chain. Allowed values: "words", "characters".                                 *
 *      rand: An object of type Random (pseudorandom number generator)                            *
 *   return value: A string containing numGen tokens of generated gibberish.                      *
 **************************************************************************************************/
std::wstring StringChain::generate(int numGen, int order, std::wstring tokenType, Random rand)
{	
	std::wstring output;

	// Select a random prefix to begin the Markov generation. Assign it to the curent buffer.
	int startingPrefixIndex = rand.nextInt(prefixSuffixMap.size());
	std::map<Prefix*, Suffix*>::iterator startingPrefixPointer = prefixSuffixMap.begin();
	for (int i = startingPrefixIndex; i > 0; i--) ++startingPrefixPointer;
	currentPrefix = (startingPrefixPointer->first)->GetPrefix();

	for(int i=0; i<numGen; ++i){
		// find the Prefix in prefixSuffixMap corresponding to the current buffer:
		Prefix tempPrefix(currentPrefix);
		auto mapEntry = prefixSuffixMap.find(&tempPrefix);
		
		// If a prefix somehow doesn't exist in the map, stop execution and print an error message:
		if (mapEntry == prefixSuffixMap.end())
		{
			output += L"Error! The Prefix \""; 
			for (auto it = tempPrefix.GetPrefix().begin(); it != tempPrefix.GetPrefix().end(); ++it)
			{
				std::wstring prefixString = *it;
				std::wstring prefixWString;
				prefixWString.assign(prefixString.begin(), prefixString.end());
				output += L" " + prefixWString;
			}
			output += L"\" does not exist in map. There must be an error in the program's ";
			output += L"logic somewhere. The length of the prefix is ";
			output += std::to_wstring(tempPrefix.GetPrefix().size()) + L"\r\n";
			output += printMap();
			return output;
		}

		// Choose one of the Prefix's possible Suffixes:
		nextToken = (*(mapEntry->second)).GetRandomSuffix(rand);
	
		// advance the buffer by 1 word:
		currentPrefix.push_back(nextToken);
		currentPrefix.pop_front();
	
		//save the word to the output, unless it is a nonword:
		if(nextToken != NONWORD)
		{
			if(tokenType == L"words") output += L" " + nextToken;
			else output += nextToken;
		}
	}

	return output;
}

/**************************************************************************************************
 * Frees all memmory that was allocated for the <Prefix, Suffix> map. Called after gibberish is   *
 * generated to prevent memory leakage.                                                           *
 *   return value: none                                                                           *
 **************************************************************************************************/
void StringChain::deleteMap() 
{
	for (auto it = prefixSuffixMap.begin(); it != prefixSuffixMap.end(); ++it) 
	{
		delete it->second;
		delete it->first;
	}
}

// Debugging utilities

/**************************************************************************************************
 * Constructs a single string containing all tokens of the current prefix, separated by spaces.   *
 * Created for debugging purposes.                                                                *
 *   return value: A string representation of currentPrefix.                                      *
 **************************************************************************************************/
std::wstring StringChain::PrintCurrentPrefix()
{
	std::wstring output = L"currentPrefix: {";
	for(auto it=currentPrefix.begin(); it != currentPrefix.end(); ++it)
	{
		output += (*it) + L" ";
	}
	output += L"}";
	return output;
}

/**************************************************************************************************
 * Constructs a string to display nextToken. Created for debugging purposes.                      *
 *   return value: A modified string representation of nextToken.                                 *
 **************************************************************************************************/
std::wstring StringChain::printnextToken()
{
	std::wstring output = L"next token: " + nextToken;
	return output;
}

/**************************************************************************************************
 * Generates a single string containing a readable representation of the entire prefix-suffx map. *
 * Created for debugging purposes.                                                                *
 *   return value: A string each prefix-suffix pairing on its own line                            *
 **************************************************************************************************/
std::wstring StringChain::printMap(){
	std::wstring output;
	for(auto it = prefixSuffixMap.begin(); it != prefixSuffixMap.end(); ++it)
	{
		output += L"PREFIX {";
		output += (*(it->first)).GetPrefixString();
		output += L"}; SUFFIXES {";
		output += (*(it->second)).GetAllSuffixes();
		output += L"}\r\n";
	}	
	output += L"pairs with multiple suffixes: " + std::to_wstring(multiples);
	return output;
}
