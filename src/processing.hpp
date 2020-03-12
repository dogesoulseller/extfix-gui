#pragma once

#include <weirdlib_fileops.hpp>

#include <unordered_set>
#include <string>
#include <filesystem>
#include <vector>
#include <utility>
#include <iostream>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

inline void FixFileExtensions(const std::unordered_set<std::string>& files, const std::string& outputPath, bool doMove) {
	using namespace std::string_literals;
	std::vector<std::pair<std::string, std::filesystem::path>> filesToWrite;

	// TODO: Calculate changed file count
	// TODO: Update status bar
	for (const auto& file : files) {
		auto detectedType = wlib::file::DetectFileType(file);
		auto newName = std::filesystem::path(wlib::file::ChangeExtensionToMatchType(file)).filename();

		if (detectedType == wlib::file::FILETYPE_UNKNOWN) {
			return;
		} else {
			filesToWrite.push_back(std::make_pair(file, newName));
		}
	}

	if (doMove) {
		for (size_t i = 0; i < filesToWrite.size(); i++) {
			std::error_code ecMove;
			std::filesystem::rename(filesToWrite[i].first, outputPath / filesToWrite[i].second, ecMove);

			// On error, do a copy + remove
			if (ecMove) {
				std::error_code ecCopy;
				std::error_code ecRemove;

				std::filesystem::copy_file(filesToWrite[i].first, outputPath / filesToWrite[i].second, ecCopy);
				if (ecCopy) {
					if (auto response = wxMessageBox("Failed to copy file "s + filesToWrite[i].first, "Error copying file", wxICON_ERROR | wxOK | wxCANCEL);
					  response == wxCANCEL) {
						break;
					}
					continue;
				}

				std::filesystem::remove(filesToWrite[i].first, ecRemove);
				if (ecRemove) {
					if (auto response = wxMessageBox("Failed to remove file "s + filesToWrite[i].first, "Error deleting file", wxICON_ERROR | wxOK | wxCANCEL);
					  response == wxCANCEL) {
						break;
					}
					continue;
				}
			}
		}
	} else {
		for (size_t i = 0; i < filesToWrite.size(); i++) {
			std::error_code ec;
			std::filesystem::copy_file(filesToWrite[i].first, outputPath / filesToWrite[i].second, ec);

			if (ec) {
				if (auto response = wxMessageBox("Failed to copy file "s + filesToWrite[i].first, "Error copying file", wxICON_ERROR | wxOK | wxCANCEL);
				  response == wxCANCEL) {
					break;
				}
				continue;
			}
		}
	}
}