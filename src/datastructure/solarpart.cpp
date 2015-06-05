#include "solarpart.h"

SolarPart::SolarPart()
{
    this->energy = 0;
    this->duration = 0;
    this->highestDayEnergy = 0;
}

SolarPart::~SolarPart()
{
}

void SolarPart::addDay(Day day) {
    if (!this->datesAdded.contains(day.getDate())) {
        if (!this->yearData.contains(day.getDate().year())) {
            this->yearData.insert(day.getDate().year(), Year());
        }
        this->yearData[day.getDate().year()].addDay(day);
        this->energy += day.getEnergy();
        this->duration += day.getDuration();
        this->datesAdded.insert(day.getDate());
        if (this->highestDayEnergy < day.getEnergy()) {
            this->highestDayEnergy = day.getEnergy();
        }
    }
}

void SolarPart::doFinalStatistics()
{
    this->start = QDate(this->yearData.first().getFirst());
    this->end = QDate(this->yearData.last().getLast());
}

float SolarPart::getEnergy() const
{
    return this->energy;
}


float SolarPart::getDuration() const {
    return this->duration;
}

float SolarPart::getHighestDayEnergy()
{
    return this->highestDayEnergy;
}

QDate &SolarPart::getBeginningDate()
{
    return this->start;
}

QDate &SolarPart::getEndingDate()
{
    return this->end;
}

Year& SolarPart::getYear(QDate &date)
{
    // TODO check if year is available
    return this->yearData[date.year()];
}

Year& SolarPart::getYear(int year)
{
     return this->yearData[year];
}

Month& SolarPart::getMonth(QDate &date)
{
   return this->yearData[date.year()].getMonth(date);
}

Day& SolarPart::getDay(QDate &date)
{
    return this->yearData[date.year()].getDay(date);
}

QList<Day> SolarPart::getDaysInRange(QDate &startDate, QDate &endDate)
{
    QList<Day> days;
    if (startDate <= endDate) { // check if the parameters make sense
        QDate date = startDate;
        while (date <= endDate) {
            Day day = this->getDay(date);
            days.append(day);
            QDate date2 = date.addDays(1);
            date = date2;
        }
    }
    return days;
}

QList<int> SolarPart::getYearList()
{
    QList<int> yearList = QList<int>();
    for (int i=this->start.year(); i<=this->end.year(); ++i) {
        yearList.append(i);
    }
    return yearList;
}

QPair<QVector<int>, QVector<float> > SolarPart::getEnergyValues()
{
    QVector<int> dates;
    QVector<float> energy;
    foreach (int year, this->yearData.keys()) {
        dates << year;

    }
    foreach (Year year, this->yearData.values()) {
        energy << year.getEnergy();
    }

    return QPair<QVector<int>, QVector<float> >(dates, energy);
}

QPair<QVector<QDate>, QVector<float> > SolarPart::getEnergyValuesOfDays(QDate &startDate, QDate &endDate)
{
    QVector<QDate> dates;
    QVector<float> energy;
    QList<Day> days = this->getDaysInRange(startDate, endDate);
    foreach (Day day, days) {
        dates << day.getDate();
        energy << day.getEnergy();
    }

    return QPair<QVector<QDate>, QVector<float> >(dates, energy);
}

QVector<QList<QDateTime> > SolarPart::getSignificantTimes(QDate &startDate, QDate &endDate)
{
    QVector<QList<QDateTime> > significantTimes;
    QList<Day> days = this->getDaysInRange(startDate, endDate);
    foreach (Day day, days) {
        significantTimes.append(day.getImportantDates());
    }
    return significantTimes;
}

float SolarPart::getSunhoursInRange(QDate &startDate, QDate &endDate)
{
    float sunhours = 0;
    foreach (Day day, this->getDaysInRange(startDate, endDate)) {
        sunhours += day.getDuration();
    }
    return sunhours;
}

float SolarPart::getEnergyInRange(QDate &startDate, QDate &endDate)
{
    float energy = 0;
    foreach (Day day, this->getDaysInRange(startDate, endDate)) {
        energy += day.getEnergy();
    }
    return energy;
}


int SolarPart::getDayCount()
{
    return datesAdded.size();
}

QSet<QDate> &SolarPart::getDatesAdded() {
    return this->datesAdded;
}

QMap<int, Year> &SolarPart::getYearData() {
    return this->yearData;
}

QDataStream &operator <<(QDataStream &out, const SolarPart &sp)
{
    out << sp.yearData << sp.start << sp.end << sp.datesAdded
         << sp.energy << sp.duration << sp.highestDayEnergy;
    return out;
}


QDataStream &operator >>(QDataStream &in, SolarPart &sp)
{
    QMap<int, Year> map;
    QDate start;
    QDate end;
    QSet<QDate> datesAdded;
    float energy;
    float duration;
    float highestDayEnergy;

    in >> map >> start >> end >> datesAdded >> energy >> duration >> highestDayEnergy;

    sp.yearData = map;
    sp.start = start;
    sp.end = end;
    sp.energy = energy;
    sp.duration = duration;
    sp.datesAdded = datesAdded;
    sp.highestDayEnergy = highestDayEnergy;

    return in;
}
