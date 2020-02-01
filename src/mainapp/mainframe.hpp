#pragma once

#include <wx/wx.h>
#include <unordered_set>
#include <string>

enum
{
	MainFrame_ID_PickFile,
	MainFrame_ID_PickDir,
	MainFrame_ID_RemoveItemsAtSelection,
	MainFrame_ID_AddItemsFromPath,
	MainFrame_ID_RemoveAllItems

};

class MainFrame : public wxFrame
{
  public:
	MainFrame();

	void OnRemoveAllItems(wxCommandEvent& event);

	void OnPickFile(wxCommandEvent& event);
	void OnPickDir(wxCommandEvent& event);

	void OnRemoveItemsAtSelection(wxCommandEvent& event);
	void OnAddItemsFromPath(wxCommandEvent& event);

	wxTextCtrl* inputPath_ctrl;
	wxListBox* itemListBox;
	wxCheckBox* processRecursively_chb;

	void UpdateListBoxContents();

  private:
	std::unordered_set<std::string> filesToProcess;
};