#include "csvreader.h"

CSVReader::CSVReader()
{

}

CSVReader::~CSVReader()
{

}

Day CSVReader::parseFile(const QString& path) {
    QFile *file = new QFile(path);
    QMap<QDateTime, float> map = QMap<QDateTime, float>();
    float startEnergy = 0;
    float endEnergy = 0;
    float sumOfPower = 0; // actually, this is energy, but ignore the timefactor, as it is not relevant here

    if(file->open(QIODevice::ReadOnly|QIODevice::Text)) {
        QTextStream textStream(file);
        QString line;


        int lineNumber = 1;
        do {
           line = textStream.readLine();
           if (lineNumber == 10) {
               QStringList currentData = line.split(";");
               QString floatString = currentData.at(1);
               floatString = floatString.replace(",", ".");
               startEnergy = floatString.toFloat();
           }
           if (lineNumber++ >= 10) {
               QStringList currentData = line.split(";");
               if (currentData.size() == 3) {
                   // parse data
                   QDateTime time = QDateTime::fromString(currentData.at(0), "dd.MM.yyyy HH:mm:ss");
                   if (!time.isValid()) {
                      // qDebug() << "invalid date: " << currentData.at(0);
                       time = QDateTime::fromString(currentData.at(0), "dd.MM.yyyy HH:mm");
                   }
                   QString floatString = currentData.at(2);
                   floatString = floatString.replace(",", ".");
                   float power = floatString.toFloat();
                   map.insert(time, power);
                   sumOfPower += power;
                   // TODO: find a better way:
                   floatString = currentData.at(1);
                   floatString = floatString.replace(",", ".");
                   endEnergy = floatString.toFloat();

               }
            }
        } while (!line.isNull());

        float energy = endEnergy - startEnergy;

        float quarterEnergy = sumOfPower/4;
        float halfEnergy = sumOfPower/2;
        float threeQuarterEnergy = 3*sumOfPower/4;

        float currentEnergy = 0;

        QMap<QDateTime, float>::iterator i ;
        QList<QDateTime> importantDates;
        QDateTime momentOfMaximumPower;
        float maximumPower = 0;

        for (i = map.begin(); i != map.end(); ++i) {
            if (i.value() != 0) {
                // look for maximumPower
                if (maximumPower < i.value()) {
                    maximumPower = i.value();
                    momentOfMaximumPower = QDateTime(i.key());
                }
                currentEnergy += i.value();
                if (importantDates.size() == 0) {
                    if (currentEnergy > 0) {
                        importantDates.append(i.key());
                    }
                } else if (importantDates.size() == 1) {
                    if (currentEnergy >= quarterEnergy) {
                        importantDates.append(i.key());
                    }
                } else if (importantDates.size() == 2) {
                    if (currentEnergy >= halfEnergy) {
                        importantDates.append(i.key());
                    }
                } else if (importantDates.size() == 3) {
                    if (currentEnergy >= threeQuarterEnergy) {
                        importantDates.append(i.key());
                    }
                }
            }
        }

        QMapIterator<QDateTime, float> j(map);
        j.toBack();
        while (j.hasPrevious()) {
            j.previous();
            if (j.value() != 0) {
                importantDates.append(j.key());
                break;
            }
        }

        // sometimes, there was no power 'produced', so sunrise and sunrise are not set.
        if (importantDates.size() < 5) {
            QDateTime time = QDateTime(map.firstKey());
            while (importantDates.size() < 5) {
                importantDates.append(time);
            }
        }

       // qDebug() << path ;
       // qDebug() << sunrise << " " << sunset << " " << momentOfMaximumPower;
       // qDebug() << sunrise->toString("HH:mm:ss") << " " << sunset->toString("HH:mm:ss") << " " << momentOfMaximumPower->toString("dd.MM.yyyy HH:mm:ss") << " " << maximumPower << " " << energy;
        file->close();
        return Day(map, importantDates, momentOfMaximumPower, maximumPower, energy);
    } else {
        file->close();
        return Day();
    }


}

QStringList CSVReader::getFileList(const QString &path)
{
    QDir dir = QDir(path);
    QStringList filters;
    filters << "*.csv";
    dir.setNameFilters(filters);
    dir.setSorting(QDir::Name);
    QStringList files = dir.entryList();
    QStringList absolutePathFiles = QStringList();

    QStringListIterator i(files);
    while (i.hasNext()) {
        QString file = i.next();
        absolutePathFiles.append(dir.absoluteFilePath(file));
    }
    return absolutePathFiles;
}

void CSVReader::addData(SolarPart &solarPart, const Day &day)
{
    solarPart.addDay(Day(day));
}
