#ifndef CSVREADER_H
#define CSVREADER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>

#include "datastructure/solarpart.h"
#include "datastructure/day.h"

#include <QDebug>

class CSVReader
{
public:
    static Day parseFile(const QString &path);
    static QStringList getFileList(const QString &path);
    static void addData(SolarPart &solarPart, const Day &day);


};

#endif // CSVREADER_H
