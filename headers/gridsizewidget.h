#pragma once

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>

class GridSizeWidget : public QDialog
{
Q_OBJECT
public:
    GridSizeWidget(int size);
    ~GridSizeWidget();

signals:
    void setSize(int newSize);

public slots:
    void clickOK();
    void clickCancel();

private:
    QLabel* label;
    QSpinBox* spinbox;
    QPushButton* buttonOK;
    QPushButton* buttonCancel;
};
