#include "headers/distortionywidget.h"
#include "headers/savedistortionwidget.h"
#include "headers/templatedistortionwidget.h"
#include "headers/templatedistortionloader.h"
#include <QHBoxLayout>

DistortionYWidget::DistortionYWidget(QWidget *parent) : DistortionWidget(parent)
{
    makeLabel(labelFunc, lineFunc, "f(y) = ");
    makeLabel(labelMin, lineMin, "y from ");
    makeLabel(labelMax, lineMax, "y to ");
}

DistortionYWidget::~DistortionYWidget(){
    delete labelFunc;
    delete labelMin;
    delete labelMax;
    delete lineFunc;
    delete lineMin;
    delete lineMax;
}

void DistortionYWidget::setDefault(const Distortion& d){
    lineFunc->setText(d.getExprString().c_str());
    lineMin->setText(d.getLeftYString().c_str());
    lineMax->setText(d.getRightYString().c_str());
}

bool DistortionYWidget::checkDistortion(){
    MathParser::Expression exprFunc;
    if(!exprFunc.compile(lineFunc->text().toStdString(), {"y"})){
        return QtUtils::showErrorMessage("Invalid function");
    }

    return checkBoundaries(lineMin->text(), lineMax->text(), "y");
}

Distortion DistortionYWidget::makeDistortion(){
    return Distortion("",
                      "",
                      lineMin->text().toStdString(),
                      lineMax->text().toStdString(),
                      lineFunc->text().toStdString(),
                      {"y"});
}

std::vector<Distortion> DistortionYWidget::load(){
    return TemplateDistortionLoader().loadY();
}

QString DistortionYWidget::userPath(){
    return getPathUserY();
}
