#include "headers/distortion.h"
#include "headers/mathparser.h"
#include <QFile>
#include <QTextStream>
#include <iostream>

Distortion::Distortion(){}

Distortion::Distortion(
        const std::string& _leftXString,
        const std::string& _rightXString,
        const std::string& _leftYString,
        const std::string& _rightYString,
        const std::string& _exprString,
        std::vector<std::string> _argNames) :
    leftXString(_leftXString),
    rightXString(_rightXString),
    leftYString(_leftYString),
    rightYString(_rightYString),
    exprString(_exprString),
    argNames(_argNames)
{
}

template <typename T>
std::vector<double> makeDistortedIndexes(T f, double a, double b, int n, double pos){
    double norm = 0;

    double res = 0;
    double h = (b-a)/n;
    std::vector<double> indexes;
    std::vector<double> squares;
    for(int i = 0; i < n; i++){
        double value = f(a + h*i);
        res += value;
        norm += abs(value);
        squares.push_back(res);
    }

    double shift = *std::min_element(squares.begin(), squares.end());
    for(int i = 0; i < n; i++){
        squares[i] -= shift;
        indexes.push_back(std::max(0.0, std::min(n-1.0, squares[i]/norm*n + pos)));
    }
    return indexes;
}

void Distortion::apply(QImage* image, DistortionSettings settings){
    switch (type()) {
        case DistortionType::X:
            applyX(image, settings);
            return;
        case DistortionType::Y:
            applyY(image, settings);
            return;
        case DistortionType::XY:
            applyXY(image, settings);
            return;
    }
}

void Distortion::applyX(QImage* image, DistortionSettings settings){
    MathParser::Expression expr, left, right;
    expr.compile(exprString, argNames);
    left.compile(leftXString, {});
    right.compile(rightXString, {});
    double a = left.evaluate();
    double b = right.evaluate();

    QImage oldImage(*image);
    SingleFunc singleFunc(expr);
    std::vector<double> indexesX = makeDistortedIndexes(
        singleFunc, a, b, image->width(), settings.getPivot().x()
    );

    for(int x = 0; x < image->width(); x++){
        for(int y = 0; y < image->height(); y++){
            QColor color = getColor(oldImage, indexesX[x], y, settings.hasInterpolation());
            image->setPixelColor(x, y, color);
        }
    }
}

void Distortion::applyY(QImage* image, DistortionSettings settings){
    MathParser::Expression expr, left, right;
    expr.compile(exprString, argNames);
    left.compile(leftYString, {});
    right.compile(rightYString, {});
    double a = left.evaluate();
    double b = right.evaluate();

    QImage oldImage(*image);
    SingleFunc singleFunc(expr);
    std::vector<double> indexesY =
        makeDistortedIndexes(singleFunc, a, b, image->height(), settings.getPivot().y()
    );

    for(int x = 0; x < image->width(); x++){
        for(int y = 0; y < image->height(); y++){
            QColor color = getColor(oldImage, x, indexesY[y], settings.hasInterpolation());
            image->setPixelColor(x, y, color);
        }
    }
}

void Distortion::applyXY(QImage* image, DistortionSettings settings){
    MathParser::Expression expr, leftX, rightX, leftY, rightY;
    expr.compile(exprString, argNames);
    leftX.compile(leftXString, {});
    rightX.compile(rightXString, {});
    leftY.compile(leftYString, {});
    rightY.compile(rightYString, {});
    double ax = leftX.evaluate();
    double bx = rightX.evaluate();
    double ay = leftY.evaluate();
    double by = rightY.evaluate();

    std::vector<std::vector<double>> indexesX;
    for(int y = 0; y < image->height(); y++){
        XYFunc xyFunc(expr, ay + (by-ay)*(y+0.5)/image->height());
        indexesX.push_back(
            makeDistortedIndexes(xyFunc, ax, bx, image->width(), settings.getPivot().x())
        );
    }

    std::vector<std::vector<double>> indexesY;
    for(int x = 0; x < image->width(); x++){
        YXFunc yxFunc(expr, ax + (bx-ax)*(x+0.5)/image->width());
        indexesY.push_back(
            makeDistortedIndexes(yxFunc, ay, by, image->height(), settings.getPivot().y())
        );
    }

    QImage oldImage(*image);
    for(int x = 0; x < image->width(); x++){
        for(int y = 0; y < image->height(); y++){
            QColor color = getColor(oldImage, indexesX[y][x], indexesY[x][y], settings.hasInterpolation());
            image->setPixelColor(x, y, color);
        }
    }
}

inline int Distortion::interpolate(int c1, int c2, int c3, int c4, double dx, double dy) const{
    return static_cast<int>(
            c1 * (1-dx) * (1-dy) +
            c2 * dx * (1-dy) +
            c3 * (1-dx) * dy +
            c4 * dx * dy);
}

QColor Distortion::getColor(const QImage& image, double x, double y, bool interpolation) const{
    if(interpolation){
        int xi = (int)x;
        double dx = x - xi;
        if(xi >= image.width()-1){
            xi = image.width()-2;
            dx = 1.0;
        }

        int yi = (int)y;
        double dy = y - yi;
        if(yi >= image.height()-1){
            yi = image.height()-2;
            dy = 1.0;
        }

        QColor c1 = image.pixelColor(xi, yi);
        QColor c2 = image.pixelColor(xi+1, yi);
        QColor c3 = image.pixelColor(xi, yi+1);
        QColor c4 = image.pixelColor(xi+1, yi+1);
        int red = interpolate(c1.red(), c2.red(), c3.red(), c4.red(), dx, dy);
        int green = interpolate(c1.green(), c2.green(), c3.green(), c4.green(), dx, dy);
        int blue = interpolate(c1.blue(), c2.blue(), c3.blue(), c4.blue(), dx, dy);
        int alpha = interpolate(c1.alpha(), c2.alpha(), c3.alpha(), c4.alpha(), dx, dy);

        return QColor(red, green, blue, alpha);
    }else{
        return image.pixelColor((int)x,int(y));
    }
}

bool Distortion::save(QString path) const{
    QFile file(path + ".txt");
    if(!file.open(QIODevice::WriteOnly)) return false;

    QTextStream out(&file);
    out << leftXString.c_str() << ";";
    out << rightXString.c_str() << ";";
    out << leftYString.c_str() << ";";
    out << rightYString.c_str() << ";";
    out << exprString.c_str() << ";";

    if(type() == DistortionType::XY){
        out << "xy";
    }else{
        if(type() == DistortionType::X){
            out << "x";
        }else{
            out << "y";
        }
    }

    file.close();
    return true;
}

DistortionLoadResult loadDistortion(QString path){
    QFile file(path);

    if(!file.exists()) return {Distortion(), false};
    if(!file.open(QIODevice::ReadOnly)) return {Distortion(), false};
    QTextStream in(&file);
    if(in.atEnd()) return {Distortion(), false};
    QString line = in.readAll();
    QStringList list = line.split(';');
    if(list.size() != 6) return {Distortion(), false};

    std::vector<std::string> argNames;
    if(list.at(5) == "x" || list.at(5) == "y"){
        argNames = {list.at(5).toStdString()};
    }else if(list.at(5) == "xy"){
        argNames = {"x", "y"};
    }else return {Distortion(), false};

    file.close();

    return {Distortion(list.at(0).toStdString(),
                       list.at(1).toStdString(),
                       list.at(2).toStdString(),
                       list.at(3).toStdString(),
                       list.at(4).toStdString(),
                       argNames),
            true};
}

const std::string& Distortion::getLeftXString() const{
    return leftXString;
}

const std::string& Distortion::getRightXString() const{
    return rightXString;
}

const std::string& Distortion::getLeftYString() const{
    return leftYString;
}

const std::string& Distortion::getRightYString() const{
    return rightYString;
}

const std::string& Distortion::getExprString() const{
    return exprString;
}

std::string Distortion::getExtendedExprString() const{
    if(type() == DistortionType::X){
        return "f(x) = " + exprString;
    }else{
        if(type() == DistortionType::Y){
            return "f(y) = " + exprString;
        }else{
            return "f(x,y) = " + exprString;
        }
    }
}

DistortionType Distortion::type() const{
    if(argNames.size() == 2){
        return DistortionType::XY;
    }else{
        if(argNames[0] == "x"){
            return DistortionType::X;
        }else{
            return DistortionType::Y;
        }
    }
}

SingleFunc::SingleFunc(MathParser::Expression& _expr) : expr(_expr){
}

double SingleFunc::operator() (double x){
    return expr.evaluate(x);
}

XYFunc::XYFunc(MathParser::Expression& _expr, double _y) : expr(_expr), y(_y){
}

double XYFunc::operator() (double x){
    return expr.evaluate(x, y);
}

YXFunc::YXFunc(MathParser::Expression& _expr, double _x) : expr(_expr), x(_x){
}

double YXFunc::operator() (double y){
    return expr.evaluate(x, y);
}
