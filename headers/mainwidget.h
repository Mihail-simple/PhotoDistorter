#pragma once

#include "headers/mathparser.h"
#include "headers/distortion.h"
#include "headers/workerdistortion.h"
#include "headers/workspacewidget.h"
#include <QWidget>
#include <QFrame>
#include <QScrollBar>
#include <QGridLayout>
#include <QPainter>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMainWindow>
#include <QMouseEvent>
#include <QCheckBox>
#include <vector>
#include <QListWidget>

class MainWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

public slots:
    void loadImage();
    void saveImage();
    void addDistortionX();
    void addDistortionY();
    void addDistortionXY();
    void removeDistortion();
    void upDistortion();
    void downDistortion();
    void editDistortion();
    void imageUpdated(QImage* newImage);
    void gridUpdated(QImage* newGrid);
    void setGridActive(bool active);
    void openGridSizeDialog();
    void setGridSize(int newSize);
    void setInterpolation(bool active);
    void setPivot(QPointF newPivot);
    void resetPivot();
    void openAbout();

private:
    QListWidget* listDistortions;
    QPushButton* buttonAddDistortionX;
    QPushButton* buttonAddDistortionY;
    QPushButton* buttonAddDistortionXY;
    QPushButton* buttonRemoveDistortion;
    QPushButton* buttonUpDistortion;
    QPushButton* buttonDownDistortion;
    WorkspaceWidget* workspace;

    QImage* baseImage = nullptr;
    DistortionSettings distortionSettings;
    int gridSize = 10;

    void init();
    void addDistortion(DistortionLoadResult result);
    void updateDistortedImage();
    void updateDistortion(int row, Distortion d, const std::string& label);
    Distortion& getDistortion(int row);
    QImage* image();
    QImage* makeGrid();
    void startWorker(WorkerDistortion* worker);
};
