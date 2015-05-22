#include "qsolanizer.h"
#include "ui_qsolanizer.h"


QSolanizer::QSolanizer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QSolanizer)
{
    ui->setupUi(this);
    this->someColors = QList<QColor>();
    this->someColors.append(QColor(230, 250, 7));
    this->someColors.append(QColor(250, 137, 7));
    this->someColors.append(QColor(38, 189, 60));
    this->someColors.append(QColor(45, 45, 45));
    this->someColors.append(QColor(129, 227, 227));
    this->someColors.append(QColor(129, 255, 125));
    this->someColors.append(QColor(59, 255, 60));
    this->someColors.append(QColor(104, 176, 176));
    this->someColors.append(QColor(252, 75, 5));
    this->someColors.append(QColor(133, 70, 7));

}

QSolanizer::~QSolanizer()
{
    delete ui;
}


void QSolanizer::on_calendarWidget_selectionChanged()
{
    QDate date = this->ui->calendarWidget->selectedDate();
    this->plotDayData(date);

}

void QSolanizer::on_bTest_clicked()
{
   this->readData();


}

void QSolanizer::readData() {
    QStringList files = CSVReader::getFileList(QString("C:\\Users\\Carsten\\Documents\\SMA\\Sunny Explorer\\Tageswerte"));
    QFuture<SolarPart> reduced = QtConcurrent::mappedReduced(files, &CSVReader::parseFile, &CSVReader::addData);

    QProgressDialog progressDialog;
    QFutureWatcher<SolarPart> watcher;

    progressDialog.setLabelText("Lese Daten ein");
    connect(&progressDialog, SIGNAL(canceled()), &watcher, SLOT(cancel()));
    connect(&watcher, SIGNAL(finished()), &progressDialog, SLOT(reset()));
    connect(&watcher, SIGNAL(progressRangeChanged(int,int)), &progressDialog, SLOT(setRange(int,int)));
    connect(&watcher, SIGNAL(progressValueChanged(int)), &progressDialog, SLOT(setValue(int)));
    watcher.setFuture(reduced);

    progressDialog.exec();


    watcher.waitForFinished();
    if (!watcher.isCanceled()) {
        sp = reduced.result();
        sp.doFinalStatistics();
        qDebug() << sp.getDayCount() << " days read";
        this->fillDataWidgets();
    } else {
        QMessageBox::critical(this, "Abgebrochen", "Sie haben den Vorgang abgebrochen!");
    }

}


void QSolanizer::fillDataWidgets() {
    // set range of date inputs
    this->ui->calendarWidget->setMinimumDate(sp.getBeginningDate());
    this->ui->calendarWidget->setMaximumDate(sp.getEndingDate());
    this->ui->dateEdit->setMinimumDate(sp.getBeginningDate());
    this->ui->dateEdit->setMaximumDate(sp.getEndingDate());
    this->ui->dateEditStart->setMinimumDate(sp.getBeginningDate());
    this->ui->dateEditStart->setMaximumDate(sp.getEndingDate());
    this->ui->dateEditEnd->setMinimumDate(sp.getBeginningDate());
    this->ui->dateEditEnd->setMaximumDate(sp.getEndingDate());
    // clear month selection and year selection
    this->ui->tMonthSelection->clear();
    this->ui->listWidget->clear();

    // fill tree widget for month selection
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
        //fill year list
        this->ui->listWidget->addItem(QString::number(yearNumber));
    }
    // show some data, so the plots are not empty
    this->plotDayData(sp.getEndingDate());
    this->showMonthData(sp.getEndingDate());
    this->plotYearData(sp.getEndingDate().year());
    this->plotTotalData();

    this->ui->tMonthSelection->expandAll();

}

void QSolanizer::plotDayData(QDate date)
{
    // maybe move this into day-class
    Day *dd = sp.getDay(&date);

    float midnight = QDateTime(date).toTime_t();


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
    ui->wPowerCurve->graph(0)->setPen(QPen(Qt::blue));
    ui->wPowerCurve->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->wPowerCurve->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->wPowerCurve->xAxis->setDateTimeFormat("HH:mm");
   // ui->wPowerCurve->xAxis->setRange(*/timeline->at(0), timeline->at(timeline->size()-1));
    ui->wPowerCurve->xAxis->setRange(midnight, midnight+24*3600);
    ui->wPowerCurve->xAxis->setAutoTickStep(false);
    ui->wPowerCurve->xAxis->setTickStep(3600); // 1h step
    ui->wPowerCurve->xAxis->setSubTickCount(6); // every 10 minutes
    ui->wPowerCurve->xAxis->setTickLabelRotation(60);
    ui->wPowerCurve->setAntialiasedElement(QCP::aePlottables);

    ui->wPowerCurve->yAxis->setRange(0, 15);
    ui->wPowerCurve->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->wPowerCurve->replot();

    // fill groupbox
    ui->lDayDuration->setText(QString("%1 h").arg(dd->getEnergy(),0, 'f', 2));
    ui->lDayEnergy->setText(QString("%1 kWh").arg(dd->getEnergy(),0,'f',2));
    ui->lPeakpower->setText(QString("%1 kW").arg(dd->getMaximumPower(), 0, 'f', 2));
    ui->lSunrise->setText(dd->getSunrise()->toString("HH:mm"));
    ui->lSunset->setText(dd->getSunset()->toString("HH:mm"));
}


void QSolanizer::showMonthData(QDate date) {
    Month *month = sp.getMonth(&date);
    QPair<QVector<QDate>, QVector<float> > data = month->getEnergyValues();
    this->plotDailyEnergyValues(&data);

    // fill group box data
    this->ui->lMonthEnergy->setText(QString("%1 kWh").arg(month->getEnergy(),0,'f',2));
    this->ui->lMonthSunhours->setText(QString("%1 h").arg(month->getDuration(),0,'f',2));

}

void QSolanizer::showCustomRange(QDate start, QDate end)
{
    QPair<QVector<QDate>, QVector<float> > data = sp.getEnergyValuesOfDays(&start, &end);
    this->plotDailyEnergyValues(&data);
}

void QSolanizer::plotDailyEnergyValues(QPair<QVector<QDate>, QVector<float> >* data)
{
    QCPBars *bars = new QCPBars(this->ui->wMonthPlot->xAxis, this->ui->wMonthPlot->yAxis);
    QDate startingDate = data->first.at(0);
    this->ui->wMonthPlot->clearPlottables();
    this->ui->wMonthPlot->addPlottable(bars);
    QPen pen;
    pen.setWidthF(1.2);
    //bars->setName(date.toString("MMMM yyyy"));
    pen.setColor(QColor(255, 131, 0));
    bars->setPen(pen);
    bars->setBrush(QColor(255, 131, 0, 50));
    QVector<double> ticks;
    QVector<QString> labels;
    QVector<double> values;
    for (int i=0; i<data->first.size(); ++i) {
        ticks << i+1;
        labels << data->first.at(i).toString("dd.MM.");
        values << data->second.at(i);
    }
    if (data->first.size() <= 31) {
        this->ui->wMonthPlot->xAxis->setAutoTicks(false);
        this->ui->wMonthPlot->xAxis->setAutoTickLabels(false);
        this->ui->wMonthPlot->xAxis->setTickVector(ticks);
        this->ui->wMonthPlot->xAxis->setTickVectorLabels(labels);
        this->ui->wMonthPlot->xAxis->setTickLabelRotation(60);
    } else {
        this->ui->wMonthPlot->xAxis->setAutoTicks(true);
        this->ui->wMonthPlot->xAxis->setAutoTickLabels(true);
        this->ui->wMonthPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
        this->ui->wMonthPlot->xAxis->setDateTimeFormat("dd.MM.yy");
//         qDebug() << this->ui->wMonthPlot->xAxis->tickVector();
//         labels.clear();
//         foreach (double d, this->ui->wMonthPlot->xAxis->tickVector()) {
//             QDate date = startingDate.addDays(d);
//             labels << date.toString("dd.MM.yy");
//         }
//         this->ui->wMonthPlot->xAxis->setTickVectorLabels(labels);
    }

    this->ui->wMonthPlot->xAxis->setSubTickCount(0);
    this->ui->wMonthPlot->xAxis->setTickLength(0,4);
    this->ui->wMonthPlot->xAxis->grid()->setVisible(false);
    this->ui->wMonthPlot->xAxis->setRange(0, ticks.size()+1);
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

void QSolanizer::plotYearData(int yearNumber)
{
    Year *year = sp.getYear(yearNumber);
    QPair<QVector<QDate>, QVector<float> > data = year->getEnergyValues();
    QCPBars *bars = new QCPBars(this->ui->wYearPlot->xAxis, this->ui->wYearPlot->yAxis);
    this->ui->wYearPlot->clearPlottables();
    this->ui->wYearPlot->addPlottable(bars);
    QPen pen;
    pen.setWidthF(1.2);
    bars->setName(QString::number(yearNumber));
    pen.setColor(QColor(255, 131, 0));
    bars->setPen(pen);
    bars->setBrush(QColor(255, 131, 0, 50));
    this->ui->wYearPlot->xAxis->setAutoTicks(false);
    this->ui->wYearPlot->xAxis->setAutoTickLabels(false);
    QVector<double> ticks;
    QVector<QString> labels;
    QVector<double> values;
    for (int i=0; i<data.first.size(); ++i) {
        ticks << i+1;
        labels << data.first.at(i).toString("MMMM");
        values << data.second.at(i);
    }
    this->ui->wYearPlot->xAxis->setTickVector(ticks);
    this->ui->wYearPlot->xAxis->setTickVectorLabels(labels);
    this->ui->wYearPlot->xAxis->setTickLabelRotation(60);
    this->ui->wYearPlot->xAxis->setSubTickCount(0);
    this->ui->wYearPlot->xAxis->setTickLength(0,4);
    this->ui->wYearPlot->xAxis->grid()->setVisible(false);
    this->ui->wYearPlot->xAxis->setRange(0, 13);
    this->ui->wYearPlot->yAxis->setRange(0, 2000);
    this->ui->wYearPlot->yAxis->setPadding(5);
    this->ui->wYearPlot->yAxis->grid()->setSubGridVisible(true);


    this->ui->wYearPlot->yAxis->setLabel("Energie");

    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0,0,0,25));
    this->ui->wYearPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    this->ui->wYearPlot->yAxis->grid()->setSubGridPen(gridPen);
    bars->setData(ticks, values);
    this->ui->wYearPlot->replot();

    // set group box data
    this->ui->lYearDuration->setText(QString("%1 h").arg(year->getDuration(),0,'f',0));
    this->ui->lYearEnergy->setText(QString("%1 kWh").arg(year->getEnergy(),0,'f',0));

}

void QSolanizer::plotAllYearData()
{
    this->ui->wYearPlot->clearPlottables();

    QList<QVector<double> > allEnergyData;
    QVector<double> months;
    QVector<QString> labels;

    //generate ticklabels and tickvector
    for (int i=1; i<=12; i++) {
        months << i;
        labels << QDate::fromString(QString::number(i), "M").toString("MMMM");
    }
    for (int i=sp.getBeginningDate().year(); i<=sp.getEndingDate().year(); ++i) {
        Year* year = sp.getYear(i);
        QVector<double> yearEnergy;
        foreach (double month, months) {
            if (year->hasDataOfMonth((int)month)) {
                yearEnergy << year->getMonth((int)month)->getEnergy();
            } else {
                yearEnergy << 0;
            }
        }
        allEnergyData << yearEnergy;
    }
    QCPBarsGroup *group = new QCPBarsGroup(this->ui->wYearPlot);
    QCPBars *bars;
    int j=0;

    for (int i=0; i<allEnergyData.size(); i++) {
        QVector<double> yearData = allEnergyData.at(i);
        bars = new QCPBars(this->ui->wYearPlot->xAxis, this->ui->wYearPlot->yAxis);
        this->ui->wYearPlot->addPlottable(bars);
        bars->setData(months, yearData);
        QColor c = QColor(this->someColors.at(j));
        bars->setPen(c);
        c.setAlpha(50);
        bars->setBrush(c);
        bars->addToLegend();
        bars->setWidth(2/(3*(1+(float)sp.getEndingDate().year()-sp.getBeginningDate().year())));
        bars->setBarsGroup(group);
        bars->setName(QString::number(sp.getBeginningDate().year()+i));
        j++;
        if(j == someColors.size()) {
            j=0;
        }
    }

    this->ui->wYearPlot->xAxis->setRange(0, 13);
    this->ui->wYearPlot->yAxis->setRange(0, 2000);
    this->ui->wYearPlot->xAxis->setTickVector(months);
    this->ui->wYearPlot->xAxis->setTickVectorLabels(labels);
    this->ui->wYearPlot->xAxis->setAutoTickStep(false);
    this->ui->wYearPlot->xAxis->setTickStep(1);

    this->ui->wYearPlot->legend->setVisible(true);
    this->ui->wYearPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    this->ui->wYearPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    this->ui->wYearPlot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    this->ui->wYearPlot->legend->setFont(legendFont);

    this->ui->wTotalPlot->setInteractions(QCP::iRangeZoom);


    this->ui->wYearPlot->replot();
}

void QSolanizer::plotTotalData()
{
    this->ui->wTotalPlot->clearPlottables();
    // get data
    QList<QVector<double> > allEnergyData;
    QVector<double> years;
    QVector<QString> labels;

    QList<QColor> colors;
    colors.append(QColor(129, 227, 227)); //january
    colors.append(QColor(104, 176, 176)); // february
    colors.append(QColor(129, 255, 125)); // march
    colors.append(QColor(59, 255, 60)); //april
    colors.append(QColor(38, 189, 60)); // may
    colors.append(QColor(230, 250, 7)); // june
    colors.append(QColor(250, 137, 7)); // july
    colors.append(QColor(252, 75, 5)); // august
    colors.append(QColor(252, 129, 5)); // september
    colors.append(QColor(196, 98, 0)); //october
    colors.append(QColor(133, 70, 7)); // november
    colors.append(QColor(45, 45, 45)); // december

    for (int i=sp.getBeginningDate().year(); i<=sp.getEndingDate().year(); i++) {
        years << i;
        labels << QString::number(i);
    }

    for (int i=1; i<=12; i++) {
        QVector<double> monthData;
        for (int j=sp.getBeginningDate().year(); j<=sp.getEndingDate().year(); j++) {
            Year* year = sp.getYear(j);
            if (year->hasDataOfMonth(i)) {
                monthData << year->getMonth(i)->getEnergy();
            } else {
                monthData << 0;
            }
        }
        allEnergyData << monthData;
    }

    QList<QCPBars*> bars;
    QPen pen;
    pen.setWidthF(1.2);
    for (int i=1; i<=12; i++) {
        QCPBars *bar = new QCPBars(this->ui->wTotalPlot->xAxis, this->ui->wTotalPlot->yAxis);
        bar->setName(QDate(2015, i, 1).toString("MMMM"));
        pen.setColor(colors.at(i-1));
        bar->setPen(pen);
        bar->addToLegend();
        QColor color = QColor(colors.at(i-1));
        color.setAlpha(150);
        bar->setBrush(color);
        bar->setData(years , allEnergyData.at(i-1));
        if (!bars.isEmpty()) {
            bar->moveAbove(bars.at(bars.size()-1));
        }
        bars.append(bar);
    }

    this->ui->wTotalPlot->xAxis->setAutoTicks(false);
    this->ui->wTotalPlot->xAxis->setAutoTickLabels(false);
    this->ui->wTotalPlot->xAxis->setTickVector(years);
    this->ui->wTotalPlot->xAxis->setTickVectorLabels(labels);
    this->ui->wTotalPlot->xAxis->setSubTickCount(0);
    this->ui->wTotalPlot->xAxis->setRange(years.at(0)-1, years.at(years.size()-1)+2); //+2 to have space for the legend

    this->ui->wTotalPlot->yAxis->setRange(0, 14000);
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0,0,0,25));
    this->ui->wTotalPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    this->ui->wTotalPlot->yAxis->grid()->setSubGridPen(gridPen);

    // create legend
    this->ui->wTotalPlot->legend->setVisible(true);
    this->ui->wTotalPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    this->ui->wTotalPlot->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    this->ui->wTotalPlot->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    this->ui->wTotalPlot->legend->setFont(legendFont);

    this->ui->wTotalPlot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);

    this->ui->wTotalPlot->replot();

     // set group box data
    this->ui->lTotalBegin->setText(sp.getBeginningDate().toString("dd.MM.yyyy"));
    this->ui->lTotalEnd->setText(sp.getEndingDate().toString("dd.MM.yyyy"));
    this->ui->lTotalDuration->setText(QString("%1 h").arg(sp.getDuration(),0,'f',0));
    this->ui->lTotalEnergy->setText(QString("%1 kWh").arg(sp.getEnergy(),0,'f',0));
    this->ui->lTotalData->setText(QString::number(sp.getDayCount()));
}

void QSolanizer::on_tMonthSelection_itemSelectionChanged()
{
    QList<QTreeWidgetItem *> selectedItems = this->ui->tMonthSelection->selectedItems();
    if (selectedItems.size() == 1) {
        if (selectedItems.at(0)->childCount() == 0) {
            int year = selectedItems.at(0)->parent()->text(0).toInt();
            QDate month = QDate::fromString(selectedItems.at(0)->text(0), "MMMM");
            month.setDate(year, month.month(), 1);
            QDate nextMonth = month.addMonths(1);
            QDate lastDayOfMonth = nextMonth.addDays(-1);
            if (month >= sp.getBeginningDate()) {
                this->ui->dateEditStart->setDate(month);
            } else {
                this->ui->dateEditStart->setDate(sp.getBeginningDate());
            }

            if (lastDayOfMonth <= sp.getEndingDate()) {
                this->ui->dateEditEnd->setDate(lastDayOfMonth);
            } else {
                this->ui->dateEditEnd->setDate(sp.getEndingDate());
            }
            this->showMonthData(month);
        }
    }
}

void QSolanizer::on_listWidget_itemSelectionChanged()
{
    QList<QListWidgetItem *> selectedItems = this->ui->listWidget->selectedItems();
    if (selectedItems.size() == 1) {
        this->plotYearData(selectedItems.at(0)->text().toInt());
    }
}

void QSolanizer::on_checkBox_stateChanged(int checkState)
{
    if (checkState == Qt::Checked) {
        this->ui->listWidget->setEnabled(false);
        this->plotAllYearData();

    } else {
        this->ui->listWidget->setEnabled(true);
        this->on_listWidget_itemSelectionChanged();
    }
}

void QSolanizer::on_dateEdit_dateChanged(const QDate &date)
{
    this->plotDayData(date);
}

void QSolanizer::on_dateEditStart_dateChanged(const QDate &date)
{
    this->ui->dateEditEnd->setMinimumDate(date);
    this->showCustomRange(this->ui->dateEditStart->date(), this->ui->dateEditEnd->date());
}

void QSolanizer::on_dateEditEnd_dateChanged(const QDate &date)
{
    this->ui->dateEditStart->setMaximumDate(date);
    this->showCustomRange(this->ui->dateEditStart->date(), this->ui->dateEditEnd->date());
}
