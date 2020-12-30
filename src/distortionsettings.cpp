#include "headers/distortionsettings.h"

DistortionSettings::DistortionSettings() : pivot(0,0), interpolation(true)
{
}

DistortionSettings::DistortionSettings(QPointF _pivot, bool _interpolation) :
    pivot(_pivot), interpolation(_interpolation)
{
}

QPointF DistortionSettings::getPivot(){
    return pivot;
}

bool DistortionSettings::hasInterpolation(){
    return interpolation;
}
