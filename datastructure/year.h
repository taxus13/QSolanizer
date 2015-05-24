#ifndef YEAR_H
#define YEAR_H

#include <QMap>
#include <QList>
#include <QDataStream>

#include "datastructure/day.h"
#include "datastructure/month.h"

class Year
{
public:
    Year();
    Year(QMap<int, Month *> *monthdata, float energy, float duration);
    ~Year();
    void addDay(Day *day);


    Month* getMonth(QDate *date);
    Month* getMonth(int month);
    Day* getDay(QDate *date);
    QDate getFirst();
    QDate getLast();
    QList<QDate> getMonthList();
    QPair<QVector<QDate>, QVector<float> > getEnergyValues();

    float getEnergy() const;
    float getDuration() const;
    QMap<int, Month*> *getMonthData() const;

    bool hasDataOfMonth(int month);
private:
    QMap<int, Month*> *monthdata;
    float energy;
    float duration;
};

QDataStream &operator <<(QDataStream &out, const Year &year);
QDataStream &operator >>(QDataStream &in, Year &year);

#endif // YEAR_H
