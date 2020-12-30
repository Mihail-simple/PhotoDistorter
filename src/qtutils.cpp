#include "headers/qtutils.h"
#include <QMessageBox>

QHBoxLayout* QtUtils::makeHBox(QWidget* widgetLeft, QWidget* widgetRight){
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(widgetLeft);
    layout->addWidget(widgetRight);
    return layout;
}

bool QtUtils::showErrorMessage(QString text){
    QMessageBox msg;
    msg.setWindowTitle("Warning");
    msg.setIcon(QMessageBox::Warning);
    msg.setText(text);
    msg.exec();
    return false;
}
