#include "month.h"

Month::Month()
{
    this->dayData = QMap<int, Day*>();
    this->energy = 0;
    this->duration = 0;
}

Month::~Month()
{

}

void Month::addDay(Day *day)
{
    this->dayData[day->getDate().day()] = day;
    this->energy += day->getEnergy();
    this->duration += day->getDuration();
}

float Month::getEnergy()
{
    return this->energy;
}

float Month::getDuration() {
    return this->duration;
}

Day *Month::getDay(QDate *date)
{
    return this->dayData.value(date->day());
}

QDate Month::getFirst() {
    return this->dayData.first()->getDate();
}

QDate Month::getLast() {
    return this->dayData.last()->getDate();
}

QList<Day *> Month::getAllDays()
{
    return this->dayData.values();
}

QPair<QVector<QDate>, QVector<float> > Month::getEnergyValues()
{
    QVector<QDate> dates;
    QVector<float> energy;
    foreach (Day *day, this->getAllDays()) {
        dates << day->getDate();
        energy << day->getEnergy();
    }
    return QPair<QVector<QDate>, QVector<float> >(dates, energy);
}
