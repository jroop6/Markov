/**************************************************************************************************
 * Author: Jonathan Roop                                                                          *
 *                                                                                                *
 * A class for storing "suffixes" for Markov chain generation. A suffix is a word or character    *
 * that tends to follow a group of other words or characters. For example, Suffixes for "Beam us  *
 * up," might include "Scotty" and "Enterprise." Since a group of words can have multiple         *
 * possible Suffixes, the suffixes are stored in a list.                                          *
 **************************************************************************************************/

#include "Suffix.h"

/**************************************************************************************************
 * Constructor. Initializes the list of possible suffixes with the list passed as a parameter.    *
 **************************************************************************************************/
Suffix::Suffix(std::list<std::wstring> suffixSetter) : possibleSuffixes(suffixSetter){}

/**************************************************************************************************
 * A method for adding a word to the list of possible suffixes.                                   *
 *   Inputs:                                                                                      *
 *      newSuffix: A std:wstring containing the suffix to add to the list of poosible suffixes.   *
 *   return value: none                                                                           *
 **************************************************************************************************/
void Suffix::AddSuffix(std::wstring newSuffix)
{
	possibleSuffixes.push_back(newSuffix);
}

/**************************************************************************************************
 * Retrieves a random word (or character) from the list of possible suffixes. This method is      *
 * called frequently while generating gibberish from the Markov chain. Note: Operator += is not   *
 * defined for list iterators, so "randomWordPointer += randomIndex" won't work. Instead, we must *
 * construct a loop and use ++. For better performance, possibleSuffixes can be made into a       *
 * std::vector instead of a std::list. Such a data structure consumes more memory.                *
 *   Inputs:                                                                                      *
 *      rand: An object of type Random (pseudorandom number generator)                            *
 *   return value: a wstring containing a single token from the suffix list.                      *
 **************************************************************************************************/
std::wstring Suffix::GetRandomSuffix(Random rand)
{
	int randomIndex = rand.nextInt(possibleSuffixes.size());
	auto randomWordPointer = possibleSuffixes.begin();
	for(int i=0; i<randomIndex; ++i) ++randomWordPointer;
	return *randomWordPointer;
}

/**************************************************************************************************
 * Constructs a string containing all the words or characters in the suffix list, separated by    *
 * commas. Created for debugging purposes.                                                        *
 *   return value: A single wstring with all the possible suffixes.                               *
 **************************************************************************************************/
std::wstring Suffix::GetAllSuffixes()
{
	std::wstring output = L"";
	for (auto it = possibleSuffixes.begin(); it != possibleSuffixes.end(); ++it)
	{
		output += (*it) + L", ";
	}
	return output;
}