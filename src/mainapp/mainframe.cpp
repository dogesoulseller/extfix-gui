#include "mainframe.hpp"
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <filesystem>
#include <wx/filepicker.h>
#include <wx/dirdlg.h>

static const std::string AboutTextContents = "Extfix-gui, a wxWidgets GUI for the weirdlib extension fixer";

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "extfix-gui", wxDefaultPosition, wxSize(450, 480)) {
    //* Menu bar
	wxMenu* menuFile = new wxMenu;
    menuFile->Append(MainFrame_ID_PickFile, "Add file...\tCtrl-O");
    menuFile->Append(MainFrame_ID_PickDir, "Add directory...\tCtrl-Shift-O");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuEdit = new wxMenu;
    menuEdit->Append(MainFrame_ID_RemoveAllItems, "Remove all items\tCtrl-R");
    menuEdit->Append(MainFrame_ID_RemoveItemsAtSelection, "Remove selected items\tDel");

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuEdit, "&Edit");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("Idle");

    //* Events
    Bind(wxEVT_MENU, [=](wxCommandEvent&){ Close(true); }, wxID_EXIT);
    Bind(wxEVT_MENU, [=](wxCommandEvent&){ wxMessageBox(AboutTextContents, "About extfix-gui", wxOK | wxICON_INFORMATION); }, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnRemoveAllItems, this, MainFrame_ID_RemoveAllItems);
    Bind(wxEVT_MENU, &MainFrame::OnRemoveItemsAtSelection, this, MainFrame_ID_RemoveItemsAtSelection);
    Bind(wxEVT_MENU, &MainFrame::OnPickFile, this, MainFrame_ID_PickFile);
    Bind(wxEVT_MENU, &MainFrame::OnPickDir, this, MainFrame_ID_PickDir);

    Bind(wxEVT_BUTTON, &MainFrame::OnPickFile, this, MainFrame_ID_PickFile);
    Bind(wxEVT_BUTTON, &MainFrame::OnPickDir, this, MainFrame_ID_PickDir);
    Bind(wxEVT_BUTTON, &MainFrame::OnRemoveItemsAtSelection, this, MainFrame_ID_RemoveItemsAtSelection);
    Bind(wxEVT_BUTTON, &MainFrame::OnAddItemsFromPath, this, MainFrame_ID_AddItemsFromPath);

    //* Layout
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    //* Topmost
    wxBoxSizer* hboxTop = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* nameStaticText = new wxStaticText(panel, wxID_ANY, wxT("File/Directory: "));

    inputPath_ctrl = new wxTextCtrl(panel, wxID_ANY);

    wxButton* pickFile_btn = new wxButton(panel, MainFrame_ID_PickFile, "File", wxDefaultPosition, wxSize(50, -1));
    wxButton* pickDir_btn = new wxButton(panel, MainFrame_ID_PickDir, "Dir", wxDefaultPosition, wxSize(50, -1));

    hboxTop->Add(nameStaticText, 0, wxRIGHT, 5);
    hboxTop->Add(inputPath_ctrl, 1, wxRIGHT, 4);
    hboxTop->Add(pickFile_btn);
    hboxTop->Add(pickDir_btn);

    vbox->Add(hboxTop, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    vbox->Add(-1, 10);

    //* List of files found
    wxBoxSizer* hboxList = new wxBoxSizer(wxHORIZONTAL);
    itemListBox = new wxListBox(panel, -1, wxDefaultPosition, wxSize(-1, 300), wxArrayString(), wxLB_ALWAYS_SB | wxLB_MULTIPLE);
    hboxList->Add(itemListBox, 1, wxRIGHT, 10);
    vbox->Add(hboxList, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    //* Buttons controlling list
    wxBoxSizer* hboxListControls = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* hboxDirCheckbox = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* hboxListButtons = new wxBoxSizer(wxHORIZONTAL);

    wxButton* addItemsFromPath_btn = new wxButton(panel, MainFrame_ID_AddItemsFromPath, "Add");
    wxButton* removeItemsSelected_btn = new wxButton(panel, MainFrame_ID_RemoveItemsAtSelection, "Delete");
    processRecursively_chb = new wxCheckBox(panel, -1, "Process dirs recursively");
    hboxDirCheckbox->Add(processRecursively_chb, 0, wxLEFT);
    hboxListButtons->Add(addItemsFromPath_btn, 0, wxALIGN_RIGHT | wxRIGHT, 5);
    hboxListButtons->Add(removeItemsSelected_btn, 0, wxALIGN_RIGHT | wxRIGHT, 5);

    hboxListControls->Add(hboxDirCheckbox, 1, wxALIGN_LEFT | wxTOP | wxBOTTOM | wxRIGHT, 10);
    hboxListControls->Add(hboxListButtons, 0, wxALIGN_RIGHT | wxLEFT | wxTOP | wxBOTTOM | wxRIGHT, 10);

    vbox->Add(hboxListControls, 0, wxALIGN_RIGHT | wxTOP | wxBOTTOM | wxRIGHT, 10);

    panel->SetSizer(vbox);
    Centre();
}

void MainFrame::OnPickFile(wxCommandEvent& event) {
    wxFileDialog picker(this, "Open file", "", "", "All files *|*", wxFD_FILE_MUST_EXIST | wxFD_OPEN | wxFD_MULTIPLE);

    if (picker.ShowModal() == wxID_CANCEL) {
        SetStatusText("Idle");
        return;
    }

    // Format paths properly
    wxArrayString paths;

    picker.GetPaths(paths);
    if (paths.size() == 0) {
        SetStatusText("No selection");
        return;
    }

    std::stringstream newValueStr;

    for (const auto& p : paths) {
        newValueStr << p << ';';
    }

    std::string valString = newValueStr.str();

    // Remove trailing semicolon
    valString.pop_back();

    inputPath_ctrl->SetValue(valString);
}

void MainFrame::OnPickDir(wxCommandEvent& event) {
    wxDirDialog picker(this, "Open directory", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

    if (picker.ShowModal() == wxID_CANCEL) {
        return;
    }

    inputPath_ctrl->SetValue(picker.GetPath());
}

void MainFrame::OnRemoveItemsAtSelection(wxCommandEvent& event) {
    wxArrayInt selections;
    itemListBox->GetSelections(selections);

    for (auto s = selections.rbegin(); s != selections.rend(); ++s) {
        std::string tmp(itemListBox->GetString(*s));
        filesToProcess.erase(tmp);
        itemListBox->Delete(*s);
    }

    std::stringstream statusMsg;
    statusMsg << "Deleted " << selections.size() << " files";

    SetStatusText(statusMsg.str());
}

std::vector<std::string> splitBySemicolon(std::string& str) {
    std::vector<std::string> out;

    auto str_cstr = const_cast<char*>(str.data());

    char* pch = strtok(str_cstr,";");
    while (pch != nullptr) {
        out.emplace_back(pch);
        pch = strtok(nullptr, ";");
    }

    return out;
}

void MainFrame::OnAddItemsFromPath(wxCommandEvent& event) {
    std::string pathSelection(inputPath_ctrl->GetValue());
    if (pathSelection.empty()) {
        SetStatusText("No files or dir in text area");
        return;
    }

    std::vector<std::string> paths;

    if (std::filesystem::is_directory(pathSelection)) { // Process directory
        if (processRecursively_chb->GetValue()) {
            std::filesystem::recursive_directory_iterator iterator(pathSelection);
            for (const auto& entry : iterator) {
                if (entry.is_regular_file()) {
                    paths.push_back(entry.path());
                } else {
                    continue;
                }
            }
        } else {
            std::filesystem::directory_iterator iterator(pathSelection);
            for (const auto& entry : iterator) {
                if (entry.is_regular_file()) {
                    paths.push_back(entry.path());
                } else {
                    continue;
                }
            }
        }
    } else { // Process list of files
        paths = splitBySemicolon(pathSelection);
    }

    size_t addedCount = 0;

    for (const auto& p : paths) {
        if (std::filesystem::is_regular_file(p)) {
            auto [_ignored_, wasAdded] = filesToProcess.insert(p);
            addedCount += wasAdded ? 1 : 0;
        } else {
            SetStatusText("The provided file(s) do not exist");
            continue;
        }
    }

    std::stringstream statusText;
    statusText << "Added " << addedCount << " new files";
    SetStatusText(statusText.str());

    UpdateListBoxContents();
}

void MainFrame::UpdateListBoxContents() {
    auto listBoxContents = itemListBox->GetStrings();
    wxArrayString newItems;

    for (const auto& file : filesToProcess) {
        for (const auto& boxitem : listBoxContents) {
            if (file == boxitem) {
                goto alreadyExists;
            }
        }

        newItems.Add(file);

        alreadyExists:
        continue;
    }

    itemListBox->Append(newItems);
}

void MainFrame::OnRemoveAllItems(wxCommandEvent& event) {
    auto response = wxMessageBox("Are you sure you want to remove all items from the list?", "Confirm", wxYES_NO | wxCANCEL | wxICON_QUESTION, this);

    if (response == wxCANCEL || response == wxNO) {
        SetStatusText("Idle");
        return;
    } else if (response == wxYES) {
        std::stringstream statusMsg;
        statusMsg << "Deleted " << itemListBox->GetCount() << " files";

        filesToProcess.clear();
        itemListBox->Clear();

        SetStatusText(statusMsg.str());
    }
}