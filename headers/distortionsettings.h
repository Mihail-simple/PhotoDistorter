#pragma once

#include <QPointF>

class DistortionSettings
{
public:
    DistortionSettings();
    DistortionSettings(QPointF _pivot, bool _interpolation);

    QPointF getPivot();
    bool hasInterpolation();

private:
    QPointF pivot;
    bool interpolation;
};
