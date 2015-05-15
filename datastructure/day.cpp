#include "day.h"

Day::Day(QMap<QDateTime, float> *powerCurve, QDateTime *sunrise, QDateTime *sunset, QDateTime *momentOfMaximumPower, float maximumPower, float energy) {
    this->powerCurve = powerCurve;
    this->sunrise = sunrise;
    this->sunset = sunset;
    this->momentOfMaximumPower = momentOfMaximumPower;
    this->maximumPower = maximumPower;
    this->energy = energy;
    this->duration = sunrise->secsTo(*sunset)/3600; // save some cpu time
    // qDebug() << sunrise->toString("dd.MM.yyyy") << " " << sunset << " " << momentOfMaximumPower << " " << maximumPower << " " << energy;
}

Day::Day()
{

}

Day::~Day()
{

}

QMap<QDateTime, float>* Day::getPowerCurve() const
{
    return this->powerCurve;
}

QDate Day::getDate() const
{
    return this->sunrise->date();
}

QDateTime *Day::getSunrise() const {
    return this->sunrise;
}

QDateTime *Day::getSunset() const {
    return this->sunset;
}

QDateTime *Day::getMomentOfMaximumPower() const {
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

