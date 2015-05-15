#include "solarpart.h"

SolarPart::SolarPart()
{
    this->datesAdded = QSet<QDate>();
    this->yearData = QMap<int, Year*>();
    this->energy = 0;
    this->duration = 0;
}

SolarPart::~SolarPart()
{

}

void SolarPart::addDay(Day *day) {
    if (!this->datesAdded.contains(day->getDate())) {
        if (!yearData.contains(day->getDate().year())) {
            yearData[day->getDate().year()] = new Year();
        }
        yearData[day->getDate().year()]->addDay(day);
        this->energy += day->getEnergy();
        this->duration += day->getDuration();
        this->datesAdded.insert(day->getDate());
    }


}

void SolarPart::doFinalStatistics()
{
    this->start = this->yearData.first()->getFirst();
    this->end = this->yearData.last()->getLast();
}

float SolarPart::getEnergy()
{
    return this->energy;
}


float SolarPart::getDuration() {
    return this->duration;
}

QDate SolarPart::getBeginningDate()
{
    return this->start;
}

QDate SolarPart::getEndingDate()
{
    return this->end;
}

Year *SolarPart::getYear(QDate *date)
{
    // TODO check if year is available
    return this->yearData.value(date->year());
}

Year *SolarPart::getYear(int year)
{
     return this->yearData.value(year);
}

Month *SolarPart::getMonth(QDate *date)
{
   return this->yearData.value(date->year())->getMonth(date);
}

Day *SolarPart::getDay(QDate *date)
{
    return this->yearData.value(date->year())->getDay(date);
}
