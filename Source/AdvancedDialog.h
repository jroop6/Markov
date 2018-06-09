#pragma once

class AdvancedDialog : public BaseDialog<AdvancedDialog>
{
public:
	BOOL CALLBACK messageHandler(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
};

BOOL CALLBACK AdvancedDialog::messageHandler(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_INITDIALOG:
	{
		SetWindowPos(m_hwnd, HWND_TOP, 50, 50, 200, 200, 0);
		return TRUE;
	}
	case WM_COMMAND:
	{
		return TRUE;
	}
	case WM_CLOSE:
	{
		DestroyWindow(m_hwnd);
		advancedDialog = NULL;
		return TRUE;
	}
	default:
		return FALSE;
		//return messageHandler(hwnd, Message, wParam, lParam);
	}
}