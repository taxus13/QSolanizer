#include "qsolanizer.h"
#include "ui_qsolanizer.h"


QSolanizer::QSolanizer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QSolanizer)
{
    ui->setupUi(this);
    connect(ui->bTest, SIGNAL(clicked()), this, SLOT(test()));
}

QSolanizer::~QSolanizer()
{
    delete ui;
}

void QSolanizer::test() {
    ui->tbOut->setText("Test succeeded");

//    Day *dd = CSVReader::parseFile(QString("C:\\Users\\Carsten\\Documents\\SMA\\Sunny Explorer\\Tageswerte\\PV-Anlage Dalheim-20120514.csv"));
//    QVector<double> *timeline = new QVector<double>();
//    QVector<double> *power = new QVector<double>();
//    QMap<QDateTime, float>::iterator i;
//    for (i = dd->getPowerCurve()->begin(); i != dd->getPowerCurve()->end(); ++i) {
//        timeline->append(i.key().toTime_t());
////        qDebug() << i.value();
//        power->append(i.value());
//    }


//    ui->wPowerCurve->addGraph();
//    ui->wPowerCurve->graph(0)->setData(*timeline, *power);
//    ui->wPowerCurve->xAxis->setTickLabelType(QCPAxis::ltDateTime);
//    ui->wPowerCurve->xAxis->setDateTimeFormat("HH:mm");
//    ui->wPowerCurve->xAxis->setRange(timeline->at(0), timeline->at(timeline->size()-1));
//    ui->wPowerCurve->yAxis->setRange(0, 15);
//    ui->wPowerCurve->replot();

//    ui->tbOut->setText(dd->getDate().toString("dd.MM.yyyy") + QString("\n") + QString::number(dd->getDuration()) + "h\n"
//                       + QString::number( dd->getMaximumPower()) + " _ " + dd->getMomentOfMaximumPower()->toString("HH:mm:ss"));

    //QList<Day*> *days =
    CSVReader::parseFiles(QString("C:\\Users\\Carsten\\Documents\\SMA\\Sunny Explorer\\Tageswerte"));
//    QListIterator<Day*> j(*days);
//    while (j.hasNext()) {
//        Day *d = j.next();
//        qDebug() << d->getDate().toString("dd.MM.yyyy") << " "  << d->getEnergy();
//    }
    qDebug() << "finished";
}
