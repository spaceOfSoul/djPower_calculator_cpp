#include "songSearch.h"

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

std::vector<Song> filter_songs(const std::string& filePath, const std::string& sheetName, int button_type, int difficulty, bool is_sc) {
    std::vector<Song> result;

    try {
        OpenXLSX::XLDocument doc;
        doc.open(filePath);
        auto wks = doc.workbook().worksheet(sheetName);

        uint32_t startRow = 6;
        uint32_t startCol = 0;
        uint32_t endCol = 0;

        // 버튼 타입 구분
        switch (button_type) {
        case 4:
            startCol = 4;
            endCol = 7;
            break;
        case 5:
            startCol = 8;
            endCol = 11;
            break;
        case 6:
            startCol = 12;
            endCol = 15;
            break;
        case 8:
            startCol = 16;
            endCol = 19;
            break;
        default:
            throw std::invalid_argument("Invalid button type");
        }

        if (is_sc) {
            startCol = endCol; // SC인 경우 마지막 열만
        }
        else {
            endCol -= 1; // SC가 아닌 경우 마지막 열빼고
        }

        for (uint32_t row = startRow; row <= wks.rowCount(); ++row) {
            bool found = false;
            for (uint32_t col = startCol; col <= endCol; ++col) {
                auto cell = wks.cell(row, col);
                // std::cout << cell.value() << " " << cellType(cell.value().type()) << "\n";
                if (cell.value().type() == OpenXLSX::XLValueType::Float && abs(cell.value().get<float>() - difficulty) <= 0.1) { // float라 이렇게 검색
                    found = true;
                    break;
                }
            }

            if (found) {
                Song song;
                song.dlc = wks.cell(row, 2).value().getString();
                if (wks.cell(row, 3).value().type() == OpenXLSX::XLValueType::Float) // 3:33이 엑셀에는 시간으로 저장됨 (h:mm)
                    song.songName = "3:33";
                else
                    song.songName = wks.cell(row, 3).value().getString();
                result.push_back(song);
            }
        }

        doc.close();
    }

    catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }

    return result;
}
