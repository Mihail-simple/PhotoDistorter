#pragma once

#include "headers/distortion.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class SaveDistortionWidget : public QDialog
{
    Q_OBJECT
public:
    SaveDistortionWidget(const Distortion& _d, const QString& _path);
    ~SaveDistortionWidget();

public slots:
    void clickOK();
    void clickCancel();

private:
    QLabel* labelPath;
    QLineEdit* linePath;
    QPushButton* buttonOK;
    QPushButton* buttonCancel;
    const Distortion& d;
    QString path;

    QString getDefaultName() const;
};
