#include "solarplantproperties.h"

SolarPlantProperties::SolarPlantProperties(double latitude, double beta, double gamma, double area, double efficiency, double peakPower)
{
    this->latitude = latitude;
    this->beta = beta;
    this->gamma = gamma;
    this->area = area;
    this->efficiency = efficiency;

    this->peakPower = peakPower;
}

QPair<QVector<double>, QVector<double> > SolarPlantProperties::getTheoreticalPowerCurve(QDate &date, bool cutPower)
{
    // "declination"
    QDate endOfLastYear = QDate(date.year()-1, 12, 31)   ;
    int daysSinceYearStart = endOfLastYear.daysTo(date);
    double gAngle = 23.45 * sin(2*M_PI*(daysSinceYearStart-81)/365);
    double delta = gAngle/180*M_PI;

    // declination
    double phi = this->latitude / 180 * M_PI;

 // "sunrise"
    double hourOfSunrise = 12-((acos(-1*tan(delta)*tan(phi))/15)*180/M_PI);
    double hourOfSunset = 12+((acos(-1*tan(delta)*tan(phi))/15)*180/M_PI);
    QTime sunrise = QTime::fromMSecsSinceStartOfDay(hourOfSunrise*3600*1000);
    QTime sunset = QTime::fromMSecsSinceStartOfDay(hourOfSunset*3600*1000);

    QVector<double> time;
    QVector<double> power;

    QTime currentTime = sunrise;
    while (currentTime < sunset) {
        time << currentTime.msecsSinceStartOfDay();
        double currentPower = this->calculatePower();
        if (currentPower > peakPower) {
            currentPower = peakPower;
        }
        power << currentPower;

        currentTime = time.addSecs(5*60); // every five minutes
    }


}

