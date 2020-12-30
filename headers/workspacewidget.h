#pragma once

#include <QImage>
#include <QWidget>
#include <QFrame>
#include <QScrollBar>

class WorkspaceWidget : public QWidget
{
Q_OBJECT
public:
    WorkspaceWidget();
    ~WorkspaceWidget();
    bool eventFilter(QObject *object, QEvent *ev) override;
    void setImage(QImage* _image);
    void setGrid(QImage* _grid);
    void setGridActive(bool active);
    bool isGridOn() const;
    QImage* getImage() const;
    void resetSliders();
    void updateSliderMaximum();

public slots:
    void trySetPivot();

signals:
    void setPivot(QPointF newPivot);

protected:
  void paintEvent(QPaintEvent *e) override;
  void resizeEvent(QResizeEvent *e) override;

private:
    QImage* image = nullptr;
    QImage* grid = nullptr;
    QFrame* imageFrame;
    QScrollBar* sliderH;
    QScrollBar* sliderV;

    double zoomFactor = 1;
    bool dragImage = false;
    QPoint dragImageP;
    QPoint dragImageSliders;
    bool gridOn = false;
    bool pivotModeOn = false;

    void doPainting();
    int imageX();
    int imageY();
    QRect sourceRect();
    QRect targetRect();
    double pixelRatio();
    void zoom(int delta, QPointF mouse);
    QPointF pointOnImage(QPointF mouse);
};
