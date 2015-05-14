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
    ~Day();
    QMap<QDateTime, float> *getPowerCurve();
    QDate getDate();
    QDateTime *getSunrise();
    QDateTime *getSunset();
    QDateTime *getMomentOfMaximumPower();

    float getMaximumPower();
    float getEnergy();
    float getDuration();

private:
    void processData();

    QMap<QDateTime, float>* powerCurve;
    QDateTime *sunrise;
    QDateTime *sunset;
    QDateTime *momentOfMaximumPower;

    float maximumPower;
    float energy;
};

#endif // DAY_H
