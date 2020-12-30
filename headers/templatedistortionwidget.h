#pragma once
#include "headers/verticalscrollarea.h"
#include "headers/templatedistortionwidgetitem.h"
#include <vector>
#include <QDialog>

class TemplateDistortionWidget : public QDialog
{
    Q_OBJECT
public:
    TemplateDistortionWidget(
            std::vector<Distortion> distortions,
            QWidget *parent = nullptr);
    int getNum() const;

public slots:
    void clickOK(int _num);

private:
    VerticalScrollArea* scroll;
    int num = -1;
};
