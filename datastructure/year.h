#ifndef YEAR_H
#define YEAR_H

#include <QMap>
#include <QList>

#include "datastructure/day.h"
#include "datastructure/month.h"

class Year
{
public:
    Year();
    ~Year();
    void addDay(Day *day);
    float energy;
    float duration;

    Month* getMonth(QDate *date);
    Day* getDay(QDate *date);
    QDate getFirst();
    QDate getLast();
    QList<QDate> getMonthList();

private:
    QMap<int, Month*> monthdata;
    float getEnergy();
    float getDuration();

};

#endif // YEAR_H
