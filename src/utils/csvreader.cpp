#include "csvreader.h"

Day CSVReader::parseFile(const QString& path) {
    QFile *file = new QFile(path);
    QDateVector timeVector;
    QDataRow dataVector;

    float startEnergy = 0;
    float endEnergy = 0;
    float sumOfPower = 0; // actually, this is an energy value, but ignorea the timefactor, as it is not relevant here

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
                       time = QDateTime::fromString(currentData.at(0), "dd.MM.yyyy HH:mm");
                   }
                   QString floatString = currentData.at(2);
                   floatString = floatString.replace(",", ".");
                   float power = floatString.toFloat();
//                   map.insert(time, power);
                   timeVector << time;
                   dataVector << power;
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

//        QMap<QDateTime, float>::iterator i ;
        QList<QDateTime> importantDates;
        QDateTime momentOfMaximumPower;
        float maximumPower = 0;

        for (int i=0; i<dataVector.size();++i) {
            if (dataVector[i] != 0) {
                // look for maximumPower
                if (maximumPower < dataVector[i]) {
                    maximumPower = dataVector[i];
                    momentOfMaximumPower = QDateTime(timeVector[i]);
                }
                currentEnergy += dataVector[i];
                if (importantDates.size() == 0) {
                    if (currentEnergy > 0) {
                        importantDates.append(timeVector[i]);
                    }
                } else if (importantDates.size() == 1) {
                    if (currentEnergy >= quarterEnergy) {
                        importantDates.append(timeVector[i]);
                    }
                } else if (importantDates.size() == 2) {
                    if (currentEnergy >= halfEnergy) {
                        importantDates.append(timeVector[i]);
                    }
                } else if (importantDates.size() == 3) {
                    if (currentEnergy >= threeQuarterEnergy) {
                        importantDates.append(timeVector[i]);
                    }
                }
            }
        }

        for (int i=dataVector.size()-1; i>0; i--) {
            if (dataVector[i] != 0) {
                importantDates.append(timeVector[i]);
                break;
            }
        }
//        QMapIterator<QDateTime, float> j(map);
//        j.toBack();
//        while (j.hasPrevious()) {
//            j.previous();
//            if (j.value() != 0) {
//                importantDates.append(j.key());
//                break;
//            }
//        }

        // sometimes, there was no power 'produced', so sunrise and sunrise are not set.
        if (importantDates.size() < 5) {
            QDateTime time = QDateTime(timeVector[0]);
            while (importantDates.size() < 5) {
                importantDates.append(time);
            }
        }
        file->close();
        return Day(QPair<QDateVector, QDataRow>(timeVector, dataVector), importantDates, momentOfMaximumPower, maximumPower, energy);
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
