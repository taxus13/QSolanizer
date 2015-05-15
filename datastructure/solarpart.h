#ifndef SOLARPART_H
#define SOLARPART_H

#include <QDateTime>
#include <QSet>
#include <QMap>
#include <QList>
#include <QVector>
#include <QPair>

#include "datastructure/day.h"
#include "datastructure/year.h"

class SolarPart
{
public:
    SolarPart();
    ~SolarPart();
    void addDay(Day*);
    void doFinalStatistics();

    float getEnergy();
    float getDuration();
    QDate getBeginningDate();
    QDate getEndingDate();
    Year* getYear(QDate *date);
    Year* getYear(int year);
    Month* getMonth(QDate *date);
    Day* getDay(QDate *date);
    QList<int> getYearList();


private:
    QDate start;
    QDate end;

    QSet<QDate> datesAdded;
    QMap<int, Year*> yearData;

    float energy;
    float duration;

};

#endif // SOLARPART_H
