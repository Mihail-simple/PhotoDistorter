#pragma once

#include "headers/mathparser.h"
#include "headers/distortionsettings.h"
#include <vector>
#include <QImage>

enum DistortionType{
    X,
    Y,
    XY
};

class Distortion
{
public:
    Distortion();
    Distortion(const std::string& _leftXString,
               const std::string& _rightXString,
               const std::string& _leftYString,
               const std::string& _rightYString,
               const std::string& _expsrString,
               std::vector<std::string> _argNames);
    void apply(QImage* image, DistortionSettings settings);
    bool save(QString path) const;

    const std::string& getLeftXString() const;
    const std::string& getRightXString() const;
    const std::string& getLeftYString() const;
    const std::string& getRightYString() const;
    const std::string& getExprString() const;
    std::string getExtendedExprString() const;
    DistortionType type() const;
private:
    std::string leftXString;
    std::string rightXString;
    std::string leftYString;
    std::string rightYString;
    std::string exprString;
    std::vector<std::string> argNames;

    void applyX(QImage* image, DistortionSettings settings);
    void applyY(QImage* image, DistortionSettings settings);
    void applyXY(QImage* image, DistortionSettings settings);
    inline int interpolate(int c1, int c2, int c3, int c4, double dx, double dy) const;
    QColor getColor(const QImage& image, double x, double y, bool interpolation) const;
};

struct DistortionLoadResult{
    Distortion d;
    bool hasDistortion;
};

DistortionLoadResult loadDistortion(QString path);

class SingleFunc{
public:
    SingleFunc(MathParser::Expression& _expr);
    double operator() (double x);
private:
    MathParser::Expression& expr;
};

class XYFunc{
public:
    XYFunc(MathParser::Expression& _expr, double _y);
    double operator() (double x);
private:
    MathParser::Expression& expr;
    double y;
};

class YXFunc{
public:
    YXFunc(MathParser::Expression& _expr, double _x);
    double operator() (double y);
private:
    MathParser::Expression& expr;
    double x;
};
