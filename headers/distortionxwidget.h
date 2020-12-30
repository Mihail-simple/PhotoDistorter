#pragma once

#include "headers/distortion.h"
#include "headers/qtutils.h"
#include "headers/distortionwidget.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class DistortionXWidget : public DistortionWidget
{
    Q_OBJECT
public:
    explicit DistortionXWidget(QWidget *parent = nullptr);
    ~DistortionXWidget();
    void setDefault(const Distortion& d) override;

public slots:

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
