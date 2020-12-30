#include "headers/templatedistortionwidgetitem.h"
#include <QVBoxLayout>
#include <QThread>

TemplateDistortionWidgetItem::TemplateDistortionWidgetItem(
        QImage* baseImage,
        const Distortion& d,
        int _num) :
        num(_num)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);

    buttonImage = new ClickableLabel("", this);
    buttonImage->setCursor(Qt::PointingHandCursor);
    buttonImage->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(buttonImage);

    label = new QLabel(d.getExtendedExprString().c_str());
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    WorkerDistortion* imageWorker = new WorkerDistortion({d}, baseImage, DistortionSettings());
    connect(imageWorker, &WorkerDistortion::finished, this, &TemplateDistortionWidgetItem::imageFinished);
    startWorker(imageWorker);
}

TemplateDistortionWidgetItem::~TemplateDistortionWidgetItem(){
    delete buttonImage;
    delete label;
}

void TemplateDistortionWidgetItem::resizeEvent(QResizeEvent *event){
//    buttonImage->setPixmap(buttonImage->pixmap()->scaled(buttonImage->width(), buttonImage->height()));
}

void TemplateDistortionWidgetItem::imageFinished(QImage* image){
    buttonImage->setImage(image);
    connect(buttonImage, &ClickableLabel::clicked, this, &TemplateDistortionWidgetItem::pressed);
}

void TemplateDistortionWidgetItem::pressed(){
    emit notifyNumber(num);
}

void TemplateDistortionWidgetItem::startWorker(WorkerDistortion* worker){
    auto *worker_thread = new QThread();
    worker->moveToThread(worker_thread);
    connect(worker_thread, &QThread::started, worker, &WorkerDistortion::run);
    connect(worker, &WorkerDistortion::finished, worker_thread, &QThread::quit);
    connect(worker, &WorkerDistortion::finished, worker, &WorkerDistortion::deleteLater);
    connect(worker_thread, &QThread::finished, worker_thread, &QThread::deleteLater);
    worker_thread->start();
}
