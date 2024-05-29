#include "dj_calculator_UI.h"
#include "constant.h"
#include "calculate.h"
#include <algorithm>

PowerApp::PowerApp(QWidget* parent)
    : QWidget(parent) {
    initUI();
}

void PowerApp::initUI() {
    QVBoxLayout* main_layout = new QVBoxLayout(this);

    // 입력단
    QHBoxLayout* input_layout = new QHBoxLayout();
    main_layout->addLayout(input_layout);

    QLabel* dj_power1_label = new QLabel("Lowest basic Dj Power:");
    input_layout->addWidget(dj_power1_label);

    dj_power1_input = new QLineEdit(this);
    input_layout->addWidget(dj_power1_input);

    QLabel* dj_power2_label = new QLabel("Lowest new Dj Power:");
    input_layout->addWidget(dj_power2_label);

    dj_power2_input = new QLineEdit(this);
    input_layout->addWidget(dj_power2_input);

    QPushButton* calculate_button = new QPushButton("Calculate", this);
    main_layout->addWidget(calculate_button);
    connect(calculate_button, &QPushButton::clicked, this, &PowerApp::calculate);

    // 산출 정확도 표시단
    QHBoxLayout* results_layout = new QHBoxLayout();
    main_layout->addLayout(results_layout);

    QVBoxLayout* left_layout = new QVBoxLayout();
    QVBoxLayout* right_layout = new QVBoxLayout();

    QWidget* left_widget = new QWidget();
    left_widget->setLayout(left_layout);
    QScrollArea* left_scroll = new QScrollArea();
    left_scroll->setWidget(left_widget);
    left_scroll->setWidgetResizable(true);

    QWidget* right_widget = new QWidget();
    right_widget->setLayout(right_layout);
    QScrollArea* right_scroll = new QScrollArea();
    right_scroll->setWidget(right_widget);
    right_scroll->setWidgetResizable(true);

    results_layout->addWidget(left_scroll);
    results_layout->addWidget(right_scroll);

    // 노래 목록단
    song_list_label = new QLabel("Recommand Songs:");
    main_layout->addWidget(song_list_label);

    // 키 라디오 버튼
    {
        QHBoxLayout* radio_layout = new QHBoxLayout();
        main_layout->addLayout(radio_layout);

        radio_4B = new QRadioButton("4B", this);
        radio_layout->addWidget(radio_4B);

        radio_5B = new QRadioButton("5B", this);
        radio_layout->addWidget(radio_5B);

        radio_6B = new QRadioButton("6B", this);
        radio_layout->addWidget(radio_6B);

        radio_8B = new QRadioButton("8B", this);
        radio_layout->addWidget(radio_8B);

        radio_4B->setChecked(true); // 기본값으로 설정
    }

    song_list_view = new QListView(this);
    main_layout->addWidget(song_list_view);

    // 창 설정
    setWindowTitle("DJMAX DJPower Calculator");
    setGeometry(300, 100, 800, 600);

    // initialize results
    for (auto it = difficulty_constant.begin(); it != difficulty_constant.end(); ++it) {
        QString difficulty = it.key();
        double constant = it.value();
        for (int x = 8800; x <= 10000; ++x) {
            double accuracy = x / 100.0;
            double result = calculate_power(accuracy, constant * 2.22 + 2.31);
            results[difficulty].append(qMakePair(accuracy, result));
        }
    }

    // 난이도 목록 라벨 추가
    QStringList ordered_keys = {
        "SC15", "SC14", "SC13", "SC12", "SC11", "SC10",
        "SC9", "SC8", "SC7", "SC6", "SC5", "SC4", "SC3", "SC2", "SC1",
        "15", "14", "13", "12", "11", "10",
        "9", "8", "7", "6", "5", "4", "3", "2", "1"
    };

    int i = 0;
    for (const QString& key : ordered_keys) {
        QLabel* label = new QLabel(key + " : -", this);
        if (i < 15) {
            left_layout->addWidget(label);
            left_labels[key] = label;
        }
        else {
            right_layout->addWidget(label);
            right_labels[key] = label;
        }
        ++i;
    }
}

void PowerApp::calculate() {
    bool ok1, ok2;
    double dj_power1_value = dj_power1_input->text().toDouble(&ok1);
    double dj_power2_value = dj_power2_input->text().toDouble(&ok2);


    for (auto it = difficulty_constant.begin(); it != difficulty_constant.end(); ++it) {
        QString difficulty = it.key();
        QString basic_acc = "-";
        QString new_acc = "-";

        bool is_sc = true;
        int difficulty_num = difficulty.toInt(&is_sc); // 바로 안되면 sc, 바로 되면 sc 아님.
        //std::cout << "convert : " << difficulty_num << "\n";
        is_sc = !is_sc;
        //std::cout << "is_sc : " << is_sc << "\n";


        if (is_sc) {
            difficulty_num = difficulty.mid(2).toInt();
        }
        //std::cout << "re convert : " << difficulty_num << "\n";

        if (ok1) {
            QVector<double> accuracies1;
            for (const auto& result : results[difficulty]) {
                if (result.second > dj_power1_value) {
                    //std::cout << is_sc << " " << difficulty_num << "\n";
                    accuracies1.append(result.first);
                    
                    std::vector<Song> songs = filter_songs(getSelectedButton(), difficulty_num, is_sc); // 임시로 4button
                    for (Song song : songs) {
                        // 최신곡 제외
                        if (song.dlc == "V EXTENSION 5" || song.dlc == "FALCOM" || song.songName == "Kamui" || song.songName == "Re:BIRTH") {
                            continue;
                        }
                        recommand_songs.push_back(song);
                    }
                }
            }
            if (!accuracies1.isEmpty()) {
                basic_acc = QString::number(*std::min_element(accuracies1.begin(), accuracies1.end()));
            }
        }

        if (ok2) {
            QVector<double> accuracies2;
            for (const auto& result : results[difficulty]) {
                if (result.second > dj_power2_value) {
                    accuracies2.append(result.first);

                    std::vector<Song> songs = filter_songs(getSelectedButton(), difficulty_num, is_sc); // 임시로 4button
                    for (Song song : songs) {
                        // 최신곡만
                        if (song.dlc == "V EXTENSION 5" || song.dlc == "FALCOM" || song.songName == "Kamui" || song.songName == "Re:BIRTH")
                            recommand_songs.push_back(song);
                    }
                }
            }
            if (!accuracies2.isEmpty()) {
                new_acc = QString::number(*std::min_element(accuracies2.begin(), accuracies2.end()));
            }
        }

        if (left_labels.contains(difficulty)) {
            left_labels[difficulty]->setText(difficulty + " :\t Basic: " + basic_acc + ",\t New: " + new_acc);
        }
        else if (right_labels.contains(difficulty)) {
            right_labels[difficulty]->setText(difficulty + " :\t Basic: " + basic_acc + ",\t New: " + new_acc);
        }
    }

    // 추천 목록 정리
    std::sort(recommand_songs.begin(), recommand_songs.end(), [](const Song& a, const Song& b) {
        return a.songName < b.songName;
        });
    auto last = std::unique(recommand_songs.begin(), recommand_songs.end(), [](const Song& a, const Song& b) {
        return a.songName == b.songName;
        });
    recommand_songs.erase(last, recommand_songs.end());

    // 리스트 뷰 업뎃
    song_list_view->setUpdatesEnabled(false);  // 업데이트 중지

    QStandardItemModel* model = new QStandardItemModel(song_list_view);

    for (const Song& song : recommand_songs) {
        //std::cout << song.dlc << ", " << song.songName << std::endl;
        QString itemText = QString("%1\t\t\t%2\t")
            .arg(QString::fromStdString(song.songName))
            .arg(QString::fromStdString(song.dlc));
        QStandardItem* item = new QStandardItem(itemText);
        model->appendRow(item);
    }

    song_list_view->setModel(model);

    song_list_view->setUpdatesEnabled(true);  // 업데이트 재개

    recommand_songs.clear();
}

int PowerApp::getSelectedButton() const {
    if (radio_4B->isChecked()) return 4;
    if (radio_5B->isChecked()) return 5;
    if (radio_6B->isChecked()) return 6;
    if (radio_8B->isChecked()) return 8;
    return 4; // default
}