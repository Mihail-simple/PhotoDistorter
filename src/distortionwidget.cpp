#include "headers/distortionwidget.h"
#include "headers/templatedistortionwidget.h"
#include "headers/templatedistortionloader.h"
#include "headers/savedistortionwidget.h"
#include <QHBoxLayout>

DistortionWidget::DistortionWidget(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Add");
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    buttonLoad = new QPushButton("Load...", this);
    buttonSave = new QPushButton("Save...", this);
    mainLayout->addLayout(QtUtils::makeHBox(buttonSave, buttonLoad));

    buttonOK = new QPushButton("OK", this);
    buttonCancel = new QPushButton("Cancel", this);
    mainLayout->addLayout(QtUtils::makeHBox(buttonOK, buttonCancel));

    connect(buttonLoad, &QPushButton::clicked, this, &DistortionWidget::clickLoad);
    connect(buttonSave, &QPushButton::clicked, this, &DistortionWidget::clickSave);
    connect(buttonOK, &QPushButton::clicked, this, &DistortionWidget::clickOK);
    connect(buttonCancel, &QPushButton::clicked, this, &DistortionWidget::clickCancel);

    hasDistortion = false;
}

DistortionWidget::~DistortionWidget(){
    delete buttonSave;
    delete buttonLoad;
    delete buttonOK;
    delete buttonCancel;
}

void DistortionWidget::makeLabel(QLabel*& label, QLineEdit*& line, QString text){
    label = new QLabel(text, this);
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    line = new QLineEdit(this);
    QVBoxLayout* mainLayout = (QVBoxLayout*)(layout());
    mainLayout->insertLayout(mainLayout->count()-1, QtUtils::makeHBox(label, line));
}

DistortionLoadResult DistortionWidget::getDistortion(){
    if(hasDistortion){
        return {makeDistortion(), hasDistortion};
    }else{
        return {Distortion(), hasDistortion};
    }
}

void DistortionWidget::clickLoad(){
    TemplateDistortionLoader loader;
    std::vector<Distortion> distortions = load();
    TemplateDistortionWidget dialog(distortions);
    dialog.exec();
    if(dialog.getNum() != -1){
        setDefault(distortions[dialog.getNum()]);
    }
}

void DistortionWidget::clickSave(){
    if(!checkDistortion()) return;

    Distortion d = makeDistortion();
    SaveDistortionWidget dialog(d, userPath());
    dialog.exec();
}

void DistortionWidget::clickOK(){
    if(checkDistortion()){
        hasDistortion = true;
        close();
    }
}

void DistortionWidget::clickCancel(){
    close();
}

bool DistortionWidget::checkBoundaries(QString textLeft, QString textRight, QString arg){
    MathParser::Expression exprLeft, exprRight;
    if(!exprLeft.compile(textLeft.toStdString(), {}) ||
       !exprRight.compile(textRight.toStdString(), {})){
        return QtUtils::showErrorMessage("Invalid format for " + arg + " boundaries");
    }
    if(exprLeft.evaluate() >= exprRight.evaluate()){
        return QtUtils::showErrorMessage("Invalid interval for " + arg);
    }
    return true;
}
