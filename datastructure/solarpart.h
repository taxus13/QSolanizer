#ifndef SOLARPART_H
#define SOLARPART_H

#include <QDateTime>
#include <QSet>
#include <QMap>

#include "datastructure/day.h"
#include "datastructure/year.h"

class SolarPart
{
public:
    SolarPart();
    ~SolarPart();
    void addDay(Day*);

private:
    QDateTime *start;
    QDateTime *end;

    QSet<QDate> datesAdded;
    QMap<int, Year*> yearData;

};

#endif // SOLARPART_H
