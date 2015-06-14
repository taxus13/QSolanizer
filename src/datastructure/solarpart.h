#ifndef SOLARPART_H
#define SOLARPART_H

#include <QDateTime>
#include <QSet>
#include <QMap>
#include <QList>
#include <QVector>
#include <QPair>
#include <QDataStream>

#include "day.h"
#include "year.h"
#include "solarplantproperties.h"

class SolarPart
{
public:
    SolarPart();
    ~SolarPart();
    void addDay(Day day);
    void doFinalStatistics();

    inline QList<Day> getDaysInRange(QDate &startDate, QDate &endDate);

    QList<int> getYearList();
    QPair<QVector<int>, QVector<float> > getEnergyValues();

    QPair<QVector<QDate>, QVector<float> > getEnergyValuesOfDays(QDate &startDate, QDate &endDate);
    QVector<QList<QDateTime> > getSignificantTimes(QDate &startDate, QDate &endDate);
    float getSunhoursInRange(QDate &startDate, QDate &endDate);
    float getEnergyInRange(QDate &startDate, QDate &endDate);

    //meta
    int getDayCount();

    // simple get methods
    QSet<QDate> &getDatesAdded();
    QMap<int, Year> &getYearData();
    float getEnergy() const;
    float getDuration() const;
    float getHighestPower();
    float getHighestDayEnergy();
    float getHighestMonthEnergy();
    float getHighestYearEnergy();
    QDate &getBeginningDate();
    QDate &getEndingDate();

    // just some return statements
    Year &getYear(QDate &date);
    Year &getYear(int year);
    Month &getMonth(QDate &date);
    Day &getDay(QDate &date);
    Day &getAverageDay(int month);

    SolarPlantProperties &getSolarPlantProperties();
    void setSolarPlantProperties(SolarPlantProperties spp);
    // for IO
    friend QDataStream &operator <<(QDataStream &out, const SolarPart &sp);
    friend QDataStream &operator >>(QDataStream &in, SolarPart &sp);

private:
    QDate start;
    QDate end;

    QSet<QDate> datesAdded;
    QMap<int, Year> yearData;

    QMap<int, Day> averageDayData;

    bool propertiesAvailable;
    SolarPlantProperties spp;

    float energy;
    float duration;

    float highestPower;
    float highestDayEnergy;
    float highestMonthEnergy;
    float highestYearEnergy;

};



#endif // SOLARPART_H
