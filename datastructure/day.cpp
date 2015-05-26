#include "day.h"

Day::Day(QMap<QDateTime, float> powerCurve, QList<QDateTime> importantDates, QDateTime momentOfMaximumPower, float maximumPower, float energy) {
    this->powerCurve = powerCurve;
    this->importantDates = importantDates;
    this->momentOfMaximumPower = momentOfMaximumPower;
    this->maximumPower = maximumPower;
    this->energy = energy;
    this->calculateDuration();
//     qDebug() << sunrise.toString("dd.MM.yyyy") << " " << sunset << " " << momentOfMaximumPower << " " << maximumPower << " " << energy;
//     qDebug() << "full day init";
}

Day::Day()
{
}

Day::Day(const Day &day) {
    this->powerCurve = day.powerCurve;
    this->importantDates = day.importantDates;
    this->momentOfMaximumPower = day.momentOfMaximumPower;
    this->maximumPower = day.maximumPower;
    this->energy = day.energy;
    this->calculateDuration();
//    qDebug() << sunrise.toString("dd.MM.yyyy") << " " << sunset << " " << momentOfMaximumPower << " " << maximumPower << " " << energy;
//    qDebug() << "copy day constructor";
}

Day::~Day()
{

}

QList<QDateTime> &Day::getImportantDates()
{
    return this->importantDates;
}

QMap<QDateTime, float> &Day::getPowerCurve()
{
    return this->powerCurve;
}

QDate Day::getDate() const
{
    return this->importantDates.at(0).date();
}

QDateTime Day::getSunrise() {
    return this->importantDates.at(0);
}

QDateTime Day::getSunset() {
    return this->importantDates.at(4);
}

QDateTime Day::getMomentOfMaximumPower() {
    return this->momentOfMaximumPower;
}

float Day::getMaximumPower() {
    return this->maximumPower;
}

float Day::getEnergy() {
    return this->energy;
}

float Day::getDuration() {
    return this->duration;
}

void Day::calculateDuration()
{
    this->duration = this->importantDates.at(0).secsTo(this->importantDates.at(4))/3600;
}



QDataStream &operator <<(QDataStream &out, const Day &day)
{
   out << day.powerCurve << day.importantDates
       << day.momentOfMaximumPower << day.maximumPower << day.energy;
   return out;
}


QDataStream &operator >>(QDataStream &in, Day &day)
{
    QMap<QDateTime, float> powerCurve;
    QList<QDateTime> importantDates;
    QDateTime momentOfMaximumPower;

    float maximumPower;
    float energy;
    in >> powerCurve >> importantDates >> momentOfMaximumPower >> maximumPower >> energy;

    day.powerCurve = powerCurve;
    day.importantDates = importantDates;
    day.momentOfMaximumPower = momentOfMaximumPower;
    day.maximumPower = maximumPower;
    day.energy = energy;
    day.calculateDuration();
    return in;
}
