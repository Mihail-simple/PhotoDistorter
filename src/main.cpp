#include "headers/mainwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("icon.png"));
    MainWidget w;
    w.resize(800, 600);
    w.show();
    return a.exec();
}
