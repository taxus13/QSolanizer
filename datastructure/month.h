#ifndef MONTH_H
#define MONTH_H

#include <QMap>
#include <QDataStream>

#include "datastructure/day.h"

class Month
{
public:
    Month();
    Month(QMap<int, Day *> *daydata, float energy, float duration);
    ~Month();
    void addDay(Day* day);

    float getEnergy() const;
    float getDuration() const;
    QMap<int, Day*> *getDayData() const;

    QDate getFirst();
    QDate getLast();
    QList<Day*> getAllDays();
    QPair<QVector<QDate>, QVector<float> > getEnergyValues();

    Day* getDay(QDate *date);

private:
    QMap<int, Day*>* dayData;
    float energy;
    float duration;

};

QDataStream &operator <<(QDataStream &out, const Month& month);
QDataStream &operator >>(QDataStream &in, Month& month);

#endif // MONTH_H
