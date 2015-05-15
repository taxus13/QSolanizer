#ifndef DAY_H
#define DAY_H

#include <QMap>
#include <QDateTime>
#include <QString>

#include <QDebug>

class Day
{
public:
    Day(QMap<QDateTime, float>*, QDateTime*, QDateTime*, QDateTime*, float, float);
    Day(); //just for testing!!!
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

#endif // DAY_H
