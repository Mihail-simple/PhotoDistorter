#pragma once

#include "headers/distortion.h"
#include <vector>

class TemplateDistortionLoader
{
public:
    TemplateDistortionLoader();

    std::vector<Distortion> loadX();
    std::vector<Distortion> loadY();
    std::vector<Distortion> loadXY();
    void checkUserSaveDir();

private:
    std::vector<Distortion> load(QString path);
};

QString getPathUserX();
QString getPathUserY();
QString getPathUserXY();
