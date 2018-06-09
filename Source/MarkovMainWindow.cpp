/**************************************************************************************************
 * Author: Jonathan Roop                                                                          *
 *                                                                                                *
 * This class handles the GUI display for the Markov Program. The user is presented with a simple *
 * interface with buttons to add and remove files from the Markov chain, a button for generating  *
 * gibberish, and a button for opening an advanced options dialog. The advanced options dialog    *
 * provides options for changing the order of the Markov chain, the number of tokens to generate  *
 * when the Generate button is pressed, and whether to use words or characters for the Markov     *
 * chain. The GUI is resizeable.                                                                  *
 **************************************************************************************************/

#include "MarkovMainWindow.h"
#include "COM_util.h"
#include "StringChain.h"
#include "resource.h"
#include <string>
#include <tchar.h>
#include <locale>
#include <codecvt>
#include <Windows.h>
#define MAX_ORDER 20
#define MIN_ORDER 1
#define MAX_GEN 9999
#define MIN_GEN 1
#define MAX_INPUT_SIZE 5

/**************************************************************************************************
 * Message handler for the main window. A WM_CREATE message will prompt the program to create the *
 * GUI, while WM_CLOSE and WM_DESTROY messages close the application. Clicking on any buttons     *
 * will put a WM_COMMAND message on the queue; the program determines which button fired the      *
 * message, and calls an approprate OnClick function. WM_PAINT and WM_SIZE are encountered        *
 * whenever the GUI background needs repainting or when the window has been resized,              *
 * respectively. All other types of messages are forwarded to the default Windows message         *
 * handler, DefWindowProc.                                                                        *
 *   Inputs:                                                                                      *
 *      uMsg: An unsigned integer indicating the type of message.                                 *
 *      wParam: A UINT_PTR pointing to additional info about the message (the exact contents      *
 *              depend on the message type; see Windows documentation).                           *
 *      lParam: A LONG_PTR pointing to additional info about the message (the exact contents      *
 *              depend on the message type; see Windows documentation).                           *
 *   return value: 0 if the function succeeds, -1 for unexpected outcomes, or whatever            *
 *   DefWindowProc returns for the miscellaneous messages that it handles.                        *
 **************************************************************************************************/
LRESULT MarkovMainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		GenerateGUI();
		InitializeGeneratorFileList(); // puts a default item in the listbox
		Resize();
		return 0;
	}
	case WM_CLOSE:
		DeleteObject(boldFont);
		DestroyWindow(m_hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		OnPaint();
		return 0;
	case WM_SIZE:
		Resize();
		return 0;
	case WM_COMMAND:
	{
		// Determine which control the user clicked and call its corresponding OnClick method:
		HWND controlHandle = (HWND)lParam;
		if (controlHandle == addAFileButton) return AddAFileButtonOnClick();
		else if (controlHandle == removeFileButton)	return RemoveFileButtonOnClick();
		else if (controlHandle == listBox) return 0;
		else if (controlHandle == generateButton) return GenerateButtonOnClick();
		else if (controlHandle == advancedButton) return AdvancedButtonOnClick();
		else return -1;
	}
	default:
		return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
	}
}

// Static variable initialization:
MarkovMainWindow::MarkovOptions * MarkovMainWindow::markovOptions = NULL;

/**************************************************************************************************
 * Repositions all elements in the main window. This function is called whenever the window has   *
 * been resized, requiring that all its child elements be moved around to make it look pretty     *
 * again. The entire window is also "invalidated" to force a repaint.                             *
 *   return value: none.                                                                          *
 **************************************************************************************************/
void MarkovMainWindow::Resize()
{
	RECT clientArea;
	int clientHeight;
	int clientWidth;
	if (GetClientRect(m_hwnd, &clientArea))
	{
		// Cached constants:
		clientHeight = clientArea.bottom;
		clientWidth = clientArea.right;
		box_height = clientHeight - 4 * standard_margin - static_text_height - button_height;
		editControl_width = clientWidth - 3 * standard_margin - listBox_width;

		// Set sizes and positions of Dialog elements:
		SetWindowPos(addAFileButton, HWND_TOP, 
			standard_margin, //-------------------------------------------------------------- x
			3 * standard_margin + static_text_height + box_height, //------------------------ y
			add_button_width, //------------------------------------------------------------- width
			button_height, //---------------------------------------------------------------- height
			0);
		SetWindowPos(removeFileButton, HWND_TOP, 
			standard_margin + listBox_width - remove_button_width, //------------------------ x
			3 * standard_margin + static_text_height + box_height, //------------------------ y
			remove_button_width, //---------------------------------------------------------- width
			button_height, //---------------------------------------------------------------- height
			0);
		SetWindowPos(generateButton, HWND_TOP, 
			2*standard_margin+listBox_width+editControl_width/2 - generate_button_width/2, // x 
			3 * standard_margin + static_text_height + box_height, //------------------------ y
			generate_button_width, //-------------------------------------------------------- width
			button_height, //---------------------------------------------------------------- height
			0);
		SetWindowPos(advancedButton, HWND_TOP, 
			clientWidth - standard_margin - advanced_button_width, //------------------------ x
			3 * standard_margin + static_text_height + box_height, //------------------------ y
			advanced_button_width, //-------------------------------------------------------- width
			button_height, //---------------------------------------------------------------- height
			0);
		SetWindowPos(staticLabel, HWND_TOP, 
			standard_margin, //-------------------------------------------------------------- x
			standard_margin, //-------------------------------------------------------------- y
			listBox_width, //---------------------------------------------------------------- width
			static_text_height, //----------------------------------------------------------- height
			0);
		SetWindowPos(listBox, HWND_TOP, 
			standard_margin, //-------------------------------------------------------------- x
			2 * standard_margin + static_text_height, //------------------------------------- y
			listBox_width, //---------------------------------------------------------------- width
			box_height,//-------------------------------------------------------------------- height
			0);
		SetWindowPos(editControl, HWND_TOP, 
			2 * standard_margin + listBox_width, //------------------------------------------ x
			2 * standard_margin + static_text_height,//-------------------------------------- y
			editControl_width, //------------------------------------------------------------ width
			box_height, //------------------------------------------------------------------- height
			0);
	}
	InvalidateRect(m_hwnd, NULL, FALSE); // forces a repaint of the entire client area of the window
}

/**************************************************************************************************
 * Fills the main window with a solid color. This function is called whenever the main window is  *
 * being redrawn (child elements automatically draw themselves on top of this background).        *
 *   return value: none                                                                           *
 **************************************************************************************************/
void MarkovMainWindow::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_hwnd, &ps);
	FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
	EndPaint(m_hwnd, &ps);
}

/**************************************************************************************************
 * Removes the currently selected file from the listbox whenever the Remove File Button is        *
 * clicked. If no files are selected, nothing is removed.                                         *
 *   return value: always 0.                                                                      *
 **************************************************************************************************/
int MarkovMainWindow::RemoveFileButtonOnClick()
{
	int selectedFileIndex = (int)SendMessage(listBox, LB_GETCURSEL, 0, 0);
	if (selectedFileIndex == LB_ERR); // if nothing is selected, don't do anything
	else
	{
		SendMessage(listBox, LB_DELETESTRING, selectedFileIndex, NULL);
		fileList.erase(fileList.begin() + selectedFileIndex);
		numFiles--;
		// set the selection to the first item, for easy mass deleting
		SendMessage(listBox, LB_SETCURSEL, 0, NULL); 
	}
	return 0;
}

/**************************************************************************************************
 * Displays a File Open Dialog whenever the Add A File button is clicked. The selected file is    *
 * added to the list of files to generate from, and its name is displayed in the GUI. If the user *
 * attempts to add more than 10 files, an error message is displayed (there's no real technical   *
 * reason for this; I just wanted to play around with MessageBoxes).                              *
 *   return value: 0 if the operation is successful, or -1 if one of the necessary operations     *
 *   fails.                                                                                       *
 **************************************************************************************************/
int MarkovMainWindow::AddAFileButtonOnClick()
{
	if (numFiles > 9)
	{
		MessageBox(m_hwnd, L"You cannot generate from more than 10 files.", 
			       NULL, MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	else
	{
		//Create the FileOpen dialog object:
		IFileOpenDialog *pFileOpen;
		HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog,
			                          reinterpret_cast<void**>(&pFileOpen));
		if (SUCCEEDED(hr))
		{
			//Show the dialog box:
			hr = pFileOpen->Show(NULL);
			if (SUCCEEDED(hr))
			{
				//Grab the file name and path from the dialog box:
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR fileName;
					hr = pItem->GetDisplayName(SIGDN_NORMALDISPLAY, &fileName);
					if (SUCCEEDED(hr))
					{
						PWSTR filePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
						if (SUCCEEDED(hr))
						{
							// Add the item to fileList, display it on the ListBox, and select it:
							fileList.push_back(MarkovMainWindow::FileRoster{fileName,filePath,-1});
							DWORD newIndex = SendMessage(listBox, LB_ADDSTRING, 0, 
								                         (LPARAM)fileList.back().name.c_str());
							fileList.back().index = newIndex;
							SendMessage(listBox, LB_SETCURSEL, newIndex, NULL);
							numFiles++;
							return 0;
						}
					}
				}
			}
		}
		SafeRelease(&pFileOpen);
	}
	return -1;
}

/**************************************************************************************************
 * Generates gibberish from the selected files whenever the Generate Button is clicked. All the   *
 * files are read one at a time and then the giberish is generated from the entire Markov chain.  *
 * If a particular file cannot be opened, a MessageBox is displayed, asking the user whether to   *
 * ignore the file, try opening it again, or abort the execution. The generated gibberish is then *
 * displayed in the Edit Control on the right-hand side of the GUI. Note: all input is assumed to *
 * be UTF-8 encoded.                                                                              *
 *   return value: always 0.                                                                      *
 **************************************************************************************************/
int MarkovMainWindow::GenerateButtonOnClick()
{
	if (numFiles < 1)
	{
		MessageBox(m_hwnd, L"You must specify at least 1 file.", NULL, MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	else
	{
		std::wstring filename;
		std::wstring output;
		StringChain stringChain(mOptions.order);

		// Read in all selected files. 
		for (int i = 0; i < numFiles; ++i) 
		{
			filename = fileList.at(i).fullPath;
			std::wifstream fid(filename.c_str());

			// If a file cannot be opened, ask the user what to do
			while (!fid.is_open())
			{
				int decision = MessageBox(m_hwnd, (L"Error! failed to open the file " + filename
					                      + L".\r\n").c_str(), L"File Error", 
					                      MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION);
				switch (decision)
				{
				case IDABORT:
					stringChain.deleteMap();
					return 0;
				case IDRETRY:
					fid.open(filename.c_str());
					continue;
				case IDIGNORE:
					break;
				}
				break;
			}

			// If the file was successfully opened, process it
			if(fid.is_open())
			{
				fid.imbue(std::locale(fid.getloc(), new std::codecvt_utf8_utf16<wchar_t>));
				stringChain.AddItems(fid, mOptions.tokenType);
				fid.close();
			}
		}

		// Generate gibberish
		output += stringChain.generate(mOptions.numGen, mOptions.order, mOptions.tokenType, rand);
		stringChain.deleteMap();

		// set the edit control's text to display the gibberish
		SetWindowText(editControl, output.c_str());
		return 0;
	}
}

/**************************************************************************************************
 * Displays the Advanced Options dialog box whenever the Advanced Button is clicked. The dialog   *
 * box features controls for setting the order of the Markov chain, how many words or characters  *
 * should be generated when the Generate Button is clicked, and whether to use characters or      *
 * words for the Markov Chain. Unlike the main window, the Advanced Options dialog box is         *
 * generated using a .rc resource file.                                                           *
 *   return value: always 0                                                                       *
 **************************************************************************************************/
int MarkovMainWindow :: AdvancedButtonOnClick()
{
	INT_PTR result = DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DIALOG1), m_hwnd,
		reinterpret_cast<DLGPROC>(settingsMessageHandler), (LPARAM)&mOptions);
	return 0;
}

/**************************************************************************************************
 * Adds the example file to the file list and makes it the selected item. This function is called *
 * only once, immediately after the main window GUI is generated.                                 *
 *   return value: none                                                                           *
 **************************************************************************************************/
void MarkovMainWindow::InitializeGeneratorFileList()
{
	fileList.push_back(MarkovMainWindow::FileRoster{ L"simple.txt", 
		L"C:\\Root\\C++\\manual_workspace\\redoneHwAssignments\\6_markov\\simple.txt", -1 });
	DWORD newIndex = SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)fileList.at(0).name.c_str());
	fileList.at(0).index = newIndex;
	numFiles++;
	SendMessage(listBox, LB_SETCURSEL, newIndex, NULL);
}

/**************************************************************************************************
 * Instantiates all child elements of the GUI window. This function is called only once, in       *
 * response to a WM_CREATE message. A WM_RESIZE message will be received soon after WM_CREATE, so *
 * there is no need to specify the positions or sizes the elements at this time.                  *
 *   return value: none                                                                           *
 **************************************************************************************************/
void MarkovMainWindow::GenerateGUI()
{
	// Create controls for main window:
	staticLabel = CreateWindowEx(
		0L, 
		TEXT("static"), 
		TEXT("I want to generate gibberish from these files:"), 
		WS_CHILD | WS_VISIBLE, 
		0, 0, 0, 0, 
		m_hwnd, 
		(HMENU)staticLabelID, 
		NULL, 
		NULL);
	listBox = CreateWindowEx(
		0L, 
		TEXT("listbox"), 
		NULL, WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_BORDER, 
		0, 0, 0, 0, 
		m_hwnd, 
		(HMENU)listBoxID, 
		NULL, 
		NULL);
	addAFileButton = CreateWindowEx(
		0L, 
		TEXT("button"), 
		TEXT("Add a File"), 
		WS_CHILD | WS_VISIBLE, 
		0, 0, 0, 0, 
		m_hwnd, 
		(HMENU)addAFileButtonID, 
		NULL, 
		NULL);
	removeFileButton = CreateWindowEx(
		0L,
		TEXT("button"),
		TEXT("Remove File"), 
		WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0, 
		m_hwnd,
		(HMENU)removeFileButtonID, 
		NULL, 
		NULL);
	editControl = CreateWindowEx(
		0L, 
		TEXT("edit"), 
		NULL,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_BORDER,
		0, 0, 0, 0,
		m_hwnd,
		(HMENU)editControlID, 
		(HINSTANCE)GetWindowLong(m_hwnd, GWL_HINSTANCE),
		NULL);
	generateButton = CreateWindowEx(
		0L, 
		TEXT("button"), 
		TEXT("Generate!"), 
		WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0, 
		m_hwnd, 
		(HMENU)generateButtonID,
		NULL,
		NULL);
	advancedButton = CreateWindowEx(
		0L, 
		TEXT("button"),
		TEXT("Advanced"), 
		WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0, 
		m_hwnd, (HMENU)advancedButtonID,
		NULL,
		NULL);

	// Make the text in the "Generate" button bold, so it stands out:
	boldFont = CreateFont(0, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	SendMessage(generateButton, WM_SETFONT, (WPARAM)boldFont, (LPARAM)MAKELONG(FALSE, 0));
}

/**************************************************************************************************
 * Checks whether a wide-character string contains only numerical characters (0-9).               *
 *   Inputs:                                                                                      *
 *      input: a long-character string whose length (including the null terminator) is            *
 *             MAX_INPUT_SIZE.                                                                    *
 *   return value: TRUE if *all* characters are numerical digits, FALSE otherwise.                *
 **************************************************************************************************/
BOOL MarkovMainWindow::isNumerical(wchar_t * input)
{
	int i;
	for(i=0; i<MAX_INPUT_SIZE; i++)
	{
		wchar_t digitToTest = input[i];
		if (!isdigit(digitToTest)) return FALSE;
	}
	return TRUE;
}

// Sets the name of the Window Class.
PCWSTR MarkovMainWindow::ClassName() const { return L"Markov Generator Main Window"; }

/**************************************************************************************************
 * Message Handler for the Advanced Options dialog box. A WM_INITDIALOG message is sent to the    *
 * dialog box upon instantiation and will prompt the program to create the GUI, while WM_CLOSE    *
 * will close the dialog box. Clicking on either the OK or CANCEL buttons will put a WM_COMMAND   *
 * message on the queue; the program determines which button fired the message, and either saves  *
 * the displayed values to the MarkovOptions structure or discards them before putting a WM_CLOSE *
 * message on the queue.                                                                          *
 *   Inputs:                                                                                      *
 *      hwnd: A handle to the dialog box window.                                                  *
 *      message: An unsigned integer indicating the type of message.                              *
 *      wParam: A UINT_PTR pointing to additional info about the message (the exact contents      *
 *              depend on the message type).                                                      *
 *      lParam: A LONG_PTR pointing to additional info about the message (the exact contents      *
 *              depend on the message type).                                                      *
 *   return value: TRUE if the message was processed, FALSE if it was not. A FALSE return value   *
 *                 prompts Windows' dialog manager to perform a default operation.                *
 **************************************************************************************************/
BOOL MarkovMainWindow::settingsMessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND controlHandle;
	HWND markovOrderSpinControl = GetDlgItem(hwnd, IDC_SPIN1);

	switch (message)
	{
	case WM_INITDIALOG:
		GenerateAdvancedOptionsDialog(hwnd, lParam);
		return TRUE;

	case WM_COMMAND:
		// Determine whether the user clicked "OK" or "CANCEL". Store new values if "OK" was clicked
		controlHandle = (HWND)lParam;
		switch (LOWORD(wParam))
		{
		case IDOK: 
			ConfirmSettings(hwnd);
		case IDCANCEL: 
			EndDialog(hwnd, NULL);
		}
		return TRUE;

	case WM_CLOSE:
		EndDialog(hwnd, NULL);
		return TRUE;

	default:
		return FALSE;
	}
}

/**************************************************************************************************
 * Generates the GUI for the Advanced Options dialog box. The lParam parameter contains a pointer *
 * to a structure containing the current settings so that the dialog box can display them. The    *
 * dialog box is positioned in the center of the main window.                                     *
 *   Inputs:                                                                                      *
 *      hwnd: A handle to the dialog box window.                                                  *
 *      lParam: A LONG_PTR pointing to a MarkovOptions structure, containing all the current      *
 *              settings values.                                                                  *
 *   return value: none                                                                           *
 **************************************************************************************************/
void MarkovMainWindow::GenerateAdvancedOptionsDialog(HWND hwnd, LPARAM lParam)
{
	int newY;
	int newX;

	// Acquire the current markov options:
	markovOptions = (MarkovOptions *) lParam;

	// Set values on the spin controls:
	SendDlgItemMessage(hwnd, IDC_SPIN1, UDM_SETRANGE, 0, MAKELPARAM(MAX_ORDER, MIN_ORDER));
	SendDlgItemMessage(hwnd, IDC_SPIN1, UDM_SETPOS, 0, markovOptions->order);
	SendDlgItemMessage(hwnd, IDC_SPIN2, UDM_SETRANGE, 0, MAKELPARAM(MAX_GEN, MIN_GEN));
	SendDlgItemMessage(hwnd, IDC_SPIN2, UDM_SETPOS, 0, markovOptions->numGen);

	// Select the appropriate radio button:
	if (markovOptions->tokenType == L"words")
	{
		SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, BST_CHECKED, NULL);
		SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	else
	{
		SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, BST_UNCHECKED, NULL);
		SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, BST_CHECKED, NULL);
	}

	// position the dialogbox:
	RECT parentRect;
	GetWindowRect(GetParent(hwnd), &parentRect);
	RECT thisRect;
	GetWindowRect(hwnd, &thisRect);
	newX = (parentRect.right + parentRect.left) / 2 - (thisRect.right - thisRect.left) / 2;
	newY = (parentRect.bottom + parentRect.top) / 2 - (thisRect.bottom - thisRect.top) / 2;
	SetWindowPos(hwnd, HWND_TOP, newX, newY, 0, 0, SWP_NOSIZE);
}

/**************************************************************************************************
 * Saves the settings in the currently displayed Advanced Options dialog box. The settings are    *
 * stored by overwritting the values in the mOptions structure, which is later referenced when    *
 * the user clicks the Generate Button. This function is called when the user clicks OK on the    *
 * Advanced Options dialog box.                                                                   *
 *   Inputs:                                                                                      *
 *      hwnd: A handle to the dialog box window.                                                  *
 *   return value: none                                                                           *
 **************************************************************************************************/
void MarkovMainWindow::ConfirmSettings(HWND hwnd)
{
	UINT wordsChecked;
	wchar_t buff[MAX_INPUT_SIZE];

	// Guard against nonsenscial user input before changing values
	GetWindowText(GetDlgItem(hwnd, IDC_EDIT1), buff, MAX_INPUT_SIZE);
	if (isNumerical(buff))
	{
		int input = _ttoi(buff);
		if (input > MAX_ORDER) input = MAX_ORDER;
		if (input < MIN_ORDER) input = MIN_ORDER;
		markovOptions->order = _ttoi(buff);
	}

	GetWindowText(GetDlgItem(hwnd, IDC_EDIT2), buff, MAX_INPUT_SIZE);
	if (isNumerical(buff))
	{
		int input = _ttoi(buff);
		if (input > MAX_GEN) input = MAX_GEN;
		if (input < MIN_GEN) input = MIN_GEN;
		markovOptions->numGen = _ttoi(buff);
	}

	wordsChecked = IsDlgButtonChecked(hwnd, IDC_RADIO1);
	if (wordsChecked == BST_CHECKED)
	{
		markovOptions->tokenType = L"words";
	}
	else
	{
		markovOptions->tokenType = L"characters";
	}
}