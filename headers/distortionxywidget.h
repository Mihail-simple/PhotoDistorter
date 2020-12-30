#pragma once

#include "headers/distortionwidget.h"
#include "headers/distortion.h"
#include "headers/qtutils.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class DistortionXYWidget : public DistortionWidget
{
    Q_OBJECT
public:
    explicit DistortionXYWidget(QWidget *parent = nullptr);
    ~DistortionXYWidget();
    void setDefault(const Distortion& d) override;

private:
    QLabel* labelFunc;
    QLabel* labelMinX;
    QLabel* labelMaxX;
    QLabel* labelMinY;
    QLabel* labelMaxY;
    QLineEdit* lineFunc;
    QLineEdit* lineMinX;
    QLineEdit* lineMaxX;
    QLineEdit* lineMinY;
    QLineEdit* lineMaxY;

    bool checkDistortion() override;
    Distortion makeDistortion() override;
    std::vector<Distortion> load() override;
    QString userPath() override;
};
