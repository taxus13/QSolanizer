#include "solarplantproperties.h"

SolarPlantProperties::SolarPlantProperties(double latitude, double beta, double gamma, double area, double efficiency, double peakPower)
{
    this->latitude = qDegreesToRadians(latitude);
    this->beta = qDegreesToRadians(beta);
    this->gamma = qDegreesToRadians(gamma);

    this->area = area;
    this->efficiency = efficiency;
    this->peakPower = peakPower;
}

QPair<QVector<double>, QVector<double> > SolarPlantProperties::getTheoreticalPowerCurve(QDate &date, bool cutPower)
{
    QDate endOfLastYear = QDate(date.year()-1, 12, 31);
    int daysSinceYearStart = endOfLastYear.daysTo(date);
    double gAngle = 23.45 * sin(2*M_PI*(daysSinceYearStart-81)/365);
    double delta = qDegreesToRadians(gAngle);    // declination (rad)

    double phi = this->latitude;

    double timeToZenit = qRadiansToDegrees(acos(-1*tan(delta)*tan(phi))/15);

    double hourOfSunrise = 12-timeToZenit;
    double hourOfSunset = 12+timeToZenit;

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

        currentTime += 5/60;
    }


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

