#include "headers/distortion.h"
#include <QListWidgetItem>

class ListDistortionItem : public QListWidgetItem
{
public:
    explicit ListDistortionItem(const QString &text, Distortion _d);
    ~ListDistortionItem();
    Distortion& getDistortion();

private:
    Distortion d;
};
