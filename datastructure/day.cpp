#include "day.h"

Day::Day(QMap<QDateTime, float> powerCurve, QDateTime sunrise, QDateTime sunset, QDateTime momentOfMaximumPower, float maximumPower, float energy) {
    this->powerCurve = powerCurve;
    this->sunrise = sunrise;
    this->sunset = sunset;
    this->momentOfMaximumPower = momentOfMaximumPower;
    this->maximumPower = maximumPower;
    this->energy = energy;
    this->duration = sunrise.secsTo(sunset)/3600; // save some cpu time
     qDebug() << sunrise.toString("dd.MM.yyyy") << " " << sunset << " " << momentOfMaximumPower << " " << maximumPower << " " << energy;
     qDebug() << "full day init";
}

Day::Day()
{
    qDebug() << "empty day constructor";
}

Day::Day(const Day &day) {
    this->powerCurve = day.getPowerCurve();
    this->sunrise = day.getSunrise();
    this->sunset = day.getSunset();
    this->momentOfMaximumPower = day.getMomentOfMaximumPower();
    this->maximumPower = day.getMaximumPower();
    this->energy = day.getEnergy();
    this->duration = this->sunrise.secsTo(this->sunset)/3600;
    qDebug() << sunrise.toString("dd.MM.yyyy") << " " << sunset << " " << momentOfMaximumPower << " " << maximumPower << " " << energy;
    qDebug() << "copy day constructor";
}

Day::~Day()
{
//    if (this->powerCurve != 0)
//        delete this->powerCurve;
//    if (this->sunrise != 0)
//        qDebug() << this->sunrise;
//        delete this->sunrise;
//    if (this->sunset != 0)
//        delete this->sunset;
//    if (this->momentOfMaximumPower != 0)
//        delete this->momentOfMaximumPower;


}

QMap<QDateTime, float> Day::getPowerCurve() const
{
    return this->powerCurve;
}

QDate Day::getDate() const
{
    return this->sunrise.date();
}

QDateTime Day::getSunrise() const {
    return this->sunrise;
}

QDateTime Day::getSunset() const {
    return this->sunset;
}

QDateTime Day::getMomentOfMaximumPower() const {
    return this->momentOfMaximumPower;
}

float Day::getMaximumPower() const {
    return this->maximumPower;
}

float Day::getEnergy() const {
    return this->energy;
}

float Day::getDuration()  const{
    return this->duration;
}



QDataStream &operator <<(QDataStream &out, const Day &day)
{
   out << day.getPowerCurve() << day.getSunrise() << day.getSunset()
       << day.getMomentOfMaximumPower() << day.getMaximumPower() << day.getEnergy();
   return out;
}


QDataStream &operator >>(QDataStream &in, Day &day)
{
    // Attention!
    qDebug() << "Deserialize Day";
    QMap<QDateTime, float> powerCurve;
    QDateTime sunrise;
    QDateTime sunset;
    QDateTime momentOfMaximumPower;

    float maximumPower;
    float energy;
    in >> powerCurve >> sunrise >> sunset >> momentOfMaximumPower >> maximumPower >> energy;
    day = Day(powerCurve, sunrise, sunset, momentOfMaximumPower, maximumPower, energy);

    return in;
}
