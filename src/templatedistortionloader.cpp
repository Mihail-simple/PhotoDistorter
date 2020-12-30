#include "headers/templatedistortionloader.h"
#include <QDir>
#include <iostream>

QString PATH = "templates";
QString PATH_DEFAULT_X = PATH + "/default/x";
QString PATH_DEFAULT_Y = PATH + "/default/y";
QString PATH_DEFAULT_XY = PATH + "/default/xy";
QString PATH_USER = PATH + "/user";
QString PATH_USER_X = PATH + "/user/x";
QString PATH_USER_Y = PATH + "/user/y";
QString PATH_USER_XY = PATH + "/user/xy";

TemplateDistortionLoader::TemplateDistortionLoader()
{
}

std::vector<Distortion> TemplateDistortionLoader::loadX(){
    std::vector<Distortion> def = load(PATH_DEFAULT_X);
    std::vector<Distortion> user = load(PATH_USER_X);
    std::copy(user.begin(), user.end(), std::back_inserter(def));
    return def;
}

std::vector<Distortion> TemplateDistortionLoader::loadY(){
    std::vector<Distortion> def = load(PATH_DEFAULT_Y);
    std::vector<Distortion> user = load(PATH_USER_Y);
    std::copy(user.begin(), user.end(), std::back_inserter(def));
    return def;
}

std::vector<Distortion> TemplateDistortionLoader::loadXY(){
    std::vector<Distortion> def = load(PATH_DEFAULT_XY);
    std::vector<Distortion> user = load(PATH_USER_XY);
    std::copy(user.begin(), user.end(), std::back_inserter(def));
    return def;
}

std::vector<Distortion> TemplateDistortionLoader::load(QString path){
    QDir dir(path);
    std::vector<Distortion> distortions;

    for(QFileInfo file : dir.entryInfoList()){
        if(file.suffix() == "txt"){
            auto res = loadDistortion(file.absoluteFilePath());
            if(res.hasDistortion) distortions.push_back(res.d);
        }
    }
    return distortions;
}

void TemplateDistortionLoader::checkUserSaveDir(){
    if(!QDir(PATH_USER).exists()){
        QDir(PATH).mkdir("user");
    }
    if(!QDir(PATH_USER_X).exists()){
        QDir(PATH_USER).mkdir("x");
    }
    if(!QDir(PATH_USER_Y).exists()){
        QDir(PATH_USER).mkdir("y");
    }
    if(!QDir(PATH_USER_XY).exists()){
        QDir(PATH_USER).mkdir("xy");
    }
}

QString getPathUserX(){
    return PATH_USER_X;
}

QString getPathUserY(){
    return PATH_USER_Y;
}

QString getPathUserXY(){
    return PATH_USER_XY;
}
