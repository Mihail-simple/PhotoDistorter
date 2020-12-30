#pragma once

#include "headers/distortion.h"
#include "headers/qtutils.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class DistortionWidget : public QDialog
{
    Q_OBJECT
public:
    explicit DistortionWidget(QWidget *parent = nullptr);
    virtual ~DistortionWidget();
    DistortionLoadResult getDistortion();
    virtual void setDefault(const Distortion& d) = 0;

public slots:
    void clickLoad();
    void clickSave();
    void clickOK();
    void clickCancel();

protected:
    QPushButton* buttonLoad;
    QPushButton* buttonSave;
    QPushButton* buttonOK;
    QPushButton* buttonCancel;
    bool hasDistortion;

    virtual bool checkDistortion() = 0;
    virtual Distortion makeDistortion() = 0;
    virtual std::vector<Distortion> load() = 0;
    virtual QString userPath() = 0;
    void makeLabel(QLabel*& label, QLineEdit*& line, QString text);
    bool checkBoundaries(QString textLeft, QString textRight, QString arg);
};
