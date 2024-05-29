#pragma once
#include <vector>
#include <string>
#include <map>
#include <OpenXLSX.hpp>

struct Song {
    std::string dlc;
    std::string songName;
    int buttonType;
    int difficulty[4];
};
extern std::vector<Song> songs_frame; // 전역으로 쓸거임

std::string cellType(OpenXLSX::XLValueType type);

void loadSongs(const std::string& filePath, const std::string& sheetName);
std::vector<Song> filter_songs(int button_type, int difficulty, bool is_sc);