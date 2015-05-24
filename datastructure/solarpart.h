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
    SolarPart(QDate *start, QDate *end, QSet<QDate> *datesAdded, QMap<int, Year*> *yearData, float energy, float duration);
    ~SolarPart();
    void addDay(Day*);
    void doFinalStatistics();

    float getEnergy() const;
    float getDuration() const;
    QDate* getBeginningDate() const;
    QDate* getEndingDate() const;
    Year* getYear(QDate *date);
    Year* getYear(int year);
    Month* getMonth(QDate *date);
    Day* getDay(QDate *date);
    QList<Day*> getDaysInRange(QDate *startDate, QDate *endDate);
    QList<int> getYearList();
    QPair<QVector<int>, QVector<float> > getEnergyValues();

    QPair<QVector<QDate>, QVector<float> > getEnergyValuesOfDays(QDate *startDate, QDate *endDate);

    //meta
    int getDayCount();

    // for storing
    QSet<QDate> *getDatesAdded() const;
    QMap<int, Year*> *getYearData() const;

private:
    QDate *start;
    QDate *end;

    QSet<QDate> *datesAdded;
    QMap<int, Year*> *yearData;

    float energy;
    float duration;

};

QDataStream &operator <<(QDataStream &out, const SolarPart &sp);
QDataStream &operator >>(QDataStream &in, SolarPart &sp);

#endif // SOLARPART_H
