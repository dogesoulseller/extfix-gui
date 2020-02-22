#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <unordered_set>
#include <string>

class MainFrame : public wxFrame
{
  public:
	MainFrame();

	void OnRemoveAllItems(wxCommandEvent& event);

	void OnPickFile(wxCommandEvent& event);
	void OnPickDir(wxCommandEvent& event);
	void OnPickOutDir(wxCommandEvent& event);

	void OnRemoveItemsAtSelection(wxCommandEvent& event);
	void OnAddItemsFromPath(wxCommandEvent& event);

	void OnStartProcessing(wxCommandEvent& event);

	wxTextCtrl* inputPath_ctrl;
	wxTextCtrl* outputPath_ctrl;

	wxListBox* itemListBox;

	wxCheckBox* processRecursively_chb;
	wxCheckBox* moveFiles_chb;

	void UpdateListBoxContents();

  private:
	void makeMenuBar() noexcept;
	void setUpEvents() noexcept;

	std::unordered_set<std::string> filesToProcess;
};