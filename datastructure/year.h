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


    Month* getMonth(QDate *date);
    Month* getMonth(int month);
    Day* getDay(QDate *date);
    QDate getFirst();
    QDate getLast();
    QList<QDate> getMonthList();
    QPair<QVector<QDate>, QVector<float> > getEnergyValues();
    float getEnergy();
    float getDuration();
    bool hasDataOfMonth(int month);
private:
    QMap<int, Month*> monthdata;
    float energy;
    float duration;


};

#endif // YEAR_H
