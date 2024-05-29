#pragma once
#include <vector>
#include <string>
#include <map>
#include <OpenXLSX.hpp>

struct Song {
    std::string dlc;
    std::string songName;
};

std::string cellType(OpenXLSX::XLValueType type);

std::vector<Song> filter_songs(const std::string& filePath, const std::string& sheetName, int button_type, int difficulty, bool is_sc);