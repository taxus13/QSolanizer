#include "month.h"

Month::Month()
{
    this->energy = 0;
    this->duration = 0;
}

Month::Month(QMap<int, Day> daydata, float energy, float duration) {
    this->dayData = daydata;
    this->energy = energy;
    this->duration = duration;
}

Month::~Month()
{

}

void Month::addDay(Day day)
{
    this->dayData.insert(day.getDate().day(), day);
    this->energy += day.getEnergy();
    this->duration += day.getDuration();
}

float Month::getEnergy()
{
    return this->energy;
}

float Month::getDuration() {
    return this->duration;
}

QMap<int, Day> &Month::getDayData()
{
    return this->dayData;
}

Day &Month::getDay(QDate &date)
{
    return this->dayData[date.day()];
}

QDate Month::getFirst() {
    return this->dayData.first().getDate();
}

QDate Month::getLast() {
    return this->dayData.last().getDate();
}

QList<Day> Month::getAllDays()
{
    return this->dayData.values();
}

QPair<QVector<QDate>, QVector<float> > Month::getEnergyValues()
{
    QVector<QDate> dates;
    QVector<float> energy;
    foreach (Day day, this->getAllDays()) {
        dates << day.getDate();
        energy << day.getEnergy();
    }
    return QPair<QVector<QDate>, QVector<float> >(dates, energy);
}


QDataStream &operator <<(QDataStream &out, const Month &month)
{
    out << month.dayData << month.energy << month.duration;
    return out;
}


QDataStream &operator >>(QDataStream &in, Month &month)
{
    QMap<int, Day> dayData ;
    float energy;
    float duration;

    in >> dayData >>  energy >> duration;
    month.dayData = dayData;
    month.energy = energy;
    month.duration = duration;

    return in;
}
