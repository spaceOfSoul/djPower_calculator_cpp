#include "songSearch.h"

std::vector<Song> songs_frame;

std::string cellType(OpenXLSX::XLValueType type) {
    switch (type) {
    case OpenXLSX::XLValueType::Empty:
        return "Empty";
    case OpenXLSX::XLValueType::Boolean:
        return "Boolean";
    case OpenXLSX::XLValueType::Integer:
        return "Integer";
    case OpenXLSX::XLValueType::Float:
        return "Float";
    case OpenXLSX::XLValueType::String:
        return "String";
    default:
        return "Unknown";
    }
}

void loadSongs(const std::string& filePath, const std::string& sheetName) {
    try {
        OpenXLSX::XLDocument doc;
        doc.open(filePath);
        auto wks = doc.workbook().worksheet(sheetName);

        uint32_t startRow = 6;
        uint32_t maxCols = 19; // 최대 열 수

        for (uint32_t row = startRow; row <= wks.rowCount(); ++row) {
            std::string dlc = wks.cell(row, 2).value().get<std::string>();
            std::string songName;
            if (wks.cell(row, 3).value().type() == OpenXLSX::XLValueType::Float) { // 3:33이 엑셀에서는 시간처럼 되어있음
                songName = "3:33";
            }
            else {
                songName = wks.cell(row, 3).value().get<std::string>();
            }

            for (int buttonType : {4, 5, 6, 8}) {
                Song song;
                song.dlc = dlc;
                song.songName = songName;
                song.buttonType = buttonType;

                for (uint32_t i = 0; i < 4; ++i) {
                    uint32_t col = 4 + (buttonType - 4) * 4 + i;
                    if (col <= maxCols) {
                        auto cell = wks.cell(row, col);
                        if (cell.value().type() == OpenXLSX::XLValueType::Float) {
                            song.difficulty[i] = (int)cell.value().get<float>();
                        }
                        else {
                            song.difficulty[i] = 0;
                        }
                    }
                }

                /*std::cout << "loaded: " << song.dlc << ", " << song.songName << ", buttonType: " << song.buttonType << ", difficulties: ";
                for (int i = 0; i < 4; ++i) {
                    std::cout << song.difficulty[i] << " ";
                }*/
                //std::cout << std::endl;

                songs_frame.push_back(song);
            }
        }

        doc.close();
    }
    catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }
}

std::vector<Song> filter_songs(int button_type, int difficulty, bool is_sc) {
    std::vector<Song> result;

    for (const auto& song : songs_frame) {
        if (song.buttonType != button_type) {
            continue;
        }

        bool found = false;
        if (is_sc) {
            if (song.difficulty[3] == difficulty) { // SC 난이도
                found = true;
            }
        }
        else {
            for (int i = 0; i < 3; ++i) {
                if (song.difficulty[i] == difficulty) {
                    found = true;
                    break;
                }
            }
        }
        if (found) {
            result.push_back(song);
        }
    }
    return result;
}

