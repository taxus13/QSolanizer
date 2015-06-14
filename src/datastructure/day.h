#ifndef DAY_H
#define DAY_H

//#include <QMap>
#include <QDateTime>
#include <QString>
#include <QDataStream>
#include <QList>

#include <QDebug>

#include "typedefs.h"
#include "qdatarow.h"

class Day
{
public:
    Day(QPair<QDateVector, QDataRow> powerCurve, float energy);
    Day(); //QtConcurrent needs this
    Day(const Day &day); // copy constructor for the "reduced" feature
    ~Day();

    QPair<QDateVector, QDataRow> &getPowerCurve();
    QPair<QDataRow, QDataRow > getPowerCurveForPlotting();
    QList<QDateTime> &getImportantDates();
    QDate getDate() const;
    QDateTime getSunrise();
    QDateTime getSunset();
    QDateTime getMomentOfMaximumPower();

    float getMaximumPower();
    float getEnergy();
    float getDuration();

    bool isComplete();

    // io
    friend QDataStream &operator <<(QDataStream &out, const Day& day);
    friend QDataStream &operator >>(QDataStream &in, Day& day);



private:
//    QMap<QDateTime, float> powerCurve;
    QPair<QDateVector, QDataRow> powerCurve;
    QList<QDateTime> importantDates;
    QDateTime momentOfMaximumPower;

    float maximumPower;
    float energy;
    float duration;

    void fillVariables();
};


#endif // DAY_H
