#include "headers/mainwidget.h"
#include "headers/distortionxwidget.h"
#include "headers/distortionxywidget.h"
#include "headers/distortionywidget.h"
#include "headers/distortion.h"
#include "headers/listdistortionitem.h"
#include "headers/gridsizewidget.h"
#include "headers/qtutils.h"
#include <iostream>
#include <math.h>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QThread>

MainWidget::MainWidget(QWidget *parent) : QMainWindow(parent)
{
    init();
}

MainWidget::~MainWidget(){
    if(baseImage != nullptr) delete baseImage;
    delete listDistortions;
    delete buttonAddDistortionX;
    delete buttonAddDistortionY;
    delete buttonAddDistortionXY;
    delete buttonRemoveDistortion;
    delete buttonUpDistortion;
    delete buttonDownDistortion;
}

void MainWidget::init(){
    setWindowTitle("Photo distorter");

    QWidget* widget = new QFrame(this);
    setCentralWidget(widget);

    QHBoxLayout* layout = new QHBoxLayout(widget);
    QVBoxLayout* layoutTools = new QVBoxLayout();

    layoutTools = new QVBoxLayout();

    buttonAddDistortionX = new QPushButton("Add f(x)", this);
    buttonAddDistortionY = new QPushButton("Add f(y)", this);
    buttonAddDistortionXY = new QPushButton("Add f(x,y)", this);

    QHBoxLayout* buttonsLayout1 = new QHBoxLayout();
    buttonsLayout1->addWidget(buttonAddDistortionX);
    buttonsLayout1->addWidget(buttonAddDistortionY);
    buttonsLayout1->addWidget(buttonAddDistortionXY);
    layoutTools->addLayout(buttonsLayout1);

    buttonRemoveDistortion = new QPushButton("Remove", this);
    buttonUpDistortion = new QPushButton("Up", this);
    buttonDownDistortion = new QPushButton("Down", this);

    QHBoxLayout* buttonsLayout2 = new QHBoxLayout();
    buttonsLayout2->addWidget(buttonRemoveDistortion);
    buttonsLayout2->addWidget(buttonUpDistortion);
    buttonsLayout2->addWidget(buttonDownDistortion);
    layoutTools->addLayout(buttonsLayout2);

    listDistortions = new QListWidget(this);
    layoutTools->addWidget(listDistortions);

    layoutTools->addStretch(1);

    connect(buttonAddDistortionX, &QPushButton::clicked, this, &MainWidget::addDistortionX);
    connect(buttonAddDistortionY, &QPushButton::clicked, this, &MainWidget::addDistortionY);
    connect(buttonAddDistortionXY, &QPushButton::clicked, this, &MainWidget::addDistortionXY);
    connect(buttonRemoveDistortion, &QPushButton::clicked, this, &MainWidget::removeDistortion);
    connect(buttonUpDistortion, &QPushButton::clicked, this, &MainWidget::upDistortion);
    connect(buttonDownDistortion, &QPushButton::clicked, this, &MainWidget::downDistortion);
    connect(listDistortions, &QListWidget::itemChanged, this, &MainWidget::updateDistortedImage);
    connect(listDistortions, &QListWidget::itemDoubleClicked, this, &MainWidget::editDistortion);

    workspace = new WorkspaceWidget();

    layout->addLayout(layoutTools);
    layout->addSpacing(5);
    layout->addWidget(workspace);
    layout->setStretchFactor(workspace, 4);
    layout->setStretchFactor(layoutTools, 1);

    menuBar()->setStyleSheet("background-color: rgb(240, 240, 240);");

    QMenu *file;
    file = menuBar()->addMenu("&File");

    QAction *load = new QAction("&Open...", file);
    file->addAction(load);

    QAction *save = new QAction("&Save as...", file);
    file->addAction(save);

    QAction *quit = new QAction("&Quit", file);
    file->addAction(quit);

    connect(load, &QAction::triggered, this, &MainWidget::loadImage);
    connect(save, &QAction::triggered, this, &MainWidget::saveImage);
    connect(quit, &QAction::triggered, qApp, QApplication::quit);

    QMenu *edit;
    edit = menuBar()->addMenu("&View");

    QAction *grid = new QAction("&Grid", edit);
    grid->setCheckable(true);
    grid->setChecked(false);
    edit->addAction(grid);

    QAction *gridSize = new QAction("&Set grid size...", edit);
    edit->addAction(gridSize);

    QAction *interpolation = new QAction("&Interpolation", edit);
    interpolation->setCheckable(true);
    interpolation->setChecked(true);
    edit->addAction(interpolation);

    QAction *pivotSet = new QAction("&Set pivot...", edit);
    edit->addAction(pivotSet);

    QAction *pivotReset = new QAction("&Reset pivot",edit);
    edit->addAction(pivotReset);

    connect(grid, &QAction::triggered, this, &MainWidget::setGridActive);
    connect(gridSize, &QAction::triggered, this, &MainWidget::openGridSizeDialog);
    connect(interpolation, &QAction::triggered, this, &MainWidget::setInterpolation);
    connect(pivotSet, &QAction::triggered, workspace, &WorkspaceWidget::trySetPivot);
    connect(workspace, &WorkspaceWidget::setPivot, this, &MainWidget::setPivot);
    connect(pivotReset, &QAction::triggered, this, &MainWidget::resetPivot);

    QMenu *help;
    help = menuBar()->addMenu("&Help");

    QAction *about = new QAction("&About...", help);
    help->addAction(about);

    connect(about, &QAction::triggered, this, &MainWidget::openAbout);
}

void MainWidget::addDistortionX(){
    DistortionXWidget dialog;
    dialog.exec();
    addDistortion(dialog.getDistortion());
}

void MainWidget::addDistortionY(){
    DistortionYWidget dialog;
    dialog.exec();
    addDistortion(dialog.getDistortion());
}

void MainWidget::addDistortionXY(){
    DistortionXYWidget dialog;
    dialog.exec();
    addDistortion(dialog.getDistortion());
}

void MainWidget::addDistortion(DistortionLoadResult result){
    if(result.hasDistortion){
        std::string funcLabel = result.d.getExtendedExprString();
        QListWidgetItem* item = new ListDistortionItem(funcLabel.c_str(), result.d);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
        listDistortions->insertItem(0, item);

        updateDistortedImage();
    }
}

void MainWidget::removeDistortion(){
    int current = listDistortions->currentRow();
    if(current == -1) return;

    delete listDistortions->takeItem(current);

    updateDistortedImage();
}

void MainWidget::upDistortion(){
    int current = listDistortions->currentRow();
    if(current == -1 || current == 0) return;

    QListWidgetItem* item = listDistortions->takeItem(current);
    listDistortions->insertItem(current-1, item);
    listDistortions->setCurrentRow(current-1);

    updateDistortedImage();
}

void MainWidget::downDistortion(){
    int current = listDistortions->currentRow();
    if(current == -1 || current == listDistortions->count()-1) return;

    QListWidgetItem* item = listDistortions->takeItem(current);
    listDistortions->insertItem(current+1, item);
    listDistortions->setCurrentRow(current+1);

    updateDistortedImage();
}

void MainWidget::editDistortion(){
    int current = listDistortions->currentRow();
    if(current == -1) return;

    Distortion& d = getDistortion(current);
    DistortionWidget* dialog = nullptr;

    if(d.type() == DistortionType::X){
        dialog = new DistortionXWidget();
    }else if(d.type() == DistortionType::Y){
        dialog = new DistortionYWidget();
    }else if(d.type() == DistortionType::XY){
        dialog = new DistortionXYWidget();
    }

    if(dialog != nullptr){
        dialog->setDefault(d);
        dialog->exec();
        DistortionLoadResult result = dialog->getDistortion();
        if(result.hasDistortion){
            updateDistortion(current, result.d, result.d.getExtendedExprString());
        }

        delete dialog;
    }
}

void MainWidget::imageUpdated(QImage* newImage){
    if(baseImage == nullptr) {
        delete newImage;
        return;
    }
    workspace->setImage(newImage);
}

void MainWidget::gridUpdated(QImage* newGrid){
    workspace->setGrid(newGrid);
}

void MainWidget::updateDistortion(int row, Distortion d, const std::string& label){
    ListDistortionItem* item = new ListDistortionItem(label.c_str(), d);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(listDistortions->item(row)->checkState());
    delete listDistortions->takeItem(row);
    listDistortions->insertItem(row, item);
    listDistortions->setCurrentRow(row);

    updateDistortedImage();
}

void MainWidget::setGridActive(bool active){
    workspace->setGridActive(active);
    updateDistortedImage();
}

void MainWidget::openGridSizeDialog(){
    GridSizeWidget dialog(gridSize);
    connect(&dialog, &GridSizeWidget::setSize, this, &MainWidget::setGridSize);
    dialog.exec();
}

void MainWidget::setGridSize(int newSize){
    gridSize = newSize;
    updateDistortedImage();
}

void MainWidget::setInterpolation(bool active){
    distortionSettings = DistortionSettings(distortionSettings.getPivot(), active);
    updateDistortedImage();
}

void MainWidget::setPivot(QPointF newPivot){
    distortionSettings = DistortionSettings(newPivot, distortionSettings.hasInterpolation());
    updateDistortedImage();
}

void MainWidget::resetPivot(){
    distortionSettings = DistortionSettings(QPointF(0,0), distortionSettings.hasInterpolation());
    updateDistortedImage();
}

void MainWidget::openAbout(){
    QMessageBox msg;
    msg.setWindowTitle("Info");
    msg.setIcon(QMessageBox::Information);
    msg.setText("Photo distorter\n\n"
                "The program allows you to distort images using mathematical "
                "functions. The functions set the \"speed\"  of movement along"
                " the image axis. The movement starts from the pivot point.\n\n"
                "Copyright 2020");
    msg.exec();
}

Distortion& MainWidget::getDistortion(int row){
    return ((ListDistortionItem*) listDistortions->item(row))->getDistortion();
}

QImage* MainWidget::image(){
    return workspace->getImage();
}

void MainWidget::loadImage(){
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), QDir::currentPath(), tr("Image Files (*.png *.jpg *.bmp)"));
    if(fileName == "") return;

    if(baseImage != nullptr){
        delete baseImage;
        baseImage = nullptr;
    }
    baseImage = new QImage(fileName);

    updateDistortedImage();
    workspace->resetSliders();
}

void MainWidget::saveImage(){
    if(image() == nullptr) return;

    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Image"), QDir::currentPath(), tr("Image Files (*.png *.jpg *.bmp)"));
    if(fileName == "") return;

    image()->save(fileName);
}

QImage* MainWidget::makeGrid(){
    if(baseImage == nullptr) return nullptr;

    QImage* grid = new QImage(baseImage->size(), QImage::Format_ARGB32);
    for(int x = 0; x < grid->width(); x++){
        for(int y = 0; y < grid->height(); y++){
            if(x % gridSize == 0 || y % gridSize == 0){
                double rx = x/(double)grid->width();
                double ry = y/(double)grid->height();
                int r = std::max(0.0, 255*(1-rx-ry));
                int g = 255*rx;
                int b = 255*ry;
                grid->setPixelColor(x, y, QColor(r, g, b));
            }else{
                grid->setPixelColor(x, y, Qt::transparent);
            }
        }
    }
    return grid;
}

void MainWidget::updateDistortedImage(){
    if(baseImage == nullptr) return;

    std::vector<Distortion> distortions;
    for(int i = listDistortions->count()-1; i >= 0; i--){
        if(listDistortions->item(i)->checkState() == Qt::Checked){
            distortions.push_back(getDistortion(i));
        }
    }

    WorkerDistortion* imageWorker = new WorkerDistortion(distortions, baseImage, distortionSettings);
    connect(imageWorker, &WorkerDistortion::finished, this, &MainWidget::imageUpdated);
    startWorker(imageWorker);

    if(workspace->isGridOn()){
        WorkerDistortion* gridWorker = new WorkerDistortion(distortions, makeGrid(), distortionSettings);
        connect(gridWorker, &WorkerDistortion::finished, this, &MainWidget::gridUpdated);
        startWorker(gridWorker);
    }
}

void MainWidget::startWorker(WorkerDistortion* worker){
    auto *worker_thread = new QThread();
    worker->moveToThread(worker_thread);
    connect(worker_thread, &QThread::started, worker, &WorkerDistortion::run);
    connect(worker, &WorkerDistortion::finished, worker_thread, &QThread::quit);
    connect(worker, &WorkerDistortion::finished, worker, &WorkerDistortion::deleteLater);
    connect(worker_thread, &QThread::finished, worker_thread, &QThread::deleteLater);
    worker_thread->start();
}
