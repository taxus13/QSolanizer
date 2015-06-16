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

Day SolarPlantProperties::getTheoreticalPowerCurve(QDate &date, bool cutPower)
{
    QDate endOfLastYear = QDate(date.year()-1, 12, 31);
    int daysSinceYearStart = endOfLastYear.daysTo(date);
    double gAngle = 23.45 * sin(2*M_PI*(daysSinceYearStart-81)/365);
    double delta = qDegreesToRadians(gAngle);    // declination (rad)
    double phi = this->latitude;

//    double timeToZenit = qRadiansToDegrees(acos(-1*tan(delta)*tan(phi))/15);

    double timeToZenit = qRadiansToDegrees(this->getOmega(delta))/15; // omega (rad) => set sunangle to 0, get omega value

    double hourOfSunrise = 12-timeToZenit;
    double hourOfSunset = 12+timeToZenit;

    float energy = 0;

    QTime sunrise((int)hourOfSunrise, (int)(hourOfSunrise-(int)hourOfSunrise)*60);
    QTime sunset((int)hourOfSunset, (int)(hourOfSunset-(int)hourOfSunset)*60);

    QDateVector time;
    QDataRow power;

    QTime currentTime = sunrise;

    while (currentTime < sunset) {
        time << QDateTime(date, currentTime);
        double currentPower = this->calculatePower(currentTime.msecsSinceStartOfDay()/(1000.0*3600), delta);
        if (cutPower && (currentPower > peakPower)) {
            currentPower = peakPower;
        }
        power << currentPower;
        energy += currentPower*5/60;
        currentTime = currentTime.addSecs(5*60);
    }
    power << .0;
    time << QDateTime(date, currentTime); // trailing 0

    QTime stime(12, 0);
    QDateTime dt(date, stime);
    if (dt.isDaylightTime()) {
        for(int i=0; i<time.size(); i++) {
            time[i]=time[i].addSecs(3600);
        }
    }
    return Day(QPair<QDateVector, QDataRow>(time, power), energy);
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

double SolarPlantProperties::getOmega(double declination)
{
    double omega = 0; //rad
    double smallestSunAngle = 999; // rad

    for (double omega_deg = 0; omega_deg < 180; omega_deg++) {
        double omega_rad = qDegreesToRadians(omega_deg);
        double sunAngle = sin(this->beta)*cos(this->gamma)*(cos(declination)*cos(omega_rad)*sin(this->latitude)-sin(declination)*cos(this->latitude))
                    +sin(this->beta)*sin(this->gamma)*cos(declination)*sin(omega_rad)+cos(this->beta)*(cos(declination)*cos(omega_rad)*cos(this->latitude)
                    +sin(declination)*sin(this->latitude));
        if (qAbs(sunAngle) < qAbs(smallestSunAngle)) {
            smallestSunAngle = sunAngle;
            omega = omega_rad;
        }
    }

    return omega;
}
double SolarPlantProperties::getPeakPower() const
{
    return peakPower;
}

void SolarPlantProperties::writePorperties(QString path)
{
    QSettings properties(path, QSettings::IniFormat);
    properties.beginGroup("Properties");
    properties.setValue("peakpower", this->peakPower);
    properties.setValue("area", this->area);
    properties.setValue("efficiency", this->efficiency);
    properties.setValue("beta", this->beta);
    properties.setValue("gamma", this->gamma);
    properties.setValue("latitude", this->latitude);
    properties.endGroup();
}

void SolarPlantProperties::readProperties(QString path)
{
    QSettings properties(path, QSettings::IniFormat);
    properties.beginGroup("Properties");
    this->peakPower = properties.value("peakpower", 0).toDouble();
    this->area = properties.value("area", 0).toDouble();
    this->efficiency = properties.value("efficiency", 0).toDouble();
    this->beta = properties.value("beta", 0).toDouble();
    this->gamma = properties.value("gamma").toDouble();
    this->latitude = properties.value("latitude", 0).toDouble();
    properties.endGroup();
}

double SolarPlantProperties::getEfficiency() const
{
    return this->efficiency;
}

double SolarPlantProperties::getEfficiencyPerc()
{
    return this->efficiency*100;
}

double SolarPlantProperties::getArea() const
{
    return this->area;
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

QDataStream &operator <<(QDataStream &out, const SolarPlantProperties &spp)
{
    out << spp.peakPower << spp.area << spp.efficiency << spp.beta << spp.gamma << spp.latitude;
    return out;
}

QDataStream &operator >>(QDataStream &in, SolarPlantProperties &spp)
{
    double peakPower;
    double area;
    double efficiency;
    double beta;
    double gamma;
    double latitude;

    in >> peakPower >> area >> efficiency >> beta >> gamma >> latitude;

    spp.peakPower = peakPower;
    spp.area = area;
    spp.efficiency = efficiency;
    spp.beta = beta;
    spp.gamma = gamma;
    spp.latitude = latitude;

    return in;
}

