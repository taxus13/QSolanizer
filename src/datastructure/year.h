#ifndef YEAR_H
#define YEAR_H

#include <QMap>
#include <QList>
#include <QDataStream>

#include "day.h"
#include "month.h"

class Year
{
public:
    Year();
    Year(QMap<int, Month> monthdata, float energy, float duration);
    ~Year();
    void addDay(Day day);

    QDate getFirst();
    QDate getLast();
    QList<QDate> getMonthList();
    QPair<QVector<QDate>, QVector<float> > getEnergyValues();

    bool hasDataOfMonth(int month);
    // get methods
    float getEnergy();
    float getDuration();
    QMap<int, Month> &getMonthData();

    // some return funcs
    Month &getMonth(QDate &date);
    Month &getMonth(int month);
    Day &getDay(QDate &date);

    // for io
    friend QDataStream &operator <<(QDataStream &out, const Year &year);
    friend QDataStream &operator >>(QDataStream &in, Year &year);

private:
    QMap<int, Month> monthdata;
    float energy;
    float duration;
};



#endif // YEAR_H
