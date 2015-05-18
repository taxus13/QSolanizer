#include "year.h"

Year::Year()
{
    this->monthdata = QMap<int, Month*>();
    this->energy = 0;
    this->duration = 0;
}

Year::~Year()
{

}

void Year::addDay(Day *day)
{
    if (!this->monthdata.contains(day->getDate().month())) {
        this->monthdata[day->getDate().month()] = new Month();
    }
    this->monthdata[day->getDate().month()]->addDay(day);
    this->energy += day->getEnergy();
    this->duration += day->getDuration();
}

Month *Year::getMonth(QDate *date)
{
    return this->monthdata.value(date->month());
}

Month *Year::getMonth(int month) {
    return this->monthdata.value(month);
}

Day *Year::getDay(QDate *date)
{
    return this->monthdata.value(date->month())->getDay(date);
}

float Year::getEnergy()
{
    return this->energy;
}

float Year::getDuration() {
    return this->duration;
}

bool Year::hasDataOfMonth(int month)
{
    return this->monthdata.contains(month);
}

QDate Year::getFirst() {
    return this->monthdata.first()->getFirst();
}

QDate Year::getLast() {
    return this->monthdata.last()->getLast();
}

QList<QDate> Year::getMonthList()
{
    QList<QDate> list = QList<QDate>();
    foreach (int month, this->monthdata.keys()) {
        list.append(QDate(this->getFirst().year(), month, 1));
    }
    return list;
}

QPair<QVector<QDate>, QVector<float> > Year::getEnergyValues()
{
    QVector<QDate> dates;
    QVector<float> energy;
    foreach (Month *month, this->monthdata.values()) {
        dates << month->getFirst();
        energy << month->getEnergy();
    }
    return QPair<QVector<QDate>, QVector<float> >(dates, energy);
}
