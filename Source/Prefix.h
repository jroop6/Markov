#pragma once

#include <string>
#include <list>

class Prefix
{
	std::list<std::wstring> prefixWords;
public:
	// Constructor
	Prefix(std::list<std::wstring> prefixSetter);

	// Accessor for the private list of prefix items.
	std::list<std::wstring> GetPrefix();

	// Constructs a single string containing all the words or characters of the prefix
	std::wstring GetPrefixString();

	/**************************************************************************************************
     * Comparator needed for prefixSuffixMap's find() function. Note: the way the find() function     *
	 * works is that it asks "is a < b?" and then "is b < a?" by invoking cmpPointees(a,b) and        *
	 * cmpPointees(b,a), respectively. If both results are false, then a must be equal to b.          *
     **************************************************************************************************/
	struct cmpPointees
	{
		bool operator () (Prefix * lhs, Prefix * rhs) const
		{
			return ((lhs->prefixWords)<(rhs->prefixWords));
		}
	};
};




