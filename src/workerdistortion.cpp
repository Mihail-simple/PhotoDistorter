#include "headers/workerdistortion.h"

WorkerDistortion::WorkerDistortion(
        const std::vector<Distortion>& _distortions,
        QImage* _image,
        DistortionSettings _settings) :
        distortions(_distortions),
        settings(_settings)
{
    image = new QImage(*_image);
}

void WorkerDistortion::run(){
    for(size_t i = 0; i < distortions.size(); i++){
            distortions[i].apply(image, settings);
    }
    emit finished(image);
}
