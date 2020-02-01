#include "mainframe.hpp"
#include "events.hpp"

#include <cstring>
#include <string>

static const std::string AboutTextContents = "Extfix-gui, a wxWidgets GUI for the weirdlib extension fixer";

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "extfix-gui", wxDefaultPosition, wxSize(450, 480)) {
    makeMenuBar();

    CreateStatusBar();
    SetStatusText("Idle");

    setUpEvents();

    //* Layout
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    //* File/dir selection
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

void MainFrame::setUpEvents() noexcept {
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
}

void MainFrame::makeMenuBar() noexcept {
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
}
