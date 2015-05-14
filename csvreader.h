#ifndef CSVREADER_H
#define CSVREADER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QtConcurrent>

#include "datastructure/solarpart.h"
#include "datastructure/year.h"
#include "datastructure/month.h"
#include "datastructure/day.h"

#include <QDebug>

class CSVReader
{




public:
    CSVReader();
    ~CSVReader();
    static Day *parseFile(const QString &);
    static QList<Day*>* parseFiles(const QString &);
    static QStringList getFileList(const QString &);
    static void processParsedData(Solar);
};

#endif // CSVREADER_H
