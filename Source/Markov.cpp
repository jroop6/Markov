/**************************************************************************************************
 * Author: Jonathan Roop                                                                          *
 *                                                                                                *
 * A program for generating gibberish from one or more source texts using a Markov chain. A       *
 * simple GUI makes it easy for a user to add whatever text files he/she desires and provides     *
 * additional options to customize the Markov chain and length of the output. Results are         *
 * displayed in a selectable textbox for easy copy/pasting.                                       *
 *                                                                                                *
 * This file is the entry point into the program and merely instantiates the main window and      *
 * starts the message loop. The GUI display is handled in the MarkovMainWindow class, and most of *
 * the under-the-hood work is performed by the StringChain class, which utilizes the Prefix,      *
 * Suffix, and Random classes.                                                                    *
 **************************************************************************************************/

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include "MarkovMainWindow.h"
#include <Windows.h>	 

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// create and show the main window:
	MarkovMainWindow mainWin;
	if (mainWin.Create(L"Markov Generator", WS_OVERLAPPEDWINDOW, 840, 535, CW_USEDEFAULT,CW_USEDEFAULT, 0))
	{
		ShowWindow(mainWin.Window(), nCmdShow);

		// Initialize Windows' Component Object Model (COM)
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hr))
		{
			// Run message loop:
			MSG msg = {};
			BOOL messageReturnValue;
			while ((messageReturnValue = GetMessage(&msg, NULL, 0, 0)) !=0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// Uninitialize COM:
			CoUninitialize();
		}
	}

	return 0;
}
