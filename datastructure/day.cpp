#include "day.h"

Day::Day(QMap<QDateTime, float> *powerCurve, QDateTime *sunrise, QDateTime *sunset, QDateTime *momentOfMaximumPower, float maximumPower, float energy) {
    this->powerCurve = powerCurve;
    this->sunrise = sunrise;
    this->sunset = sunset;
    this->momentOfMaximumPower = momentOfMaximumPower;
    this->maximumPower = maximumPower;
    this->energy = energy;
   // qDebug() << sunrise->toString("dd.MM.yyyy") << " " << sunset << " " << momentOfMaximumPower << " " << maximumPower << " " << energy;
}

Day::~Day()
{

}

QMap<QDateTime, float>* Day::getPowerCurve()
{
    return this->powerCurve;
}

QDate Day::getDate()
{
    return this->sunrise->date();
}

QDateTime *Day::getSunrise() {
    return this->sunrise;
}

QDateTime *Day::getSunset() {
    return this->sunset;
}

QDateTime *Day::getMomentOfMaximumPower() {
    return this->momentOfMaximumPower;
}

float Day::getMaximumPower() {
    return this->maximumPower;
}

float Day::getEnergy() {
    return this->energy;
}

float Day::getDuration() {
    return sunrise->secsTo(*sunset)/3600;
}

void Day::processData() {
//    QMap<QDateTime, float>::iterator i;
//    boolean beginFound = false;
//    this->maximumPower = 0f;
//    for (i = this->powerCurve->begin(); i != this->powerCurve->end(); ++i) {
//        if (!beginFound) {
//            if (i.value() != 0) {
//                beginFound = true;
//                this->sunrise = i.key();
//            }
//        }

//        if (this->maximumPower < i.value()) {
//            this->maximumPower = i.value();
//            this->momentOfMaximumPower = i.key();
//        }

//    }



//    for (i = dd->end()-1; i != dd->begin(); --i) {
//        if (i.value() != 0) {
//            this->sunset = i.key();
//            break;
//        }
//    }
}


