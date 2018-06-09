#pragma once

/**************************************************************************************************
 * A simple utility function for safely releasing a COM interface pointer. Use this on the        *
 * pointer assigned in CoCreateInstance once you're finished with the COM object.                 *
 **************************************************************************************************/
template <typename T> void SafeRelease(T** ptr)
{
	if (*ptr)
	{
		(*ptr)->Release();
		(*ptr) = NULL;
	}
}