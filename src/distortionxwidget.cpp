#include "headers/distortionxwidget.h"
#include "headers/templatedistortionwidget.h"
#include "headers/templatedistortionloader.h"
#include "headers/savedistortionwidget.h"
#include <QHBoxLayout>

DistortionXWidget::DistortionXWidget(QWidget *parent) : DistortionWidget(parent){
    makeLabel(labelFunc, lineFunc, "f(x) = ");
    makeLabel(labelMin, lineMin, "x from ");
    makeLabel(labelMax, lineMax, "x to ");
}

DistortionXWidget::~DistortionXWidget(){
    delete labelFunc;
    delete labelMin;
    delete labelMax;
    delete lineFunc;
    delete lineMin;
    delete lineMax;
}

void DistortionXWidget::setDefault(const Distortion& d){
    lineFunc->setText(d.getExprString().c_str());
    lineMin->setText(d.getLeftXString().c_str());
    lineMax->setText(d.getRightXString().c_str());
}

bool DistortionXWidget::checkDistortion(){
    MathParser::Expression exprFunc;
    if(!exprFunc.compile(lineFunc->text().toStdString(), {"x"})){
        return QtUtils::showErrorMessage("Invalid function");
    }

    return checkBoundaries(lineMin->text(), lineMax->text(), "x");
}

Distortion DistortionXWidget::makeDistortion(){
    return Distortion(lineMin->text().toStdString(),
                      lineMax->text().toStdString(),
                      "",
                      "",
                      lineFunc->text().toStdString(),
                      {"x"});
}

std::vector<Distortion> DistortionXWidget::load(){
    return TemplateDistortionLoader().loadX();
}

QString DistortionXWidget::userPath(){
    return getPathUserX();
}
