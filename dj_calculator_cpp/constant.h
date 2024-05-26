#pragma once
#include <QMap>
#include <QString>
#include <QVector>
#include <QPair>

QMap<QString, double> difficulty_constant = {
    {"SC15", 44},
    {"SC14", 42},
    {"SC13", 40},
    {"SC12", 38},
    {"SC11", 36},
    {"SC10", 34},
    {"SC9", 32},
    {"SC8", 30},
    {"SC7", 29},
    {"SC6", 28},
    {"SC5", 27},
    {"SC4", 26},
    {"SC3", 25},
    {"SC2", 24},
    {"SC1", 23},
    {"15", 30},
    {"14", 28},
    {"13", 26},
    {"12", 24},
    {"11", 22},
    {"10", 20},
    {"9", 18},
    {"8", 16},
    {"7", 14},
    {"6", 12},
    {"5", 10},
    {"4", 8},
    {"3", 6},
    {"2", 4},
    {"1", 2}
};

QMap<QString, QVector<QPair<double, double>>> results;
