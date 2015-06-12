#ifndef SOLARPLANTPROPERTIES_H
#define SOLARPLANTPROPERTIES_H

#include <QPair>
#include <QVector>
#include <QDateTime>
#include <QDebug>

#include "qmath.h"
#include "math.h"

class SolarPlantProperties
{
public:
    SolarPlantProperties(double latitude, double beta, double gamma, double area, double efficiency, double peakPower);
    SolarPlantProperties();
    QPair<QVector<double>, QVector<double> > getTheoreticalPowerCurve(QDate &date, bool cutPower);
    double getLatitude();
    double getBeta();
    double getGamma();
    double getArea() const;
    double getEfficiency() const;
    double getPeakPower() const;

private:
    double calculatePower(double hour, double declination);

    double latitude; // degree
    double beta; // degree
    double gamma; //degree
    double area; // squaremetres
    double efficiency; // percent

    double peakPower; // kWp

    static const double solarConstant = 1.367; // kW/m^2


};

#endif // SOLARPLANTPROPERTIES_H
