#pragma once

#include <QGridLayout>
#include <QResizeEvent>
#include <QScrollArea>
#include <QScrollBar>

class VerticalScrollArea : public QScrollArea
{
public:
    VerticalScrollArea(int _rows, int _cols, int _widgetsInWindow, QWidget *parent = nullptr);
    void addWidget(QWidget *w, int row, int col);

    int rowCount() const;

private:
    QWidget *contentWidget;
    QGridLayout *grid;
    int rows;
    int columns;
    int widgetsInWindow;
    void adaptSize();

protected:
    void resizeEvent(QResizeEvent *event);
};
