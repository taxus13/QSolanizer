#include "day.h"

Day::Day(QPair<QDateVector, QDataRow> powerCurve, QList<QDateTime> importantDates, QDateTime momentOfMaximumPower, float maximumPower, float energy) {
    this->powerCurve = powerCurve;
    this->importantDates = importantDates;
    this->momentOfMaximumPower = momentOfMaximumPower;
    this->maximumPower = maximumPower;
    this->energy = energy;
    this->calculateDuration();
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
}

Day::~Day()
{

}

QList<QDateTime> &Day::getImportantDates()
{
    return this->importantDates;
}

QPair<QDateVector, QDataRow> &Day::getPowerCurve()
{
    return this->powerCurve;
}

QPair<QDataRow, QDataRow> Day::getPowerCurveForPlotting()
{
    QVector<double> timeline;
//    QVector<double> power;

//    QMap<QDateTime, float>::iterator i;
//    for (i = this->powerCurve.begin(); i != this->powerCurve.end(); ++i) {
//        timeline.append(i.key().time().msecsSinceStartOfDay());
//        power.append(i.value());
//    }
    for (int i=0; i<this->powerCurve.first.size(); i++) {
        timeline << this->powerCurve.first[i].time().msecsSinceStartOfDay();
    }
    return QPair<QVector<double>, QVector<double> >(timeline, this->powerCurve.second);
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
   out << day.powerCurve.second << day.powerCurve.first << day.importantDates
       << day.momentOfMaximumPower << day.maximumPower << day.energy;
   return out;
}


QDataStream &operator >>(QDataStream &in, Day &day)
{
//    QMap<QDateTime, float> powerCurve;
//    QPair<QDateTime, QDataRow> powerCurve;
    QDateVector dateVector;
    QDataRow dataVector;
    QList<QDateTime> importantDates;
    QDateTime momentOfMaximumPower;

    float maximumPower;
    float energy;
    in >> dataVector >> dateVector >> importantDates >> momentOfMaximumPower >> maximumPower >> energy;

    day.powerCurve = QPair<QDateVector, QDataRow>(dateVector, dataVector);
    day.importantDates = importantDates;
    day.momentOfMaximumPower = momentOfMaximumPower;
    day.maximumPower = maximumPower;
    day.energy = energy;
    day.calculateDuration();
    return in;
}
