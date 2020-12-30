#pragma once
#include <QLabel>
#include <QImage>

class ClickableLabel : public QLabel {
    Q_OBJECT
public:
   explicit ClickableLabel(const QString& text="", QWidget* parent=nullptr);
   ~ClickableLabel();
    void setImage(QImage* _image);

signals:
   void clicked();

protected:
   void mousePressEvent(QMouseEvent* event);
   void paintEvent(QPaintEvent *e) override;

private:
   QImage* image;
};
