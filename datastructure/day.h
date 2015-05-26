#ifndef DAY_H
#define DAY_H

#include <QMap>
#include <QDateTime>
#include <QString>
#include <QDataStream>
#include <QList>

#include <QDebug>

class Day
{
public:
    Day(QMap<QDateTime, float> powerCurve, QList<QDateTime> importantDates, QDateTime momentOfMaximumPower, float maximumPower, float energy);
    Day(); //QtConcurrent needs this
    Day(const Day &day);
    ~Day();
    QMap<QDateTime, float> &getPowerCurve();
    QList<QDateTime> &getImportantDates();
    QDate getDate() const;
    QDateTime getSunrise();
    QDateTime getSunset();
    QDateTime getMomentOfMaximumPower();

    float getMaximumPower();
    float getEnergy();
    float getDuration();

    // io
    friend QDataStream &operator <<(QDataStream &out, const Day& day);
    friend QDataStream &operator >>(QDataStream &in, Day& day);



private:
    QMap<QDateTime, float> powerCurve;
    QList<QDateTime> importantDates;
    QDateTime momentOfMaximumPower;

    float maximumPower;
    float energy;
    float duration;

    void calculateDuration();
};


#endif // DAY_H
