#include "dj_calculator_UI.h"
#include <QtWidgets/QApplication>

#define DATAFILE_PATH "PatternData.xlsx"
#define SHEET_NAME "EN Sort by Song name"

int main(int argc, char *argv[])
{
    loadSongs(DATAFILE_PATH, SHEET_NAME);
    QApplication a(argc, argv);
    PowerApp w;
    w.show();
    return a.exec();
}
