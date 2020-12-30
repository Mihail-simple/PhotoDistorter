#pragma once

#include <QWidget>
#include <QHBoxLayout>

namespace QtUtils {
    QHBoxLayout* makeHBox(QWidget* widgetLeft, QWidget* widgetRight);
    bool showErrorMessage(QString text);
}
