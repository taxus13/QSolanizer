#include "solarpart.h"

SolarPart::SolarPart()
{
    this->energy = 0;
    this->duration = 0;
    this->highestPower = 0;
    this->highestDayEnergy = 0;
    this->highestMonthEnergy = 0;
    this->highestYearEnergy = 0;
    this->propertiesAvailable = false;
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
    }
}

void SolarPart::doFinalStatistics()
{
    this->start = QDate(this->yearData.first().getFirst());
    this->end = QDate(this->yearData.last().getLast());

    QMap<int, QDataRow> accumulatedPowerCurves;
    QMap<int, QDataRow> maximumPowerCurve;

    QMap<int, int> dayCount;

    QDateVector dateVector;

    foreach (Year year, yearData.values()) {
        if (this->highestYearEnergy < year.getEnergy()) {
            this->highestYearEnergy = year.getEnergy();
        }
        foreach (Month month, year.getMonthData().values()) {
            if (this->highestMonthEnergy < month.getEnergy()) {
                this->highestMonthEnergy = month.getEnergy();
            }
            foreach (Day day, month.getDayData()) {
                if (day.isComplete()) {
                    if (dateVector.isEmpty()) {
                        dateVector = day.getPowerCurve().first;
                    }
                    // for average day calculation
                    if (accumulatedPowerCurves.contains(day.getDate().month())) {
                        accumulatedPowerCurves[day.getDate().month()] += day.getPowerCurve().second;
                        dayCount[day.getDate().month()]++;
                    } else {
                        accumulatedPowerCurves[day.getDate().month()] = day.getPowerCurve().second;
                        dayCount[day.getDate().month()] = 1;
                    }
                    // maximum day calculation
                    if (maximumPowerCurve.contains(day.getDate().month())) {
                        maximumPowerCurve[day.getDate().month()].applyMaximumValues(day.getPowerCurve().second);
                    } else {
                        maximumPowerCurve[day.getDate().month()] = day.getPowerCurve().second;
                    }
                }
                if (this->highestDayEnergy < day.getEnergy()) {
                    this->highestDayEnergy = day.getEnergy();
                }
                if (this->highestPower < day.getMaximumPower()) {
                    this->highestPower = day.getMaximumPower();
                }
            }
        }
    }

    foreach (int key, accumulatedPowerCurves.keys()) {
        accumulatedPowerCurves[key]/=dayCount[key];
        float energyAvg = (float) accumulatedPowerCurves[key].getSum();
        energyAvg *= (5/60.0);
        float energyMax = (float) maximumPowerCurve[key].getSum();
        energyMax *= (5/60.0);
        this->averageDayData[key] = Day(QPair<QDateVector, QDataRow>(dateVector, accumulatedPowerCurves[key]), energyAvg);
        this->maximumDayData[key] = Day(QPair<QDateVector, QDataRow>(dateVector, maximumPowerCurve[key]), energyMax);
    }
}

float SolarPart::getEnergy() const
{
    return this->energy;
}


float SolarPart::getDuration() const {
    return this->duration;
}

float SolarPart::getHighestPower()
{
    return this->highestPower;
}

float SolarPart::getHighestDayEnergy()
{
    return this->highestDayEnergy;
}

float SolarPart::getHighestMonthEnergy()
{
    return this->highestMonthEnergy;
}

float SolarPart::getHighestYearEnergy()
{
    return this->highestYearEnergy;
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

Day &SolarPart::getAverageDay(int month)
{
    return this->averageDayData[month];
}

Day &SolarPart::getMaximumDay(int month)
{
    return this->maximumDayData[month];
}

SolarPlantProperties &SolarPart::getSolarPlantProperties()
{
    return this->spp;
}

void SolarPart::setSolarPlantProperties(SolarPlantProperties spp)
{
    this->spp = spp;
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
         << sp.energy << sp.duration << sp.highestPower << sp.highestDayEnergy
         << sp.highestMonthEnergy << sp.highestYearEnergy << sp.averageDayData << sp.maximumDayData;
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
    float highestPower;
    float highestDayEnergy;
    float highestMonthEnergy;
    float highestYearEnergy;
    QMap<int, Day> averageDayData;
    QMap<int, Day> maximumDayData;

    in >> map >> start >> end >> datesAdded >> energy >> duration >> highestPower >> highestDayEnergy
            >> highestMonthEnergy >> highestYearEnergy >> averageDayData >> maximumDayData;

    sp.yearData = map;
    sp.start = start;
    sp.end = end;
    sp.energy = energy;
    sp.duration = duration;
    sp.datesAdded = datesAdded;
    sp.highestPower = highestPower;
    sp.highestDayEnergy = highestDayEnergy;
    sp.highestMonthEnergy = highestMonthEnergy;
    sp.highestYearEnergy = highestYearEnergy;
    sp.averageDayData = averageDayData;
    sp.maximumDayData = maximumDayData;

    return in;
}
