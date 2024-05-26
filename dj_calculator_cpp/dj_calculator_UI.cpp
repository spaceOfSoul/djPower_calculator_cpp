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

    setWindowTitle("DJMAX DJPower Calculator");
    setGeometry(300, 300, 800, 600);
    
    // initialize results
    for (auto it = difficulty_constant.begin(); it != difficulty_constant.end(); ++it) {
        QString difficulty = it.key();
        double constant = it.value();
        for (int x = 8500; x <= 10000; ++x) {
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

        if (ok1) {
            QVector<double> accuracies1;
            for (const auto& result : results[difficulty]) {
                if (result.second > dj_power1_value) {
                    accuracies1.append(result.first);
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
}
