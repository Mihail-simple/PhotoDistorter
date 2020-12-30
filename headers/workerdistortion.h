#pragma once

#include "headers/distortion.h"
#include "headers/distortionsettings.h"
#include <vector>
#include <QObject>
#include <QThread>

class WorkerDistortion : public QObject
{
    Q_OBJECT
public:
    explicit WorkerDistortion(
            const std::vector<Distortion>& _distortions,
            QImage* _image,
            DistortionSettings _settings);

public slots:
    void run();

signals:
    void finished(QImage* image);
private:
   std::vector<Distortion> distortions;
   QImage* image;
   DistortionSettings settings;
};
