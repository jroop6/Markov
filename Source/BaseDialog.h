#pragma once

// A template for creating Dialog Windows in an object-oriented fashion. 

template <class DERIVED_TYPE> class BaseDialog
{
public:
	BaseDialog() : m_hwnd(NULL), resultPointer(NULL), ownerHWND(NULL) {}
	HWND Create(
		LPCTSTR dialogID, 			// unique identifier for this window
		HWND ownerHWND = NULL, 	    // owner window handle. If null, attempts to use the desktop window.
		HINSTANCE hInstance = NULL) // name of the "module" containing the template for this DialogBox. If unspecified, use the .exe file used to call this process.
	{
		//INT_PTR (CALLBACK DERIVED_TYPE::*messageHandlerPointer)(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
		//messageHandlerPointer = &DERIVED_TYPE::globalMessageHandler;
		//BOOL CALLBACK (*messageHandlerPointer)(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
		//messageHandlerPointer = &BaseDialog::globalMessageHandler;
		if (ownerHWND == NULL)
		{
			ownerHWND = GetDesktopWindow();
		}
		m_hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(dialogID), ownerHWND, globalMessageHandler);
		return m_hwnd;
		//resultPointer = DialogBox(dialogHandle,dialogID,parentHWND,DERIVED_TYPE::globalMessageHandler);
	}

	static BOOL CALLBACK globalMessageHandler(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	HWND Window() const { return m_hwnd; }

protected:
	INT_PTR resultPointer;
	HWND m_hwnd;
	HWND ownerHWND;
	//virtual void initializer() = 0;
	virtual BOOL CALLBACK messageHandler(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) = 0;
};


/*template <class DERIVED_TYPE> void BaseDialog<DERIVED_TYPE>::globalInitializationRoutine()
{
	// Find a suitable parent handle if none was provided.
	if(ownerHWND == NULL)
	{
		if((ownerHWND = GetParent()) == NULL)
		{
			ownerHWND = GetDesktopWindow();
		}
	}
	// now call the custom initializer for the derived class: 
	//initializer();
}*/

template <class DERIVED_TYPE> BOOL CALLBACK BaseDialog<DERIVED_TYPE>::globalMessageHandler(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	DERIVED_TYPE * currentState = NULL;

	// Get the state info for the window, setting the window pointer if this is the initial call.
	if (Message == WM_INITDIALOG)
	{
		// retrieve the CREATESTRUCT structure pointer from lParam:
		CREATESTRUCT * pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		// retrieve the pointer to the custom-defined structure from pCreate:
		currentState = (DERIVED_TYPE*)(pCreate->lpCreateParams);
		// store the WindowState pointer in the window instance:
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)currentState);
		currentState->m_hwnd = hwnd;
	}
	else
	{
		currentState = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}
	if (currentState) // A currentState exists. Call its HandleMessage() method.
	{
		return currentState->messageHandler(hwnd, Message, wParam, lParam);
	}
	else
	{
		return FALSE;
	}

	/*switch(Message)
	{
	case WM_INITDIALOG:
		{
			globalInitializationRoutine();
			return TRUE;
		}
	default:
		return FALSE;
		//return messageHandler(hwnd, Message, wParam, lParam);
	}*/
}

//Note to self: Destroy modal dialogs (not this kind of dialog) like so:
//if(!(EndDialog(m_hwnd,resultPointer)))
//{
//	  //Do some error processing (use GetLastError?)
//}
//return;