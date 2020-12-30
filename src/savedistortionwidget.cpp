#include "headers/savedistortionwidget.h"
#include "headers/qtutils.h"
#include "headers/templatedistortionloader.h"
#include <QDir>
#include <QVBoxLayout>

SaveDistortionWidget::SaveDistortionWidget(const Distortion& _d, const QString& _path)
    : d(_d), path(_path)
{
    setWindowTitle("Save");
    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);

    labelPath = new QLabel("File name: ", this);
    linePath = new QLineEdit(this);
    linePath->setText(getDefaultName());
    layout->addLayout(QtUtils::makeHBox(labelPath, linePath));

    buttonOK = new QPushButton("OK", this);
    buttonCancel = new QPushButton("Cancel", this);
    layout->addLayout(QtUtils::makeHBox(buttonOK, buttonCancel));

    connect(buttonOK, &QPushButton::clicked, this, &SaveDistortionWidget::clickOK);
    connect(buttonCancel, &QPushButton::clicked, this, &SaveDistortionWidget::clickCancel);
}

SaveDistortionWidget::~SaveDistortionWidget(){
    delete labelPath;
    delete linePath;
    delete buttonOK;
    delete buttonCancel;
}

void SaveDistortionWidget::clickOK(){
    TemplateDistortionLoader().checkUserSaveDir();
    QString filename = linePath->text();
    if(filename.contains('/') || filename.contains('\\')){
        QtUtils::showErrorMessage("Invalid file name");
        return;
    }

    if(QDir(path).entryList().contains(filename)){
        QtUtils::showErrorMessage("A file with this name already exists");
        return;
    }

    if(!d.save(path + "/" + filename)){
        QtUtils::showErrorMessage("Unknown file error");
        return;
    }

    close();
}

void SaveDistortionWidget::clickCancel(){
    close();
}

QString SaveDistortionWidget::getDefaultName() const{
    QDir dir(path);
    int idx = 0;
    QStringList names = dir.entryList();
    while(names.contains("func" + QString::number(idx) + ".txt")){
        idx++;
    }
    return "func" + QString::number(idx);
}
