#include "headers/clickablelabel.h"
#include <QPainter>

ClickableLabel::ClickableLabel(const QString& text, QWidget* parent)
    : QLabel(parent)
{
   setText(text);
   image = nullptr;
}

ClickableLabel::~ClickableLabel() {
    if(image != nullptr) delete image;
}

void ClickableLabel::setImage(QImage* _image){
    image = _image;
    repaint();
}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
   Q_UNUSED(event);
   emit clicked();
}

void ClickableLabel::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    if(image == nullptr) return;

    QPainter painter(this);
    painter.drawImage(QRect(QPoint(0,0), size()), *image, QRect(QPoint(0,0), image->size()));
}
