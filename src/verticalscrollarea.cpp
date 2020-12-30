#include "headers/verticalscrollarea.h"
#include <QPushButton>

VerticalScrollArea::VerticalScrollArea(int _rows, int _cols, int _widgetsInWindow, QWidget *parent)
:QScrollArea(parent), rows(_rows), columns(_cols), widgetsInWindow(_widgetsInWindow)
{
    setWidgetResizable(false);
    contentWidget = new QWidget(this);
    setWidget(contentWidget);
    grid = new QGridLayout(contentWidget);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void VerticalScrollArea::addWidget(QWidget *w, int row, int col){
    grid->addWidget(w, row, col);
    adaptSize();
}

int VerticalScrollArea::rowCount() const{
    return grid->rowCount();
}

void VerticalScrollArea::adaptSize(){
    int hCorrected = viewport()->height() * rows / widgetsInWindow;
    contentWidget->setFixedHeight(hCorrected);
    contentWidget->setFixedWidth(viewport()->width());
}

void VerticalScrollArea::resizeEvent(QResizeEvent *event){
    QScrollArea::resizeEvent(event);
    adaptSize();
}
