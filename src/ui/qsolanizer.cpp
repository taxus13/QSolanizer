#include "qsolanizer.h"
#include "ui_qsolanizer.h"


QSolanizer::QSolanizer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QSolanizer)
{
    ui->setupUi(this);
    this->initializeVariables();
    this->readSettings();
    if (this->readSerializedData()) {
        // if it is possible to load data, show it
        this->fillDataWidgets();
    } else {
        // present empty UI
        this->disableAllInputWidgets();
    }
}

QSolanizer::~QSolanizer()
{
    delete ui;
}

void QSolanizer::initializeVariables()
{
    this->filename = "qsolanizer.dat";
    // first color list for years
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

    //second color list for different days
    this->dayColors.append(Qt::blue);
    this->dayColors.append(Qt::red);
    this->dayColors.append(Qt::darkGray);
    this->dayColors.append(Qt::darkYellow);
    this->dayColors.append(Qt::darkGreen);
    this->dayColors.append(Qt::black);
    this->dayColors.append(Qt::magenta);
    this->dayColors.append(Qt::darkCyan);
}

void QSolanizer::readSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QString("lostbit"), QString("QSolanizer"));

    settings.beginGroup("Window");
    bool isMaximized = settings.value("maximized", false).toBool();
    if (isMaximized) {
        this->showMaximized();
    } else {
        this->resize(settings.value("size", QSize(1000, 600)).toSize());
        this->move(settings.value("pos", QPoint(100, 100)).toPoint());
    }
    int tabNo = settings.value("tab", 0).toInt();
    if ((tabNo < 0) || (tabNo >= this->ui->tabWidget->count())) {
        tabNo = 0;
    }
    this->ui->tabWidget->setCurrentIndex(tabNo);
    settings.endGroup();
    settings.beginGroup("Path");
    this->path = settings.value("path", "--").toString();
    settings.endGroup();

}

void QSolanizer::writeSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QString("lostbit"), QString("QSolanizer"));

    settings.beginGroup("Window");
    settings.setValue("maximized", this->isMaximized());
    settings.setValue("size", this->size());
    settings.setValue("pos", this->pos());
    settings.setValue("tab", this->ui->tabWidget->currentIndex());
    settings.endGroup();
    settings.beginGroup("Path");
    settings.setValue("path", this->path);
    settings.endGroup();
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
        Year year = sp.getYear(yearNumber);
        foreach (QDate month, year.getMonthList()) {
            QTreeWidgetItem *monthItem = new QTreeWidgetItem();
            monthItem->setText(0, month.toString("MMMM"));
            treeItem->addChild(monthItem);
        }
        this->ui->tMonthSelection->addTopLevelItem(treeItem);
        //fill year list
        this->ui->listWidget->addItem(QString::number(yearNumber));
    }

    this->ui->listWidget->setMinimumWidth(this->ui->listWidget->sizeHintForColumn(0));
    // show some data, so the plots are not empty
    this->plotDayData(sp.getEndingDate(), false);
    this->showMonthData(sp.getEndingDate());

    this->plotYearData(sp.getEndingDate().year());
    this->plotTotalData();

    this->ui->tMonthSelection->expandAll();

}

void QSolanizer::disableAllInputWidgets()
{
    // tab "day"
    this->ui->calendarWidget->setEnabled(false);
    this->ui->dateEdit->setEnabled(false);
    this->ui->cMultpleChoice->setEnabled(false);
    this->ui->bReset->setEnabled(false);

    //tab "month"
    this->ui->tMonthSelection->setEnabled(false);
    this->ui->dateEditStart->setEnabled(false);
    this->ui->dateEditEnd->setEnabled(false);
    this->ui->rDistribution->setEnabled(false);
    this->ui->rEnergy->setEnabled(false);

    // tab "year"
    this->ui->listWidget->setEnabled(false);
    this->ui->cCompareYears->setEnabled(false);

}

void QSolanizer::enableAllInputWidgets()
{
    // tab "day"
    this->ui->calendarWidget->setEnabled(true);
    this->ui->dateEdit->setEnabled(true);
    this->ui->cMultpleChoice->setEnabled(true);
    this->ui->bReset->setEnabled(true);

    //tab "month"
    this->ui->tMonthSelection->setEnabled(true);
    this->ui->dateEditStart->setEnabled(true);
    this->ui->dateEditEnd->setEnabled(true);
    this->ui->rDistribution->setEnabled(true);
    this->ui->rEnergy->setEnabled(true);

    // tab "year"
    this->ui->listWidget->setEnabled(true);
    this->ui->cCompareYears->setEnabled(true);
}

bool QSolanizer::getProperDir(bool changeDir)
{
    if (!QDir(this->path).exists() || changeDir) {
            QString tempPath;
        tempPath = QFileDialog::getExistingDirectory(0, QString("Ordner mit den CSV-Dateien auswählen"), QDir::homePath());
        if (tempPath.isEmpty()) { // FileDialog abortet
            return false;
        } else {
            this->path = tempPath;
        }
    }
    return true;
}

bool QSolanizer::readData() {
    if (QDir(this->path).exists()) {
        QStringList files = CSVReader::getFileList(this->path);
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
            qDebug() << " got results";
            sp.doFinalStatistics();
            qDebug() << sp.getDayCount() << " days read";
            return true;
        } else {
            QMessageBox::critical(this, "Abgebrochen", "Sie haben den Vorgang abgebrochen!");
            return false;
        }
    }
    return false;

}

bool QSolanizer::readSerializedData()
{
    QString path = QDir(this->path).filePath(this->filename);
    QFile file(path);
    qDebug() << path;
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        in >> this->sp;
        qDebug() << "finished reading";
        qDebug() << "read " << sp.getDayCount() << " days";
        file.close();
        return true;
    } else {
        file.close();
        return false;
    }

}

void QSolanizer::writeSerializedData()
{
    QString path = QDir(this->path).filePath(this->filename);
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out << this->sp;
    }
    file.close();
}

void QSolanizer::plotDayData(QDate date, bool keepOldGraphs)
{
    if (!keepOldGraphs) {
        resetDayPlot();
    }
    // maybe move this into day-class
    Day dd = sp.getDay(date);

    QVector<double> xTicks;
    QVector<QString> xLabel;
    for (int i=0; i<=24; i++) {
        xTicks << i * 1000 * 3600;
        xLabel << QString::number(i, 'f', 0) + ":00";
    }

    QVector<double> timeline;
    QVector<double> power;

    QMap<QDateTime, float> powerCurve = dd.getPowerCurve();
    QMap<QDateTime, float>::iterator i;
    for (i = powerCurve.begin(); i != powerCurve.end(); ++i) {
        timeline.append(i.key().time().msecsSinceStartOfDay());
        power.append(i.value());
    }

    int colorKey = ui->wPowerCurve->graphCount() % this->dayColors.size();
    QColor color = this->dayColors.at(colorKey);
    QColor colorAlpha = color;
    colorAlpha.setAlpha(20);


    ui->wPowerCurve->addGraph();
    ui->wPowerCurve->graph()->setData(timeline, power);
    ui->wPowerCurve->graph()->setPen(QPen(color)); // Qt::blue
    ui->wPowerCurve->graph()->setBrush(QBrush(colorAlpha));
    ui->wPowerCurve->graph()->addToLegend();
    ui->wPowerCurve->graph()->setName(dd.getDate().toString("dd.MM.yyyy"));

    ui->wPowerCurve->xAxis->setRange(0, 24 * 3600 * 1000);
    ui->wPowerCurve->xAxis->setAutoTickStep(false);
    ui->wPowerCurve->xAxis->setAutoTicks(false);
    ui->wPowerCurve->xAxis->setAutoTickLabels(false);
    ui->wPowerCurve->xAxis->setTickVector(xTicks);
    ui->wPowerCurve->xAxis->setTickVectorLabels(xLabel);
    ui->wPowerCurve->xAxis->setSubTickCount(6); // every 10 minutes
    ui->wPowerCurve->xAxis->setTickLabelRotation(60);

    ui->wPowerCurve->yAxis->setLabel("Leistung [kW]");

    ui->wPowerCurve->setAntialiasedElement(QCP::aePlottables);

    ui->wPowerCurve->yAxis->setRange(0, 15);

    ui->wPowerCurve->legend->setVisible(true);
    ui->wPowerCurve->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    ui->wPowerCurve->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    ui->wPowerCurve->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->wPowerCurve->legend->setFont(legendFont);


    ui->wPowerCurve->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->wPowerCurve->replot();

    // fill groupbox
    ui->lDayDuration->setText(QString("%1 h").arg(dd.getDuration(),0, 'f', 2));
    ui->lDayEnergy->setText(QString("%1 kWh").arg(dd.getEnergy(),0,'f',2));
    ui->lPeakpower->setText(QString("%1 kW").arg(dd.getMaximumPower(), 0, 'f', 2));
    ui->lSunrise->setText(dd.getSunrise().toString("HH:mm"));
    ui->lSunset->setText(dd.getSunset().toString("HH:mm"));
}


void QSolanizer::resetDayPlot()
{
    this->ui->wPowerCurve->clearGraphs();
    this->ui->wPowerCurve->replot();
}


void QSolanizer::showMonthData(QDate date) {
    Month month = sp.getMonth(date);
    QPair<QVector<QDate>, QVector<float> > data = month.getEnergyValues();
    this->plotDailyEnergyValues(data);

    // fill group box data
    this->ui->lMonthEnergy->setText(QString("%1 kWh").arg(month.getEnergy(),0,'f',2));
    this->ui->lMonthSunhours->setText(QString("%1 h").arg(month.getDuration(),0,'f',2));
}

void QSolanizer::showCustomRange(QDate start, QDate end)
{
    this->ui->lMonthEnergy->setText(QString("%1 kWh").arg(sp.getEnergyInRange(start, end),0,'f',2));
    this->ui->lMonthSunhours->setText(QString("%1 h").arg(sp.getSunhoursInRange(start, end),0,'f',2));
    if (this->ui->rEnergy->isChecked()) {
        QPair<QVector<QDate>, QVector<float> > data = sp.getEnergyValuesOfDays(start, end);
        this->plotDailyEnergyValues(data);
    } else {
        QVector<QList<QDateTime> > data = sp.getSignificantTimes(start, end);
        this->plotDailyDistribution(data);
    }
}

void QSolanizer::plotDailyEnergyValues(QPair<QVector<QDate>, QVector<float> > &data)
{
    QCPBars *bars = new QCPBars(this->ui->wMonthPlot->xAxis, this->ui->wMonthPlot->yAxis);
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
    for (int i=0; i<data.first.size(); ++i) {
        ticks << i+1;
        labels << data.first.at(i).toString("dd.MM.");
        values << data.second.at(i);
    }
    if (data.first.size() <= 31) {
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
    }

    this->ui->wMonthPlot->xAxis->setSubTickCount(0);
    this->ui->wMonthPlot->xAxis->setTickLength(0,4);
    this->ui->wMonthPlot->xAxis->grid()->setVisible(false);
    this->ui->wMonthPlot->xAxis->setRange(0, ticks.size()+1);
    this->ui->wMonthPlot->yAxis->setRange(0, 100);
    this->ui->wMonthPlot->yAxis->setPadding(5);
    this->ui->wTotalPlot->yAxis->setAutoTicks(true);
    this->ui->wTotalPlot->yAxis->setAutoTickLabels(true);
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

void QSolanizer::plotDailyDistribution(QVector<QList<QDateTime> > &data)
{
    this->ui->wMonthPlot->clearPlottables();

    //QVector<QCPStatisticalBox *> boxes;
    QVector<QString> xLabel;
    QVector<double> xTicks;
    QVector<double> yTicks;
    QVector<QString> yLabel;
    qDebug() << "highest energy" << this->sp.getHighestDayEnergy();

    for (int i=0; i<=24; i++) {
        yTicks << i * 1000 * 3600;
        yLabel << QString::number(i, 'f', 0) + ":00";
    }

    for (int i=0; i<data.size(); i++) {
        xTicks << i;
        xLabel << data.at(i).at(0).toString("dd.MM.yyyy");
        QCPStatisticalBox *box = new QCPStatisticalBox(this->ui->wMonthPlot->xAxis, this->ui->wMonthPlot->yAxis);
        this->ui->wMonthPlot->addPlottable(box);

        QDate date =data.at(i).at(1).date();

        float percent = this->sp.getDay(date).getEnergy() / this->sp.getHighestDayEnergy();

        QBrush boxBrush(QColor(255 * percent, 255 * (1-percent), 0, 150));
        box->setBrush(boxBrush);
        box->setKey(i);
        box->setMinimum(data.at(i).at(0).time().msecsSinceStartOfDay());
        box->setLowerQuartile(data.at(i).at(1).time().msecsSinceStartOfDay());
        box->setMedian(data.at(i).at(2).time().msecsSinceStartOfDay());
        box->setUpperQuartile(data.at(i).at(3).time().msecsSinceStartOfDay());
        box->setMaximum(data.at(i).at(4).time().msecsSinceStartOfDay());
        box->setWidth(.9);
    }

    this->ui->wMonthPlot->rescaleAxes();
    this->ui->wMonthPlot->xAxis->setSubTickCount(0);
    this->ui->wMonthPlot->xAxis->setAutoTickLabels(false);
    this->ui->wMonthPlot->xAxis->setAutoTicks(false);
    this->ui->wMonthPlot->xAxis->setTickVector(xTicks);
    this->ui->wMonthPlot->xAxis->setTickVectorLabels(xLabel);
    this->ui->wMonthPlot->yAxis->setLabel("");

    this->ui->wMonthPlot->yAxis->setAutoTickLabels(false);
    this->ui->wMonthPlot->yAxis->setAutoTicks(false);
    this->ui->wMonthPlot->yAxis->setTickVector(yTicks);
    this->ui->wMonthPlot->yAxis->setTickVectorLabels(yLabel);
    this->ui->wMonthPlot->yAxis->setSubTickCount(4);
    this->ui->wMonthPlot->yAxis->setRange(0, 24* 1000 * 3600);
    this->ui->wMonthPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    this->ui->wMonthPlot->replot();
}

void QSolanizer::plotYearData(int yearNumber)
{
    Year year = sp.getYear(yearNumber);
    QPair<QVector<QDate>, QVector<float> > data = year.getEnergyValues();
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
    this->ui->lYearDuration->setText(QString("%1 h").arg(year.getDuration(),0,'f',0));
    this->ui->lYearEnergy->setText(QString("%1 kWh").arg(year.getEnergy(),0,'f',0));

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
        Year year = sp.getYear(i);
        QVector<double> yearEnergy;
        foreach (double month, months) {
            if (year.hasDataOfMonth((int)month)) {
                yearEnergy << year.getMonth((int)month).getEnergy();
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
    // workarond, since clearPlottables didnt do anything
    QCustomPlot *temp = new QCustomPlot();
    this->ui->horizontalLayout_8->replaceWidget(this->ui->wTotalPlot, temp);
    delete this->ui->wTotalPlot;
    this->ui->wTotalPlot = temp;

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
            Year year = sp.getYear(j);
            if (year.hasDataOfMonth(i)) {
                monthData << year.getMonth(i).getEnergy();
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

void QSolanizer::closeEvent(QCloseEvent *event)
{
    this->writeSettings();
    event->accept();
}

void QSolanizer::on_calendarWidget_selectionChanged()
{
    QDate date = this->ui->calendarWidget->selectedDate();
    this->plotDayData(date, this->ui->cMultpleChoice->checkState() == Qt::Checked);

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

            QDate max;
            QDate min;

            month >= sp.getBeginningDate() ? min = month : min = sp.getBeginningDate();
            lastDayOfMonth <= sp.getEndingDate() ? max = lastDayOfMonth : max = sp.getEndingDate();

            qDebug() << min << max;

            this->ui->dateEditStart->setMaximumDate(max);
            this->ui->dateEditEnd->setMinimumDate(min);
            this->ui->dateEditStart->setDate(min);
            this->ui->dateEditEnd->setDate(max);

            this->showCustomRange(min, max);
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

void QSolanizer::on_dateEdit_dateChanged(const QDate &date)
{
    this->plotDayData(date, this->ui->cCompareYears->checkState() == Qt::Checked);
}

void QSolanizer::on_dateEditStart_userDateChanged(const QDate &date)
{
    if (this->ui->dateEditStart->hasFocus()) {
        this->ui->dateEditEnd->setMinimumDate(date);
        this->showCustomRange(date, this->ui->dateEditEnd->date());
    }
}

void QSolanizer::on_dateEditEnd_userDateChanged(const QDate &date)
{
    if (this->ui->dateEditEnd->hasFocus()) {
        this->ui->dateEditStart->setMaximumDate(date);
        this->showCustomRange(this->ui->dateEditStart->date(), date);
    }
}

void QSolanizer::on_rEnergy_toggled(bool checked)
{
    this->showCustomRange(this->ui->dateEditStart->date(), this->ui->dateEditEnd->date());
}

void QSolanizer::on_cMultpleChoice_toggled(bool checked)
{
    if (!checked) {
        this->resetDayPlot();
    }
}

void QSolanizer::on_bReset_clicked()
{
    this->resetDayPlot();
}

void QSolanizer::on_actionReload_triggered()
{
    if (this->getProperDir(false)) {
        if (this->readData()) {
            this->fillDataWidgets();
            this->writeSerializedData();
            this->enableAllInputWidgets();
        }
    }
}

void QSolanizer::on_actionOpenNew_triggered()
{
    if (this->getProperDir(true)) {
        if (this->readSerializedData() || this->readData()) {
            this->fillDataWidgets();
            this->writeSerializedData();
            this->enableAllInputWidgets();
        }
    }
}

void QSolanizer::on_actionClose_triggered()
{
    this->close();
}

void QSolanizer::on_actionAbout_triggered()
{
    QMessageBox::about(this, "Über", "QSolanizer 0.9 (beta)");
}

void QSolanizer::on_cCompareYears_stateChanged(int checkState)
{
    if (checkState == Qt::Checked) {
        this->ui->listWidget->setEnabled(false);
        this->plotAllYearData();

    } else {
        this->ui->listWidget->setEnabled(true);
        this->on_listWidget_itemSelectionChanged();
    }
}
