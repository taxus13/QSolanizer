#include "solarpart.h"

SolarPart::SolarPart()
{
    this->datesAdded = QSet();
    this->yearData = QMap<int, Year*>();
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
    }
}
