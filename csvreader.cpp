#include "csvreader.h"

CSVReader::CSVReader()
{

}

CSVReader::~CSVReader()
{

}

Day CSVReader::parseFile(const QString& path) {
    QFile *file = new QFile(path);
    QMap<QDateTime, float> *map = new QMap<QDateTime, float>();
    float startEnergy = 0;
    float endEnergy = 0;

    if(file->open(QIODevice::ReadOnly|QIODevice::Text)) {
        QTextStream textStream(file);
        QString line;

        //qDebug() << "start reading";
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
                  // qDebug() << currentData;
                   // parse data
                   QDateTime time = QDateTime::fromString(currentData.at(0), "dd.MM.yyyy HH:mm:ss");
                   if (!time.isValid()) {
                      // qDebug() << "invalid date: " << currentData.at(0);
                       time = QDateTime::fromString(currentData.at(0), "dd.MM.yyyy HH:mm");
                   }
                   QString floatString = currentData.at(2);
                   floatString = floatString.replace(",", ".");
                   float power = floatString.toFloat();
//                   qDebug() << power << " " << floatString;
                   map->insert(time, power);

                   // TODO: find a better way:
                   floatString = currentData.at(1);
                   floatString = floatString.replace(",", ".");
                   endEnergy = floatString.toFloat();

               }
            }
//           qDebug() << line;
        } while (!line.isNull());

       // qDebug() << "finished reading";
        float energy = endEnergy - startEnergy;


        QMap<QDateTime, float>::iterator i ;
        QDateTime *sunrise = 0;
        QDateTime *sunset = 0;
        QDateTime *momentOfMaximumPower = 0;
        bool beginFound = false;
        float maximumPower = 0;

        for (i = map->begin(); i != map->end(); ++i) {
            if (!beginFound) {
                if (i.value() != 0) {
                    beginFound = true;
                    sunrise = new QDateTime(i.key());
                }
            }

            if (maximumPower < i.value()) {
                maximumPower = i.value();
                momentOfMaximumPower = new QDateTime(i.key());
        //        qDebug() << momentOfMaximumPower->toString("dd.MM.yyyy HH:mm:ss") << " - " << maximumPower;
            }

        }

        QMapIterator<QDateTime, float> j(*map);
        j.toBack();
        while (j.hasPrevious()) {
            j.previous();
            if (j.value() != 0) {
                sunset = new QDateTime(j.key());

                break;
            }
        }

        // sometimes, there was no power 'produced', so sunrise and sunrise are not set.
        if (!beginFound) {
            QDateTime *time = new QDateTime(map->firstKey());
            sunrise = time;
            sunset = time;
            momentOfMaximumPower = time;
        }

       // qDebug() << path ;
       // qDebug() << sunrise << " " << sunset << " " << momentOfMaximumPower;
       // qDebug() << sunrise->toString("HH:mm:ss") << " " << sunset->toString("HH:mm:ss") << " " << momentOfMaximumPower->toString("dd.MM.yyyy HH:mm:ss") << " " << maximumPower << " " << energy;
        file->close();
        return Day(map, sunrise, sunset, momentOfMaximumPower, maximumPower, energy);
    } else {
        file->close();
        QDateTime *now = new QDateTime(QDateTime::currentDateTime());
        return Day(map, now, now, now, -1, -1);
    }


}

//QList<Day*> *CSVReader::parseFiles(const QString &path) {
//    QDir dir = QDir(path);
//    QStringList filters;
//    filters << "*.csv";
//    dir.setNameFilters(filters);
//    dir.setSorting(QDir::Name);
//    QStringList files = dir.entryList();
//    QStringList absolutePathFiles = QStringList();

//    QStringListIterator i(files);
//    while (i.hasNext()) {
//        QString file = i.next();
//        absolutePathFiles.append(dir.absoluteFilePath(file));
//        //qDebug() << dir.absoluteFilePath(file);
//    }
//    QList<Day*> future = QtConcurrent::blockingMapped(absolutePathFiles, &CSVReader::parseFile);
//    qDebug() << "no of threads: " << QString::number( QThreadPool::globalInstance()->maxThreadCount());
//    return &future;
//}

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
        //qDebug() << dir.absoluteFilePath(file);
    }
    return absolutePathFiles;
}

void CSVReader::addData(SolarPart &solarPart, const Day &day)
{
    static bool created = false;
    if (!created) {
        solarPart = SolarPart();
        created = true;
    }
    solarPart.addDay(new Day(day.getPowerCurve(), day.getSunrise(), day.getSunset(), day.getMomentOfMaximumPower(), day.getMaximumPower(), day.getEnergy()));

}
