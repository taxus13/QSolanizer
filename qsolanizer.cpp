#include "qsolanizer.h"
#include "ui_qsolanizer.h"


QSolanizer::QSolanizer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QSolanizer)
{
    ui->setupUi(this);
//    connect(ui->bTest, SIGNAL(clicked()), this, SLOT(test()));

    ui->tMonthSelection->setSelectionMode(QAbstractItemView::MultiSelection);
}

QSolanizer::~QSolanizer()
{
    delete ui;
}

void QSolanizer::test() {



}

void QSolanizer::on_calendarWidget_selectionChanged()
{
    QDate date = this->ui->calendarWidget->selectedDate();
    Day *dd = sp.getDay(&date);
    QVector<double> *timeline = new QVector<double>();
    QVector<double> *power = new QVector<double>();
    QMap<QDateTime, float>::iterator i;
    for (i = dd->getPowerCurve()->begin(); i != dd->getPowerCurve()->end(); ++i) {
        timeline->append(i.key().toTime_t());
//        qDebug() << i.value();
        power->append(i.value());
    }


    ui->wPowerCurve->addGraph();
    ui->wPowerCurve->graph(0)->setData(*timeline, *power);
    ui->wPowerCurve->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->wPowerCurve->xAxis->setDateTimeFormat("HH:mm");
    ui->wPowerCurve->xAxis->setRange(timeline->at(0), timeline->at(timeline->size()-1));
    ui->wPowerCurve->yAxis->setRange(0, 15);
    ui->wPowerCurve->replot();

    qDebug() << timeline->size();
}

void QSolanizer::on_bTest_clicked()
{
    QStringList files = CSVReader::getFileList(QString("C:\\Users\\Carsten\\Documents\\SMA\\Sunny Explorer\\Tageswerte"));
//    QList<Day*> future = QtConcurrent::blockingMapped(files, &CSVReader::parseFile);
//    SolarPart sp = QtConcurrent::blockingMappedReduced(files, &CSVReader::parseFile, &CSVReader::addData);

    QFuture<SolarPart> reduced = QtConcurrent::mappedReduced(files, &CSVReader::parseFile, &CSVReader::addData);
    reduced.waitForFinished();
    sp = reduced.result();
    sp.doFinalStatistics();

    this->ui->calendarWidget->setMinimumDate(sp.getBeginningDate());
    this->ui->calendarWidget->setMaximumDate(sp.getEndingDate());
}
