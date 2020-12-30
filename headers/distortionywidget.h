#pragma once

#include "headers/distortionwidget.h"
#include "headers/distortion.h"
#include "headers/qtutils.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class DistortionYWidget : public DistortionWidget
{
    Q_OBJECT
public:
    explicit DistortionYWidget(QWidget *parent = nullptr);
    ~DistortionYWidget();
    void setDefault(const Distortion& d) override;

private:
    QLabel* labelFunc;
    QLabel* labelMin;
    QLabel* labelMax;
    QLineEdit* lineFunc;
    QLineEdit* lineMin;
    QLineEdit* lineMax;

    bool checkDistortion() override;
    Distortion makeDistortion() override;
    std::vector<Distortion> load() override;
    QString userPath() override;
};
