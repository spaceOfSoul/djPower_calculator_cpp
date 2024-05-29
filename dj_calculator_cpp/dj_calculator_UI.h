#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QMap>
#include <QString>
#include <QVector>
#include <QStandardItemModel>
#include <QListView>
#include <QRadioButton>
#include "songSearch.h"

class PowerApp : public QWidget {
    Q_OBJECT

public:
    PowerApp(QWidget* parent = nullptr);

private slots:
    void calculate();

private:
    QLineEdit* dj_power1_input;
    QLineEdit* dj_power2_input;
    QMap<QString, QLabel*> left_labels;
    QMap<QString, QLabel*> right_labels;
    QLabel* song_list_label;
    QListView* song_list_view;

    QRadioButton* radio_4B;
    QRadioButton* radio_5B;
    QRadioButton* radio_6B;
    QRadioButton* radio_8B;

    std::vector<Song> recommand_songs;

    void initUI();
    int getSelectedButton() const;
};
