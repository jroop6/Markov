class LittleWindow : public BaseWindow<LittleWindow>  
{
public:	
   PCWSTR ClassName() const { return L"button"; }
   LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
   {
  	  switch (uMsg)
 	  {
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

