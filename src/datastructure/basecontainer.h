#ifndef BASECONTAINER_H
#define BASECONTAINER_H

#include <QDebug>

#include "solarpart.h"
#include "day.h"

class BaseContainer
{
public:
    BaseContainer();
    ~BaseContainer();
    void addDay(Day* day);

    SolarPart getSolarPart();

private:
    SolarPart solarPart;
};

#endif // BASECONTAINER_H
