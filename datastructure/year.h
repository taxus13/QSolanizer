#ifndef YEAR_H
#define YEAR_H

#include <QMap>

#include "datastructure/day.h"

class Year
{
public:
    Year();
    ~Year();
    void addDay(Day *day);
};

#endif // YEAR_H
