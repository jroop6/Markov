#pragma once
#include "BaseWindow.h"
#include "Random.h"
#include <vector>
#include <ShObjIdl.h>    // Needed for COM's openfile dialog

class MarkovMainWindow : public BaseWindow<MarkovMainWindow>
{
	/*****************************************************************
	 * Structures                                                    *
	 *****************************************************************/

	// A Structure for containing the selected files:
	struct FileRoster
	{
		std::wstring name;     // simple name of the file
		std::wstring fullPath; // absolute path of the file
		int index = -1;        // an index to help identify a file to be deleted

		FileRoster(std::wstring newName, std::wstring newDirectoryPath, int newIndex)
		{
			index = newIndex;
			name = newName;
			fullPath = newDirectoryPath;
		}
	};

	// A structure for holding parameters to the Markov Generator
	struct MarkovOptions
	{
		int order = 2;
		int numGen = 100; // number of tokens to generate
		std::wstring tokenType = L"words";
	};

	/*****************************************************************
	 * Constants and variables                                       *
	 *****************************************************************/

	// GUI controls have been manually added to the main window as child windows.
	// Here are their HWND handles:
	HWND addAFileButton;
	HWND removeFileButton;
	HWND editControl;
	HWND listBox;
	HWND staticLabel;
	HWND generateButton;
	HWND advancedButton;
	HFONT boldFont;

	// Unique identifiers for each child window:
	const int addAFileButtonID = 201;
	const int removeFileButtonID = 202;
	const int editControlID = 203;
	const int listBoxID = 204;
	const int staticLabelID = 205;
	const int generateButtonID = 206;
	const int advancedButtonID = 207;

	// Fixed sizes for various elements on the UI:
	const int standard_margin = 15;
	const int static_text_height = 25;
	const int button_height = 25;
	const int add_button_width = 80;
	const int remove_button_width = 100;
	const int generate_button_width = 80;
	const int advanced_button_width = 70;
	const int listBox_width = 300;

	// Variable sizes for various elements on the UI:
	int box_height;
	int editControl_width;

	// The total number of files added to the Markov Generator:
	int numFiles = 0;

	// A structure holding parameters specific to the interior logic of Markov.exe:
	MarkovOptions mOptions;

	// The files are put into a vector, for easy adding/deleting:
	std::vector<FileRoster> fileList;

	// Stores the Markov options found in the "Advanced" menu
	static MarkovOptions * markovOptions;

	// A random number generator to be used throughout the application
	Random rand;

	/*****************************************************************
	 * Private functions related to the main window                  *
	 *****************************************************************/
	
	// Instantiates all child elements of the main window.
	void GenerateGUI();
	// Adds the example file to the file list and makes it the selected item.
	void InitializeGeneratorFileList();
	// Fills the main window with a solid color.
	void OnPaint();
	// Repositions all elements in the main window.
	void Resize();
	// Displays a File Open Dialog whenever the Add A File button is clicked.
	int AddAFileButtonOnClick();
	// Removes the currently selected file from the listbox when the Remove File Button is clicked.
	int RemoveFileButtonOnClick();
	// Generates gibberish from the selected files whenever the Generate Button is clicked.
	int GenerateButtonOnClick();
	// Displays the Advanced Options dialog box whenever the Advanced Button is clicked.
	int AdvancedButtonOnClick();

	/*****************************************************************
	 * Private functions related to the Advanced Options dialog box: *
	 *****************************************************************/

	// Message Handler for the Advanced Options dialog box.
	static BOOL CALLBACK settingsMessageHandler(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
	// Generates the GUI for the Advanced Options dialog box.
	static void GenerateAdvancedOptionsDialog(HWND hwnd, LPARAM lParam);
	// Saves the settings in the currently displayed Advanced Options dialog box.
	static void MarkovMainWindow::ConfirmSettings(HWND hwnd);
	// Checks whether a wide-character string contains only numerical characters (0-9).
	static BOOL isNumerical(wchar_t * input);

public:
	/*****************************************************************
	 * Public functions related to the main window                   *
	 *****************************************************************/

	// Constructor
	MarkovMainWindow() : addAFileButton(NULL), removeFileButton(NULL), editControl(NULL), 
		listBox(NULL), staticLabel(NULL), generateButton(NULL), advancedButton(NULL) {} 
	// Sets the name of the Window Class.
	PCWSTR ClassName() const; 
	// Message handler for the main window.
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam); 
}; 

