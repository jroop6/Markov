/**************************************************************************************************
 * Author: Jonathan Roop                                                                          *
 *                                                                                                *
 * A class for storing "prefixes" for Markov chain generation. A prefix is a sequence of words or *
 * characters appearing in a given text. The number of items in a prefix is dictated by the       *
 * "order" of the markov chain. For example, 3rd-order word prefixes from Romeo and Juliet would  *
 * include "Wherefore art thou" and "But soft! What".                                             *
 **************************************************************************************************/

#include "Prefix.h"

/**************************************************************************************************
 * Constructor. Creates a prefix using a given list of words.                                     *
 **************************************************************************************************/
Prefix::Prefix(std::list<std::wstring> prefixSetter) : prefixWords(prefixSetter){}

/**************************************************************************************************
 * Accessor for the private list of prefix items.                                                 *
 *   return value: the list of prefix tokens                                                      *
 **************************************************************************************************/
std::list<std::wstring> Prefix::GetPrefix()
{
	return prefixWords;
}

/**************************************************************************************************
 * Constructs a single string containing all the words or characters of the prefix, separated by  *
 * spaces. Created for debugging purposes.                                                        *
 *   return value: A string representation of the entire prefix.                                  *
 **************************************************************************************************/
std::wstring Prefix::GetPrefixString()
{
	std::wstring output = L"";
	for (auto it = prefixWords.begin(); it != prefixWords.end(); ++it)
	{
		output += (*it) + L" ";
	}
	return output;
}

