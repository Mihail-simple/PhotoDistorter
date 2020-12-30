#include "headers/gridsizewidget.h"
#include "headers/qtutils.h"
#include <QVBoxLayout>

GridSizeWidget::GridSizeWidget(int size)
{
    setWindowTitle("Set size");
    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);

    label = new QLabel("Size: ", this);
    spinbox = new QSpinBox(this);
    spinbox->setValue(size);
    spinbox->setMinimum(2);
    spinbox->setMaximum(256);
    layout->addLayout(QtUtils::makeHBox(label, spinbox));

    buttonOK = new QPushButton("OK", this);
    buttonCancel = new QPushButton("Cancel", this);
    layout->addLayout(QtUtils::makeHBox(buttonOK, buttonCancel));

    connect(buttonOK, &QPushButton::clicked, this, &GridSizeWidget::clickOK);
    connect(buttonCancel, &QPushButton::clicked, this, &GridSizeWidget::clickCancel);
}

GridSizeWidget::~GridSizeWidget(){
    delete label;
    delete spinbox;
    delete buttonOK;
    delete buttonCancel;
}

void GridSizeWidget::clickOK(){
    emit setSize(spinbox->value());
    close();
}

void GridSizeWidget::clickCancel(){
    close();
}
