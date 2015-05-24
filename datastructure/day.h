#ifndef DAY_H
#define DAY_H

#include <QMap>
#include <QDateTime>
#include <QString>
#include <QDataStream>

#include <QDebug>

class Day
{
public:
    Day(QMap<QDateTime, float>* powerCurve, QDateTime* sunrise, QDateTime* sunset, QDateTime* momentOfMaximumPower, float maximumPower, float energy);
    Day(); //QtConcurrent needs this
    Day(const Day &day);
    ~Day();
    QMap<QDateTime, float> *getPowerCurve() const;
    QDate getDate() const;
    QDateTime *getSunrise() const;
    QDateTime *getSunset() const ;
    QDateTime *getMomentOfMaximumPower() const;

    float getMaximumPower() const;
    float getEnergy() const;
    float getDuration() const;

private:
    QMap<QDateTime, float>* powerCurve;
    QDateTime *sunrise;
    QDateTime *sunset;
    QDateTime *momentOfMaximumPower;

    float maximumPower;
    float energy;
    float duration;
};

QDataStream &operator <<(QDataStream &out, const Day& day);
QDataStream &operator >>(QDataStream &in, Day& day);

#endif // DAY_H
