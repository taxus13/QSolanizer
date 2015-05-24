#include "solarpart.h"

SolarPart::SolarPart()
{
    this->datesAdded =  new QSet<QDate>();
    this->yearData = new QMap<int, Year*>();
    this->start = new QDate();
    this->end = new QDate();
    this->energy = 0;
    this->duration = 0;
}

SolarPart::SolarPart(QDate *start, QDate *end, QSet<QDate> *datesAdded, QMap<int, Year *> *yearData, float energy, float duration) {
    this->start = start;
    this->end = end;
    this->datesAdded = datesAdded;
    this->yearData = yearData;
    this->energy = energy;
    this->duration = duration;
}

SolarPart::~SolarPart()
{
    delete this->start;
    delete this->end;
    delete this->datesAdded;
    delete this->yearData;
}

void SolarPart::addDay(Day *day) {
    if (!this->datesAdded->contains(day->getDate())) {
        if (!yearData->contains(day->getDate().year())) {
            yearData->insert(day->getDate().year(), new Year());
        }
        yearData->value(day->getDate().year())->addDay(day);
        this->energy += day->getEnergy();
        this->duration += day->getDuration();
        this->datesAdded->insert(day->getDate());
    }


}

void SolarPart::doFinalStatistics()
{
    this->start = new QDate(this->yearData->first()->getFirst());
    this->end = new QDate(this->yearData->last()->getLast());
}

float SolarPart::getEnergy() const
{
    return this->energy;
}


float SolarPart::getDuration() const {
    return this->duration;
}

QDate* SolarPart::getBeginningDate() const
{
    return this->start;
}

QDate* SolarPart::getEndingDate() const
{
    return this->end;
}

Year *SolarPart::getYear(QDate *date)
{
    // TODO check if year is available
    return this->yearData->value(date->year());
}

Year *SolarPart::getYear(int year)
{
     return this->yearData->value(year);
}

Month *SolarPart::getMonth(QDate *date)
{
   return this->yearData->value(date->year())->getMonth(date);
}

Day *SolarPart::getDay(QDate *date)
{
    return this->yearData->value(date->year())->getDay(date);
}

QList<Day *> SolarPart::getDaysInRange(QDate *startDate, QDate *endDate)
{
    QList<Day *> days;
    if (*startDate <= *endDate) { // check if the parameters make sense
        QDate *date = startDate;
        while (*date <= *endDate) {
            Day *day = this->getDay(date);
            days.append(day);
            QDate date2 = date->addDays(1);
            date = &date2;
        }
    }
    return days;
}

QList<int> SolarPart::getYearList()
{
    QList<int> yearList = QList<int>();
    for (int i=this->start->year(); i<=this->end->year(); ++i) {
        yearList.append(i);
    }
    return yearList;
}

QPair<QVector<int>, QVector<float> > SolarPart::getEnergyValues()
{
    QVector<int> dates;
    QVector<float> energy;
    foreach (int year, this->yearData->keys()) {
        dates << year;

    }
    foreach (Year* year, this->yearData->values()) {
        energy << year->getEnergy();
    }

    return QPair<QVector<int>, QVector<float> >(dates, energy);
}

QPair<QVector<QDate>, QVector<float> > SolarPart::getEnergyValuesOfDays(QDate *startDate, QDate *endDate)
{
    QVector<QDate> dates;
    QVector<float> energy;
    QList<Day *> days = this->getDaysInRange(startDate, endDate);
    foreach (Day* day, days) {
        dates << day->getDate();
        energy << day->getEnergy();
    }

    return QPair<QVector<QDate>, QVector<float> >(dates, energy);
}


int SolarPart::getDayCount()
{
    return datesAdded->size();
}

QSet<QDate> *SolarPart::getDatesAdded() const {
    return this->datesAdded;
}

QMap<int, Year*> *SolarPart::getYearData() const {
    return this->yearData;
}

QDataStream &operator <<(QDataStream &out, const SolarPart &sp)
{
    out << sp.getYearData()->size();
    QMapIterator<int, Year*> iterator(*sp.getYearData());
    while (iterator.hasNext()) {
        iterator.next();
        out << iterator.key();
        out << iterator.value();
    }
    out <<  sp.getBeginningDate() << sp.getEndingDate() << *sp.getDatesAdded()
         << sp.getEnergy() << sp.getDuration();
    return out;
}


QDataStream &operator >>(QDataStream &in, SolarPart &sp)
{
    qDebug() << "Deserialize SolarPart";
    int size;
    QDate start;
    QDate end;
    QSet<QDate> datesAdded;
    QMap<int, Year*> *yearData = new QMap<int, Year*>();
    float energy;
    float duration;
    in >> size;
    for (int i=0; i<size; i++) {
        int key;
        Year year;
        in >> key;
        in >> year;
        yearData->insert(key, &year);
    }
    in >> start >> end >> datesAdded >> energy >> duration;

    sp = SolarPart(&start, &end, &datesAdded, yearData, energy, duration);
    return in;
}
