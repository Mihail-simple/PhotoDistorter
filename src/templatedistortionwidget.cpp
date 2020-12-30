#include "headers/templatedistortionwidget.h"
#include <string>
#include <QLabel>
#include <QPushButton>
#include <QScreen>

std::string exampleImage = "example.jpg";

TemplateDistortionWidget::TemplateDistortionWidget(
        std::vector<Distortion> distortions,
        QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Load");
    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);

    scroll = new VerticalScrollArea((distortions.size()+2)/3, 3, 3);
    layout->addWidget(scroll);
    QImage image(exampleImage.c_str());
    for(size_t i = 0; i < distortions.size(); i++){
        TemplateDistortionWidgetItem* item = new TemplateDistortionWidgetItem(&image, distortions[i], i);
        connect(item, &TemplateDistortionWidgetItem::notifyNumber,
                this, &TemplateDistortionWidget::clickOK);
        scroll->addWidget(item, i/3, i%3);
    }
    resize(screen()->size().width()/2, screen()->virtualSize().height()*4/5);
}

void TemplateDistortionWidget::clickOK(int _num){
    num = _num;
    close();
}

int TemplateDistortionWidget::getNum() const{
    return num;
}
