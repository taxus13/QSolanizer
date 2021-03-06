#include "csvreader.h"

Day CSVReader::parseFile(const QString& path) {
//    QFile *file = new QFile(path);
    QFile file(path);
    QDateVector timeVector;
    QDataRow dataVector;

    float startEnergy = 0;
    float endEnergy = 0;
    float sumOfPower = 0; // actually, this is an energy value, but ignorea the timefactor, as it is not relevant here

    if(file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QTextStream textStream(&file);
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

        file.close();
        return Day(QPair<QDateVector, QDataRow>(timeVector, dataVector), energy);
    } else {
        file.close();
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
