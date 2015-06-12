#include "solarplantproperties.h"

SolarPlantProperties::SolarPlantProperties(double latitude, double beta, double gamma, double area, double efficiency, double peakPower)
{
    this->latitude = qDegreesToRadians(latitude);
    this->beta = qDegreesToRadians(beta);
    this->gamma = qDegreesToRadians(gamma);

    this->area = area;
    this->efficiency = efficiency/100;
    this->peakPower = peakPower;
}

SolarPlantProperties::SolarPlantProperties() {
    this->latitude = 0;
    this->beta = 0;
    this->gamma = 0;

    this->area = 0;
    this->efficiency = 0;
    this->peakPower = 0;
}

QPair<QVector<double>, QVector<double> > SolarPlantProperties::getTheoreticalPowerCurve(QDate &date, bool cutPower)
{
    qDebug() << date;
    QDate endOfLastYear = QDate(date.year()-1, 12, 31);
    int daysSinceYearStart = endOfLastYear.daysTo(date);
    double gAngle = 23.45 * sin(2*M_PI*(daysSinceYearStart-81)/365);
    double delta = qDegreesToRadians(gAngle);    // declination (rad)
    double phi = this->latitude;

    double timeToZenit = qRadiansToDegrees(acos(-1*tan(delta)*tan(phi))/15);
    double hourOfSunrise = 12-timeToZenit;
    double hourOfSunset = 12+timeToZenit;
    qDebug() << hourOfSunrise << hourOfSunset;

    QVector<double> time;
    QVector<double> power;

    double currentTime = hourOfSunrise;

    while (currentTime < hourOfSunset) {
        time << currentTime * 3600 * 1000;
        double currentPower = this->calculatePower(currentTime, delta);
        if (cutPower && (currentPower > peakPower)) {
            currentPower = peakPower;
        }
        power << currentPower;
        qDebug() << currentTime << currentPower;
        currentTime += 5.0/60;
    }

    return QPair<QVector<double>, QVector<double> >(time, power);
}

double SolarPlantProperties::calculatePower(double hour, double declination)
{
    double omega = 2 * M_PI * ((hour-12)/24); // hourAngle (rad)
    double delta = declination;
    double sunAngle = sin(this->beta)*cos(this->gamma)*(cos(delta)*cos(omega)*sin(this->latitude)-sin(delta)*cos(this->latitude))
            +sin(this->beta)*sin(this->gamma)*cos(delta)*sin(omega)+cos(this->beta)*(cos(delta)*cos(omega)*cos(this->latitude)
            +sin(delta)*sin(this->latitude));

    return this->solarConstant * this->area * this->efficiency * sunAngle;
}
double SolarPlantProperties::getPeakPower() const
{
    return peakPower;
}

double SolarPlantProperties::getEfficiency() const
{
    return efficiency;
}

double SolarPlantProperties::getArea() const
{
    return qRadiansToDegrees(this->area);
}

double SolarPlantProperties::getGamma()
{
    return qRadiansToDegrees(this->gamma);
}

double SolarPlantProperties::getBeta()
{
    return qRadiansToDegrees(this->beta);
}

double SolarPlantProperties::getLatitude()
{
    return qRadiansToDegrees(this->latitude);
}

