#pragma once
//#include <Windows.h>
//#include "BaseWindow.h"

class AdvancedWindow : public BaseWindow<AdvancedWindow>
  {
  	  public:	
 	  PCWSTR ClassName() const { return L"Advanced Options Window Type"; }
 	  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
 	  {
 	 	  switch (uMsg)
 		  {
		  case WM_CREATE:
		  {

		  }
 		  case WM_CLOSE:
			  DestroyWindow(m_hwnd);
			  return 0;
		  case WM_DESTROY:
			  PostQuitMessage(0);
			  return 0;
 		  default:
 			  return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
 		  }
 	  }
  };
 