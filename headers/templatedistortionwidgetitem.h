#pragma once

#include "headers/distortion.h"
#include "headers/workerdistortion.h"
#include "headers/clickablelabel.h"
#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPushButton>

class TemplateDistortionWidgetItem : public QWidget
{
    Q_OBJECT
public:
    TemplateDistortionWidgetItem(QImage* baseImage, const Distortion& d, int _num);
    ~TemplateDistortionWidgetItem();
public slots:
    void imageFinished(QImage* image);
    void pressed();

signals:
    void notifyNumber(int num);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    ClickableLabel* buttonImage;
    QLabel* label;
    int num;

    void startWorker(WorkerDistortion* worker);
};
