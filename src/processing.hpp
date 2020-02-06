#pragma once

#include <weirdlib_fileops.hpp>

#include <unordered_set>
#include <string>
#include <filesystem>
#include <vector>

void FixFileExtensions(const std::unordered_set<std::string>& files, const std::string& outputPath) {
	std::vector<std::string> filesToWrite;

	for (const auto& file : files) {
		auto detectedType = wlib::file::DetectFileType(file);

		auto typeExtension = wlib::file::GetFiletypeExtension(detectedType);

		if (std::filesystem::path(file).extension() == typeExtension) {
			continue;
		}

		// Change extension

	}
}