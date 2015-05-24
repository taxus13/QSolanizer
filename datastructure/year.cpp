#include "year.h"

Year::Year()
{
    this->monthdata = new QMap<int, Month*>();
    this->energy = 0;
    this->duration = 0;
}

Year::Year(QMap<int, Month *> *monthdata, float energy, float duration) {
    this->monthdata = monthdata;
    this->energy = energy;
    this->duration = duration;
}

Year::~Year()
{
    delete this->monthdata;
}

void Year::addDay(Day *day)
{
    if (!this->monthdata->contains(day->getDate().month())) {
        this->monthdata->insert(day->getDate().month(), new Month());
    }
    this->monthdata->value(day->getDate().month())->addDay(day);
    this->energy += day->getEnergy();
    this->duration += day->getDuration();
}

Month *Year::getMonth(QDate *date)
{
    return this->monthdata->value(date->month());
}

Month *Year::getMonth(int month) {
    return this->monthdata->value(month);
}

Day *Year::getDay(QDate *date)
{
    return this->monthdata->value(date->month())->getDay(date);
}

float Year::getEnergy() const
{
    return this->energy;
}

float Year::getDuration()  const {
    return this->duration;
}

bool Year::hasDataOfMonth(int month)
{
    return this->monthdata->contains(month);
}

QDate Year::getFirst() {
    return this->monthdata->first()->getFirst();
}

QDate Year::getLast() {
    return this->monthdata->last()->getLast();
}

QList<QDate> Year::getMonthList()
{
    QList<QDate> list = QList<QDate>();
    foreach (int month, this->monthdata->keys()) {
        list.append(QDate(this->getFirst().year(), month, 1));
    }
    return list;
}

QPair<QVector<QDate>, QVector<float> > Year::getEnergyValues()
{
    QVector<QDate> dates;
    QVector<float> energy;
    foreach (Month *month, this->monthdata->values()) {
        dates << month->getFirst();
        energy << month->getEnergy();
    }
    return QPair<QVector<QDate>, QVector<float> >(dates, energy);
}

QMap<int, Month *> *Year::getMonthData() const
{
    return this->monthdata;
}


QDataStream &operator <<(QDataStream &out, const Year &year)
{
    out << year.getMonthData()->size();
    QMapIterator<int, Month*> iterator(*year.getMonthData());
    while (iterator.hasNext()) {
        iterator.next();
        out << iterator.key() << *(iterator.value());
    }
    out << year.getEnergy() << year.getDuration();
    return out;
}


QDataStream &operator >>(QDataStream &in, Year &year)
{
    qDebug() << "Deserialize Year";
    QMap<int, Month*> *monthdata = new QMap<int, Month*>();
    int size;
    float energy;
    float duration;
    in >> size;
    for(int i=0; i< size; i++) {
        int key;
        Month month;
        in >> key;
        in >> month;
        monthdata->insert(key, &month);
    }

    in >> energy >> duration;
    year = Year(monthdata, energy, duration);
    return in;
}
