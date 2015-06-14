#include "day.h"

Day::Day(QPair<QDateVector, QDataRow> powerCurve, float energy) {
    this->powerCurve = powerCurve;
//    this->importantDates = importantDates;
//    this->momentOfMaximumPower = momentOfMaximumPower;
//    this->maximumPower = maximumPower;
    this->energy = energy;
    this->fillVariables();
}

Day::Day()
{
}

Day::Day(const Day &day) {
    this->powerCurve = day.powerCurve;
    this->importantDates = day.importantDates;
    this->momentOfMaximumPower = day.momentOfMaximumPower;
    this->maximumPower = day.maximumPower;
    this->energy = day.energy;
    this->duration = day.duration;
}

Day::~Day()
{

}

QList<QDateTime> &Day::getImportantDates()
{
    return this->importantDates;
}

QPair<QDateVector, QDataRow> &Day::getPowerCurve()
{
    return this->powerCurve;
}

QPair<QDataRow, QDataRow> Day::getPowerCurveForPlotting()
{
    QVector<double> timeline;

    for (int i=0; i<this->powerCurve.first.size(); i++) {
        timeline << this->powerCurve.first[i].time().msecsSinceStartOfDay();
    }
    return QPair<QVector<double>, QVector<double> >(timeline, this->powerCurve.second);
}

QDate Day::getDate() const
{
    return this->importantDates.at(0).date();
}

QDateTime Day::getSunrise() {
    return this->importantDates.at(0);
}

QDateTime Day::getSunset() {
    return this->importantDates.at(4);
}

QDateTime Day::getMomentOfMaximumPower() {
    return this->momentOfMaximumPower;
}



float Day::getMaximumPower() {
    return this->maximumPower;
}

float Day::getEnergy() {
    return this->energy;
}

float Day::getDuration() {
    return this->duration;
}

bool Day::isComplete()
{
    return this->powerCurve.second.size() == 288;
}

void Day::fillVariables()
{

    float quarterEnergy = this->energy/4;
    float halfEnergy = this->energy/2;
    float threeQuarterEnergy = 3*this->energy/4;

    float currentEnergy = 0;

    for (int i=0; i<this->powerCurve.second.size();++i) {
        if (this->powerCurve.second[i] != 0) {
            // look for maximumPower
            if (this->maximumPower < this->powerCurve.second[i]) {
                this->maximumPower = this->powerCurve.second[i];
                this->momentOfMaximumPower = QDateTime(this->powerCurve.first[i]);
            }
            currentEnergy += this->powerCurve.second[i];
            if (importantDates.size() == 0) {
                if (currentEnergy > 0) {
                    importantDates.append(this->powerCurve.first[i]);
                }
            } else if (importantDates.size() == 1) {
                if (currentEnergy >= quarterEnergy) {
                    importantDates.append(this->powerCurve.first[i]);
                }
            } else if (importantDates.size() == 2) {
                if (currentEnergy >= halfEnergy) {
                    importantDates.append(this->powerCurve.first[i]);
                }
            } else if (importantDates.size() == 3) {
                if (currentEnergy >= threeQuarterEnergy) {
                    importantDates.append(this->powerCurve.first[i]);
                }
            }
        }
    }

    for (int i=this->powerCurve.second.size()-1; i>0; i--) {
        if (this->powerCurve.second[i] != 0) {
            importantDates.append(this->powerCurve.first[i]);
            break;
        }
    }

    // sometimes, there was no power 'produced', so sunrise and sunrise are not set.
    if (importantDates.size() < 5) {
        QDateTime time = QDateTime(this->powerCurve.first[0]);
        while (importantDates.size() < 5) {
            importantDates.append(time);
        }
    }
    this->duration = this->importantDates.at(0).secsTo(this->importantDates.at(4))/3600;
}



QDataStream &operator <<(QDataStream &out, const Day &day)
{
   out << day.powerCurve.second << day.powerCurve.first << day.importantDates
       << day.momentOfMaximumPower << day.maximumPower << day.energy << day.duration;
   return out;
}


QDataStream &operator >>(QDataStream &in, Day &day)
{
    QDateVector dateVector;
    QDataRow dataVector;
    QList<QDateTime> importantDates;
    QDateTime momentOfMaximumPower;

    float maximumPower;
    float energy;
    float duration;

    in >> dataVector >> dateVector >> importantDates >> momentOfMaximumPower >>
            maximumPower >> energy >> duration;

    day.powerCurve = QPair<QDateVector, QDataRow>(dateVector, dataVector);
    day.importantDates = importantDates;
    day.momentOfMaximumPower = momentOfMaximumPower;
    day.maximumPower = maximumPower;
    day.energy = energy;
    day.duration = duration;
    return in;
}
