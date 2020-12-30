#include "headers/distortionxywidget.h"
#include "headers/savedistortionwidget.h"
#include "headers/templatedistortionwidget.h"
#include "headers/templatedistortionloader.h"
#include <QHBoxLayout>

DistortionXYWidget::DistortionXYWidget(QWidget *parent) : DistortionWidget(parent)
{
    makeLabel(labelFunc, lineFunc, "f(x,y) = ");
    makeLabel(labelMinX, lineMinX, "x from ");
    makeLabel(labelMaxX, lineMaxX, "x to ");
    makeLabel(labelMinY, lineMinY, "y from ");
    makeLabel(labelMaxY, lineMaxY, "y to ");
}

DistortionXYWidget::~DistortionXYWidget(){
    delete labelFunc;
    delete labelMinX;
    delete labelMaxX;
    delete labelMinY;
    delete labelMaxY;
    delete lineFunc;
    delete lineMinX;
    delete lineMaxX;
    delete lineMinY;
    delete lineMaxY;
}

void DistortionXYWidget::setDefault(const Distortion& d){
    lineFunc->setText(d.getExprString().c_str());
    lineMinX->setText(d.getLeftXString().c_str());
    lineMaxX->setText(d.getRightXString().c_str());
    lineMinY->setText(d.getLeftYString().c_str());
    lineMaxY->setText(d.getRightYString().c_str());
}

bool DistortionXYWidget::checkDistortion(){
    MathParser::Expression exprFunc;
    if(!exprFunc.compile(lineFunc->text().toStdString(), {"x","y"})){
        return QtUtils::showErrorMessage("Invalid function");
    }

    return checkBoundaries(lineMinX->text(), lineMaxX->text(), "x") &&
           checkBoundaries(lineMinY->text(), lineMaxY->text(), "y");
}

Distortion DistortionXYWidget::makeDistortion(){
    return Distortion(lineMinX->text().toStdString(),
                      lineMaxX->text().toStdString(),
                      lineMinY->text().toStdString(),
                      lineMaxY->text().toStdString(),
                      lineFunc->text().toStdString(),
                      {"x","y"});
}

std::vector<Distortion> DistortionXYWidget::load(){
    return TemplateDistortionLoader().loadXY();
}

QString DistortionXYWidget::userPath(){
    return getPathUserXY();
}
