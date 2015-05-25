#ifndef SOLARPART_H
#define SOLARPART_H

#include <QDateTime>
#include <QSet>
#include <QMap>
#include <QList>
#include <QVector>
#include <QPair>
#include <QDataStream>

#include "datastructure/day.h"
#include "datastructure/year.h"

class SolarPart
{
public:
    SolarPart();
    SolarPart(QDate start, QDate end, QSet<QDate> datesAdded, QMap<int, Year> yearData, float energy, float duration);
    ~SolarPart();
    void addDay(Day day);
    void doFinalStatistics();


    QList<Day> getDaysInRange(QDate &startDate, QDate &endDate);
    QList<int> getYearList();
    QPair<QVector<int>, QVector<float> > getEnergyValues();

    QPair<QVector<QDate>, QVector<float> > getEnergyValuesOfDays(QDate &startDate, QDate &endDate);

    //meta
    int getDayCount();

    // simple get methods
    QSet<QDate> &getDatesAdded();
    QMap<int, Year> &getYearData();
    float getEnergy() const;
    float getDuration() const;
    QDate &getBeginningDate();
    QDate &getEndingDate();

    // just some return statements
    Year &getYear(QDate &date);
    Year &getYear(int year);
    Month &getMonth(QDate &date);
    Day &getDay(QDate &date);
    // for IO
    friend QDataStream &operator <<(QDataStream &out, const SolarPart &sp);
    friend QDataStream &operator >>(QDataStream &in, SolarPart &sp);

private:
    QDate start;
    QDate end;

    QSet<QDate> datesAdded;
    QMap<int, Year> yearData;

    float energy;
    float duration;

};



#endif // SOLARPART_H
