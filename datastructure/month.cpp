#include "month.h"

Month::Month()
{
    this->dayData = new QMap<int, Day*>();
    this->energy = 0;
    this->duration = 0;
}

Month::Month(QMap<int, Day *>* daydata, float energy, float duration) {
    this->dayData = daydata;
    this->energy = energy;
    this->duration = duration;
}

Month::~Month()
{
    delete this->dayData;
}

void Month::addDay(Day *day)
{
    this->dayData->insert(day->getDate().day(), day);
    this->energy += day->getEnergy();
    this->duration += day->getDuration();
}

float Month::getEnergy() const
{
    return this->energy;
}

float Month::getDuration() const {
    return this->duration;
}

QMap<int, Day *> *Month::getDayData() const
{
    return this->dayData;
}

Day *Month::getDay(QDate *date)
{
    return this->dayData->value(date->day());
}

QDate Month::getFirst() {
    return this->dayData->first()->getDate();
}

QDate Month::getLast() {
    return this->dayData->last()->getDate();
}

QList<Day *> Month::getAllDays()
{
    return this->dayData->values();
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


QDataStream &operator <<(QDataStream &out, const Month &month)
{
    out << month.getDayData()->size();
    QMapIterator<int, Day*> iterator(*month.getDayData());
    while (iterator.hasNext()) {
        iterator.next();
        out << iterator.key();
        out << *(iterator.value());
    }

    out << month.getEnergy() << month.getDuration();
    return out;
}


QDataStream &operator >>(QDataStream &in, Month &month)
{
    qDebug() << "Deserialize Month";
    QMap<int, Day*> *dayData = new QMap<int, Day*>();
    float energy;
    float duration;

    int size;
    in >> size;
    for (int i=0; i < size; i++) {
        int key;
        Day day;
        in >> key;
        in >> day;
        dayData->insert(key, &day);
    }
    in >> energy >> duration;
    month = Month(dayData, energy, duration);
    return in;
}
