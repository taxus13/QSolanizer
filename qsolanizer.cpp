#include "qsolanizer.h"
#include "ui_qsolanizer.h"


QSolanizer::QSolanizer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QSolanizer)
{
    ui->setupUi(this);
//    connect(ui->bTest, SIGNAL(clicked()), this, SLOT(test()));

    //ui->tMonthSelection->setSelectionMode(QAbstractItemView::MultiSelection);
}

QSolanizer::~QSolanizer()
{
    delete ui;
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
   this->readData();


}

void QSolanizer::readData() {
    QStringList files = CSVReader::getFileList(QString("C:\\Users\\Carsten\\Documents\\SMA\\Sunny Explorer\\Tageswerte"));
    QFuture<SolarPart> reduced = QtConcurrent::mappedReduced(files, &CSVReader::parseFile, &CSVReader::addData);
    reduced.waitForFinished();
    sp = reduced.result();
    sp.doFinalStatistics();
    this->fillDataWidgets();
}

void QSolanizer::fillDataWidgets() {
    // set range of calendar widgets
    this->ui->calendarWidget->setMinimumDate(sp.getBeginningDate());
    this->ui->calendarWidget->setMaximumDate(sp.getEndingDate());
    this->ui->dateEdit->setMinimumDate(sp.getBeginningDate());
    this->ui->dateEdit->setMaximumDate(sp.getEndingDate());
    this->ui->dateEditStart->setMinimumDate(sp.getBeginningDate());
    this->ui->dateEditStart->setMaximumDate(sp.getEndingDate());
    this->ui->dateEditEnd->setMinimumDate(sp.getBeginningDate());
    this->ui->dateEditEnd->setMaximumDate(sp.getEndingDate());

    this->ui->tMonthSelection->clear();
    foreach (int yearNumber, sp.getYearList()) {
        QTreeWidgetItem *treeItem = new QTreeWidgetItem(this->ui->tMonthSelection);
        treeItem->setText(0, QString::number(yearNumber));
        Year *year = sp.getYear(yearNumber);
        foreach (QDate month, year->getMonthList()) {
            QTreeWidgetItem *monthItem = new QTreeWidgetItem();
            monthItem->setText(0, month.toString("MMMM"));
            treeItem->addChild(monthItem);
        }
        this->ui->tMonthSelection->addTopLevelItem(treeItem);

        this->ui->listWidget->addItem(QString::number(yearNumber));
    }

}

void QSolanizer::plotMonthData(QDate date) {
    qDebug() << date.toString("dd.MM.yyyy");
    QPair<QVector<QDate>, QVector<float> > data = sp.getMonth(&date)->getEnergyValues();
    QCPBars *bars = new QCPBars(this->ui->wMonthPlot->xAxis, this->ui->wMonthPlot->yAxis);
    this->ui->wMonthPlot->clearPlottables();
    this->ui->wMonthPlot->addPlottable(bars);
    QPen pen;
    pen.setWidthF(1.2);
    bars->setName(date.toString("MMMM yyyy"));
    pen.setColor(QColor(255, 131, 0));
    bars->setPen(pen);
    bars->setBrush(QColor(255, 131, 0, 50));
    this->ui->wMonthPlot->xAxis->setAutoTicks(false);
    this->ui->wMonthPlot->xAxis->setAutoTickLabels(false);
    QVector<double> ticks;
    QVector<QString> labels;
    QVector<double> values;
    for (int i=0; i<data.first.size(); ++i) {
        ticks << i+1;
        labels << data.first.at(i).toString("dd.MM.");
        values << data.second.at(i);
    }
    this->ui->wMonthPlot->xAxis->setTickVector(ticks);
    this->ui->wMonthPlot->xAxis->setTickVectorLabels(labels);
    this->ui->wMonthPlot->xAxis->setTickLabelRotation(60);
    this->ui->wMonthPlot->xAxis->setSubTickCount(0);
    this->ui->wMonthPlot->xAxis->setTickLength(0,4);
    this->ui->wMonthPlot->xAxis->grid()->setVisible(false);
    this->ui->wMonthPlot->xAxis->setRange(0, 31.1);
    this->ui->wMonthPlot->yAxis->setRange(0, 100);
    this->ui->wMonthPlot->yAxis->setPadding(5);
    this->ui->wMonthPlot->yAxis->grid()->setSubGridVisible(true);



    this->ui->wMonthPlot->yAxis->setLabel("Energie");

    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0,0,0,25));
    this->ui->wMonthPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    this->ui->wMonthPlot->yAxis->grid()->setSubGridPen(gridPen);
    bars->setData(ticks, values);
    this->ui->wMonthPlot->replot();
}

void QSolanizer::on_tMonthSelection_itemSelectionChanged()
{
    QList<QTreeWidgetItem *> selectedItems = this->ui->tMonthSelection->selectedItems();
    if (selectedItems.size() == 1) {
        if (selectedItems.at(0)->childCount() == 0) {
            int year = selectedItems.at(0)->parent()->text(0).toInt();
            QDate month = QDate::fromString(selectedItems.at(0)->text(0), "MMMM");
            month.setDate(year, month.month(), 1);
            this->plotMonthData(month);
        }
    }
}
