#ifndef MONTH_H
#define MONTH_H

#include <QMap>
#include <QDataStream>

#include "datastructure/day.h"

class Month
{
public:
    Month();
    Month(QMap<int, Day> daydata, float energy, float duration);
    ~Month();
    void addDay(Day day);



    QDate getFirst();
    QDate getLast();
    QList<Day> getAllDays();
    QPair<QVector<QDate>, QVector<float> > getEnergyValues();

    Day& getDay(QDate &date);

    // get methods
    float getEnergy();
    float getDuration();
    QMap<int, Day> &getDayData() ;

    // for io
    friend QDataStream &operator <<(QDataStream &out, const Month& month);
    friend QDataStream &operator >>(QDataStream &in, Month& month);


private:
    QMap<int, Day> dayData;
    float energy;
    float duration;

};


#endif // MONTH_H
