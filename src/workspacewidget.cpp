#include "headers/workspacewidget.h"
#include <QGridLayout>
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>

WorkspaceWidget::WorkspaceWidget()
{
    setMouseTracking(true);
    installEventFilter(this);
    QGridLayout* layout = new QGridLayout();
    setLayout(layout);

    imageFrame = new QFrame(this);
    imageFrame->setCursor(Qt::SizeAllCursor);

    sliderH = new QScrollBar(Qt::Orientation::Horizontal, this);
    sliderH->setMinimum(0);
    sliderH->setMaximum(0);
    sliderH->setValue(0);

    sliderV = new QScrollBar(Qt::Orientation::Vertical, this);
    sliderV->setMinimum(0);
    sliderV->setMaximum(0);
    sliderV->setValue(0);

    layout->addWidget(imageFrame, 0, 0, 1, 1);
    layout->addWidget(sliderH, 1, 0, 1, 1, Qt::AlignTop);
    layout->addWidget(sliderV, 0, 1, 1, 1, Qt::AlignRight);
    layout->setRowStretch(0, 1);
    layout->setColumnStretch(0, 1);
    layout->setSpacing(5);

    connect(sliderH, &QScrollBar::valueChanged, this,
            static_cast<void (WorkspaceWidget::*)()>(&WorkspaceWidget::repaint));
    connect(sliderV, &QScrollBar::valueChanged, this,
            static_cast<void (WorkspaceWidget::*)()>(&WorkspaceWidget::repaint));
}

WorkspaceWidget::~WorkspaceWidget(){
    delete imageFrame;
    delete sliderH;
    delete sliderV;
    if(image != nullptr) delete image;
}

double WorkspaceWidget::pixelRatio(){
    if(image == nullptr) return 1;

    double widthRatio = image->width() / (double)imageFrame->width();
    double heightRatio = image->height() / (double)imageFrame->height();

    return std::max(widthRatio, heightRatio) / zoomFactor;
}

void WorkspaceWidget::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);

    doPainting();
}

void WorkspaceWidget::resizeEvent(QResizeEvent *e){
    QWidget::resizeEvent(e);
    updateSliderMaximum();
}

void WorkspaceWidget::doPainting(){
    QPainter painter(this);

    painter.fillRect(targetRect(), QBrush(Qt::gray, Qt::CrossPattern));

    if(image == nullptr) return;

    painter.drawImage(targetRect(), *image, sourceRect());
    if(gridOn && grid != nullptr){
        painter.drawImage(targetRect(), *grid, sourceRect());
    }
}

int WorkspaceWidget::imageX(){
    return sliderH->value();
}

int WorkspaceWidget::imageY(){
    return sliderV->value();
}

QRect WorkspaceWidget::sourceRect(){
    if(image == nullptr) return QRect();

    int width = (int)(imageFrame->width() * pixelRatio());
    int x = imageX();
    if(width > image->width()){
        x = -(width - image->width())/2;
    }

    int height = (int)(imageFrame->height() * pixelRatio());
    int y = imageY();
    if(height > image->height()){
        y = -(height - image->height())/2;
    }

    return QRect(x, y, width, height);
}

QRect WorkspaceWidget::targetRect(){
    QRect rect = imageFrame->frameRect();
    rect.moveTo(imageFrame->pos());
    return rect;
}

void WorkspaceWidget::setImage(QImage* _image){
    if(image != nullptr) delete image;
    image = _image;
    repaint();
}

void WorkspaceWidget::setGrid(QImage* _grid){
    if(grid != nullptr) delete grid;
    grid = _grid;
    repaint();
}

void WorkspaceWidget::setGridActive(bool active){
    gridOn = active;
    repaint();
}

bool WorkspaceWidget::isGridOn() const{
    return gridOn;
}

QImage* WorkspaceWidget::getImage() const{
    return image;
}

void WorkspaceWidget::resetSliders(){
    sliderH->setValue(0);
    sliderV->setValue(0);
    updateSliderMaximum();
}

void WorkspaceWidget::updateSliderMaximum(){
    if(image == nullptr){
        sliderH->setMaximum(0);
        sliderV->setMaximum(0);
        return;
    }

    if(sourceRect().width() > image->width()){
        sliderH->setMaximum(0);
    }else{
        sliderH->setMaximum(image->width() - pixelRatio() * imageFrame->width());
    }

    if(sourceRect().height() > image->height()){
        sliderV->setMaximum(0);
    }else{
        sliderV->setMaximum(image->height() - pixelRatio() * imageFrame->height());
    }
}

void WorkspaceWidget::zoom(int delta, QPointF mouse){
    mouse -= targetRect().topLeft();
    mouse.setX(mouse.x() / targetRect().width());
    mouse.setY(mouse.y() / targetRect().height());

    QRect oldRect = sourceRect();
    if(delta > 0){
        zoomFactor = std::min(16.0, zoomFactor * 1.1);
    }else{
        zoomFactor = std::max(0.5, zoomFactor / 1.1);
    }
    if(std::abs(zoomFactor - 1.0) < 0.05) zoomFactor = 1;

    QRect newRect = sourceRect();
    sliderH->setValue(sliderH->value() + mouse.x() * (oldRect.width() - newRect.width()));
    sliderV->setValue(sliderV->value() + mouse.y() * (oldRect.height() - newRect.height()));

    updateSliderMaximum();
    repaint();
}

bool WorkspaceWidget::eventFilter(QObject *object, QEvent *ev)
{
    Q_UNUSED(object);
    if(ev->type() == QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent = (QMouseEvent*)ev;

        if(targetRect().contains(mouseEvent->pos())){
            dragImage = true;
            dragImageP = mouseEvent->pos();
            dragImageSliders = QPoint(sliderH->value(), sliderV->value());
            if(pivotModeOn){
                pivotModeOn = false;
                imageFrame->setCursor(Qt::SizeAllCursor);
                if(mouseEvent->button() == Qt::LeftButton){
                    emit setPivot(pointOnImage(mouseEvent->localPos()));
                }
            }
        }
    }

    if(ev->type() == QEvent::MouseButtonRelease){
        dragImage = false;
    }

    if(ev->type() == QEvent::MouseMove){
        QMouseEvent* mouseEvent = (QMouseEvent*)ev;
        if (dragImage){
            QPoint move = (dragImageP - mouseEvent->pos()) * pixelRatio();
            sliderH->setValue(dragImageSliders.x() + move.x());
            sliderV->setValue(dragImageSliders.y() + move.y());
            repaint();
        }
    }

    if(ev->type() == QEvent::Wheel){
        QWheelEvent* wheelEvent = (QWheelEvent*)ev;

        if(targetRect().contains(wheelEvent->position().toPoint())){
            zoom(wheelEvent->angleDelta().y(), wheelEvent->position());
        }
    }

    return false;
}

QPointF WorkspaceWidget::pointOnImage(QPointF mouse){
    mouse -= targetRect().topLeft();
    mouse.setX(mouse.x() / targetRect().width());
    mouse.setY(mouse.y() / targetRect().height());
    QPointF point(sourceRect().topLeft());
    point.setX(point.x() + mouse.x()*sourceRect().width());
    point.setY(point.y() + mouse.y()*sourceRect().height());
    return point;
}

void WorkspaceWidget::trySetPivot(){
    pivotModeOn = true;
    imageFrame->setCursor(Qt::PointingHandCursor);
}
