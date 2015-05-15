#ifndef MONTH_H
#define MONTH_H

#include <QMap>


#include "datastructure/day.h"

class Month
{
public:
    Month();
    ~Month();
    void addDay(Day* day);
    float getEnergy();
    float getDuration();
    QDate getFirst();
    QDate getLast();
    QList<Day*> getAllDays();
    QPair<QVector<QDate>, QVector<float> > getEnergyValues();

    Day* getDay(QDate *date);

private:
    QMap<int, Day*> dayData;
    float energy;
    float duration;

};

#endif // MONTH_H
