#include "headers/listdistortionitem.h"

ListDistortionItem::ListDistortionItem(
        const QString &text,
        Distortion _d) : QListWidgetItem(text), d(_d)
{
}

ListDistortionItem::~ListDistortionItem() {}

Distortion& ListDistortionItem::getDistortion(){
    return d;
}
