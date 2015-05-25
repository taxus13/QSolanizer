#include "basecontainer.h"

BaseContainer::BaseContainer()
{
    qDebug() << "base Container Created";
    //this->solarPart = SolarPart();
}

BaseContainer::~BaseContainer()
{
    qDebug() << "deleted base container";
}

void BaseContainer::addDay(Day *day)
{
    this->solarPart.addDay(day);
}

SolarPart BaseContainer::getSolarPart()
{
    return this->solarPart;
}

