#include "mainframe.hpp"
#include "events.hpp"

#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <filesystem>

static std::vector<std::string> splitBySemicolon(std::string& str) noexcept {
    std::vector<std::string> out;

    auto str_cstr = const_cast<char*>(str.data());

    char* pch = strtok(str_cstr,";");
    while (pch != nullptr) {
        out.emplace_back(pch);
        pch = strtok(nullptr, ";");
    }

    return out;
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
