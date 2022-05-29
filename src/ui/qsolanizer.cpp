#include "qsolanizer.h"
#include "ui_qsolanizer.h"

#include <cmath>
#include <QSharedPointer>

QSolanizer::QSolanizer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QSolanizer)
{
    ui->setupUi(this);
    this->initializeVariables();
    this->readSettings();
    if (this->readSerializedData()) {
        // if it is possible to load data, show it
        this->initializePlots();
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
    this->version = QString("1.0.0");
    this->filename = "qsolanizer.dat";
    this->propertyname = "qsolanizer.ini";

    this->dataSuccessfullyLoaded = false;
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
    this->dayColors.append(Qt::green);
    this->dayColors.append(Qt::cyan);
    this->dayColors.append(Qt::magenta);
    this->dayColors.append(Qt::yellow);
    this->dayColors.append(Qt::gray);

    this->dayColorsDark.append(Qt::darkBlue);
    this->dayColorsDark.append(Qt::darkRed);
    this->dayColorsDark.append(Qt::darkGreen);
    this->dayColorsDark.append(Qt::darkCyan);
    this->dayColorsDark.append(Qt::darkMagenta);
    this->dayColorsDark.append(Qt::darkYellow);
    this->dayColorsDark.append(Qt::darkGray);

    this->yearColors.append(QColor(129, 227, 227)); //january
    this->yearColors.append(QColor(104, 176, 176)); // february
    this->yearColors.append(QColor(129, 255, 125)); // march
    this->yearColors.append(QColor(59, 255, 60)); //april
    this->yearColors.append(QColor(38, 189, 60)); // may
    this->yearColors.append(QColor(230, 250, 7)); // june
    this->yearColors.append(QColor(250, 137, 7)); // july
    this->yearColors.append(QColor(252, 75, 5)); // august
    this->yearColors.append(QColor(252, 129, 5)); // september
    this->yearColors.append(QColor(196, 98, 0)); //october
    this->yearColors.append(QColor(133, 70, 7)); // november
    this->yearColors.append(QColor(45, 45, 45)); // december

    this->minEnergyColor = Qt::white;
    this->maxEnergyColor = Qt::yellow;

    for (int i=0; i<=24; i++) {
        dayTicksMSecs << i * 1000 * 3600;
        dayLabelMSecs << QString::number(i, 'f', 0) + ":00";
    }

    this->lastClickedItem = 0;

    connect(ui->wMonthPlot, SIGNAL(plottableClick(QCPAbstractPlottable*, int, QMouseEvent*)), this, SLOT(monthItemClicked(QCPAbstractPlottable*, int, QMouseEvent*)));
    connect(ui->wYearPlot, &QCustomPlot::plottableClick, this, &QSolanizer::yearItemClicked);
    connect(ui->wPowerCurve, &QCustomPlot::legendClick, this, &QSolanizer::legendItemClicked);
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
    bool showEnergy = settings.value("showEnergy", true).toBool();
    if (showEnergy) {
        this->ui->rEnergy->setChecked(true);
    } else {
        this->ui->rDistribution->setChecked(true);
    }
    this->ui->cRealCurve->setChecked(settings.value("showRealCurve", true).toBool());
    this->ui->cTheoreticalCurve->setChecked(settings.value("showTheoCurve", false).toBool());
    this->ui->cAvergeageCurve->setChecked(settings.value("showAvgCurve", false).toBool());

    this->ui->tabWidget->setCurrentIndex(tabNo);
    settings.endGroup();
    settings.beginGroup("Path");
    this->path = settings.value("path", "--").toString();
    settings.endGroup();
    settings.beginGroup("Colors");
    QString minEC = settings.value("minEnergyColor", QColor(Qt::white).name()).toString();
    QString maxEC = settings.value("maxEnergyColor", QColor(Qt::yellow).name()).toString();
    this->minEnergyColor = QColor(minEC);
    this->maxEnergyColor = QColor(maxEC);
    settings.endGroup();

    sp.getSolarPlantProperties().readProperties(QDir(this->path).absoluteFilePath(this->propertyname));
}

void QSolanizer::initializePlots()
{
    // day plot
    ui->wPowerCurve->xAxis->setRange(0, 24 * 3600);

    QSharedPointer<QCPAxisTickerTime> time_ticker(new QCPAxisTickerTime);
    time_ticker->setTimeFormat("%h:%m");
    ui->wPowerCurve->xAxis->setTicker(time_ticker);
    ui->wPowerCurve->xAxis->setTickLabelRotation(60);
    ui->wPowerCurve->xAxis->setSubTicks(true);

    ui->wPowerCurve->yAxis->setLabel("Leistung / kW");

    ui->wPowerCurve->setAntialiasedElement(QCP::aePlottables);

    ui->wPowerCurve->yAxis->setRange(0, sp.getHighestPower()*1.1);

    ui->wPowerCurve->legend->setVisible(true);
    ui->wPowerCurve->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    ui->wPowerCurve->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen(QColor(130, 130, 130, 200));

    ui->wPowerCurve->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->wPowerCurve->legend->setFont(legendFont);


    ui->wPowerCurve->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->wPowerCurve->axisRect()->setRangeZoom(Qt::Horizontal);
    ui->wPowerCurve->axisRect()->setRangeDrag(Qt::Horizontal);


    // month plot
    QSharedPointer<QCPAxisTickerDateTime> datetime_ticker(new QCPAxisTickerDateTime);
    datetime_ticker->setDateTimeFormat("dd.MM.yyyy");
    this->ui->wMonthPlot->xAxis->setTicker(datetime_ticker);
    this->ui->wMonthPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    this->ui->wMonthPlot->axisRect()->setRangeZoom(Qt::Horizontal);
    this->ui->wMonthPlot->axisRect()->setRangeDrag(Qt::Horizontal);

    //year plot
    this->ui->wYearPlot->xAxis->setRange(0, 13);
    this->ui->wYearPlot->yAxis->setRange(0, sp.getHighestMonthEnergy()*1.1);
    this->ui->wYearPlot->yAxis->setPadding(5);
    this->ui->wYearPlot->yAxis->grid()->setSubGridVisible(true);
    this->ui->wYearPlot->yAxis->setLabel("Energie  / kWh");

    this->ui->wYearPlot->legend->setVisible(true);
    this->ui->wYearPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    this->ui->wYearPlot->legend->setBrush(QColor(255, 255, 255, 200));
    this->ui->wYearPlot->legend->setBorderPen(legendPen);
    legendFont.setPointSize(10);
    this->ui->wYearPlot->legend->setFont(legendFont);
    QVector<double> months;
    QVector<QString> labels;

    //generate ticklabels and tickvector
    for (int i=1; i<=12; i++) {
        months << i;
        labels << QDate::fromString(QString::number(i), "M").toString("MMMM");
    }

    QSharedPointer<QCPAxisTickerText> month_ticker(new QCPAxisTickerText);
    month_ticker->addTicks(months, labels);
    this->ui->wYearPlot->xAxis->setTicker(month_ticker);
}

void QSolanizer::writeSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QString("lostbit"), QString("QSolanizer"));

    settings.beginGroup("Window");
    settings.setValue("maximized", this->isMaximized());
    settings.setValue("size", this->size());
    settings.setValue("pos", this->pos());
    settings.setValue("tab", this->ui->tabWidget->currentIndex());
    settings.setValue("showEnergy", this->ui->rEnergy->isChecked());
    settings.setValue("showRealCurve", this->ui->cRealCurve->isChecked());
    settings.setValue("showTheoCurve", this->ui->cTheoreticalCurve->isChecked());
    settings.setValue("showAvgCurve", this->ui->cAvergeageCurve->isChecked());
    settings.endGroup();

    settings.beginGroup("Path");
    settings.setValue("path", this->path);
    settings.endGroup();

    settings.beginGroup("Colors");
    settings.setValue("minEnergyColor", this->minEnergyColor.name());
    settings.setValue("maxEnergyColor", this->maxEnergyColor.name());
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
    QList<int> yearList = sp.getYearList();
    for (int yearNumber : yearList) {
        QTreeWidgetItem *treeItem = new QTreeWidgetItem(this->ui->tMonthSelection);
        treeItem->setText(0, QString::number(yearNumber));
        Year year = sp.getYear(yearNumber);
        QList<QDate> monthListByYear = year.getMonthList();
        for (const QDate& month : monthListByYear) {
            QTreeWidgetItem *monthItem = new QTreeWidgetItem();
            monthItem->setText(0, month.toString("MMMM"));
            treeItem->addChild(monthItem);
            if ((yearNumber == sp.getEndingDate().year()) && (month.month() == sp.getEndingDate().month())) {
                showMonthData(month);
            }
        }
        this->ui->tMonthSelection->addTopLevelItem(treeItem);
        //fill year list
        QListWidgetItem *listItem = new QListWidgetItem(QString::number(yearNumber));
        listItem->setTextAlignment(Qt::AlignCenter);
        this->ui->listWidget->addItem(listItem);
    }

    this->ui->listWidget->setFixedWidth(100);
    // show some data, so the plots are not empty
    this->plotDayData(sp.getEndingDate(), false, this->getCurrentPlottingMode());
    this->shownDates.append(sp.getEndingDate());

    this->plotYearData(sp.getEndingDate().year());
    this->plotTotalData();

    this->ui->tMonthSelection->expandAll();
    this->ui->wColorScale->setFixedHeight(180);
    this->ui->wColorScaleParent->setFixedHeight(200);
    this->drawColorScale();
    this->ui->wColorScale->setStyleSheet("border: 1px solid black");
    this->ui->lMaxEnergy->setText(QString("%1 kWh").arg(this->sp.getHighestDayEnergy(),0,'f',1));
    this->ui->lHalfEnergy->setText(QString("%1 kWh").arg(this->sp.getHighestDayEnergy()/2,0,'f',1));
    this->ui->lMinEnergy->setText(QString("%1 kWh").arg(0.0, 0,'f',1));
}

void QSolanizer::disableAllInputWidgets()
{
    // tab "day"
    this->ui->calendarWidget->setEnabled(false);
    this->ui->dateEdit->setEnabled(false);
    this->ui->cMultpleChoice->setEnabled(false);
    this->ui->bReset->setEnabled(false);
    this->ui->gPlottingMode->setEnabled(false);

    //tab "month"
    this->ui->tMonthSelection->setEnabled(false);
    this->ui->dateEditStart->setEnabled(false);
    this->ui->dateEditEnd->setEnabled(false);
    this->ui->rDistribution->setEnabled(false);
    this->ui->rEnergy->setEnabled(false);

    // tab "year"
    this->ui->listWidget->setEnabled(false);
    this->ui->cCompareYears->setEnabled(false);

    // menu
    this->ui->actionSolarPlantProperties->setEnabled(false);
    this->ui->actionReload->setEnabled(false);

}

void QSolanizer::enableAllInputWidgets()
{
    // tab "day"
    this->ui->calendarWidget->setEnabled(true);
    this->ui->dateEdit->setEnabled(true);
    this->ui->cMultpleChoice->setEnabled(true);
    this->ui->bReset->setEnabled(true);
    this->ui->gPlottingMode->setEnabled(true);

    //tab "month"
    this->ui->tMonthSelection->setEnabled(true);
    this->ui->dateEditStart->setEnabled(true);
    this->ui->dateEditEnd->setEnabled(true);
    this->ui->rDistribution->setEnabled(true);
    this->ui->rEnergy->setEnabled(true);

    // tab "year"
    this->ui->listWidget->setEnabled(true);
    this->ui->cCompareYears->setEnabled(true);

    // menu
    this->ui->actionSolarPlantProperties->setEnabled(true);
    this->ui->actionReload->setEnabled(true);
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
    this->dataSuccessfullyLoaded = false;
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
            qDebug() << "got results";
            sp.doFinalStatistics();
            qDebug() << sp.getDayCount() << "days read";
            this->dataSuccessfullyLoaded = true;
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
    this->dataSuccessfullyLoaded = false;
    QString path = QDir(this->path).filePath(this->filename);
    QFile file(path);
    qDebug() << path;
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        int version;
        in >> version ;
        if (version == this->fileFormatVersion) {
            in >> this->sp;
            qDebug() << "finished reading";
            qDebug() << "read" << sp.getDayCount() << "days";
            file.close();
            this->dataSuccessfullyLoaded = true;
            return true;
        } else {
            file.close();
            return false;
        }
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
        out << this->fileFormatVersion << this->sp;
    }
    file.close();
}

void QSolanizer::fillDayGroupbox(Day &dd)
{
    ui->lDayDuration->setText(QString("%1 h").arg(dd.getDuration(),0, 'f', 2));
    ui->lDayEnergy->setText(QString("%1 kWh").arg(dd.getEnergy(),0,'f',2));
    ui->lPeakpower->setText(QString("%1 kW").arg(dd.getMaximumPower(), 0, 'f', 2));
    ui->lSunrise->setText(dd.getSunrise().toString("HH:mm"));
    ui->lSunset->setText(dd.getSunset().toString("HH:mm"));

    if (sp.getSolarPlantProperties().getPeakPower() > 0) {
        this->ui->lDayFullLoadHours->setText(QString("%1 h").arg(dd.getEnergy()/sp.getSolarPlantProperties().getPeakPower(), 0, 'f', 1));
    } else {
        this->ui->lDayFullLoadHours->setText(QString("-"));
    }
}

void QSolanizer::plotDayData(QDate date, bool keepOldGraphs, int pm)
{
    if (!keepOldGraphs) {
        resetDayPlot();
    }
    if (this->lastClickedItem) {
        this->lastClickedItem->setSelected(false);
        // this->lastClickedItem->plottable()->setSelected(false);
    }
    int colorKey =  this->currentlyShownDates.size() % this->dayColors.size();

    Day dd = sp.getDay(date);

    if (((pm & REAL) == REAL) && !this->currentlyShownDates.contains(date)) {
        QColor color = this->dayColors.at(colorKey);
        QColor colorAlpha = color;
            colorAlpha.setAlpha(20);
        ui->wPowerCurve->addGraph();
        QPair<QVector<double>, QVector<double> > data = dd.getPowerCurveForPlotting();
        ui->wPowerCurve->graph()->setData(data.first, data.second);
        ui->wPowerCurve->graph()->setPen(QPen(color));
        ui->wPowerCurve->graph()->setBrush(QBrush(colorAlpha));
        ui->wPowerCurve->graph()->addToLegend();
        ui->wPowerCurve->graph()->setName(date.toString("dd.MM.yyyy"));
        ui->wPowerCurve->graph()->setProperty("type", REAL);
        ui->wPowerCurve->graph()->setProperty("date", date);
    }

    if (((pm & THEORETICAL) == THEORETICAL) && !this->currentlyShownDates.contains(date)) {
        QColor color = this->dayColorsDark.at(colorKey);
        QColor colorAlpha = color;
        colorAlpha.setAlpha(20);
        ui->wPowerCurve->addGraph();
        Day d = sp.getSolarPlantProperties().getTheoreticalPowerCurve(date, true);
        QPair<QVector<double>, QVector<double> > data = d.getPowerCurveForPlotting();
        ui->wPowerCurve->graph()->setData(data.first, data.second);
        ui->wPowerCurve->graph()->setPen(QPen(color));
        ui->wPowerCurve->graph()->setBrush(QBrush(colorAlpha));
        ui->wPowerCurve->graph()->addToLegend();
        ui->wPowerCurve->graph()->setName(date.toString("dd.MM.yyyy") + QString(" (theoretisch)"));
        ui->wPowerCurve->graph()->setProperty("type", THEORETICAL);
        ui->wPowerCurve->graph()->setProperty("date", date);
    }

    if (((pm & AVERAGE) == AVERAGE) && !this->currentlyShownAverageMonths.contains(date.month())) {
        ui->wPowerCurve->addGraph();
        QPair<QVector<double>, QVector<double> > data = sp.getAverageDay(date.month()).getPowerCurveForPlotting();
        ui->wPowerCurve->graph()->setData(data.first, data.second);
        QPen pen = QPen(this->yearColors.at(date.month()-1));
        pen.setWidth(3);
        ui->wPowerCurve->graph()->setPen(pen);
        ui->wPowerCurve->graph()->addToLegend();
        ui->wPowerCurve->graph()->setName(date.toString("MMMM") + QString(" (Durchschnitt)"));
        ui->wPowerCurve->graph()->setProperty("type", AVERAGE);
        ui->wPowerCurve->graph()->setProperty("date", date);
    }

    ui->wPowerCurve->replot();


    // fill groupbox
    this->fillDayGroupbox(dd);
    this->currentlyShownDates.insert(date);
    this->currentlyShownAverageMonths.insert(date.month());

}

void QSolanizer::replotDayData(int pm)
{
    this->ui->wPowerCurve->clearGraphs();
    this->ui->wPowerCurve->replot();
    this->lastClickedItem = 0;
    this->currentlyShownDates.clear();
    this->currentlyShownAverageMonths.clear();
    foreach (QDate date, this->shownDates) {
        this->plotDayData(date, true, pm);
    }

}


void QSolanizer::resetDayPlot()
{
    this->ui->wPowerCurve->clearGraphs();
    this->ui->wPowerCurve->replot();
    this->lastClickedItem = 0;
    this->shownDates.clear();
    this->currentlyShownDates.clear();
    this->currentlyShownAverageMonths.clear();
}


void QSolanizer::showMonthData(QDate date) {
    this->startMonthPlot = date;
    Month month = sp.getMonth(date);


    if (this->ui->rEnergy->isChecked()) {
        QPair<QVector<QDate>, QVector<float> > data = month.getEnergyValues();
        this->plotDailyEnergyValues(data);
    } else {
        QDate monthEnd = date.addDays(date.daysInMonth());
        QVector<QList<QDateTime> > data = sp.getSignificantTimes(date, monthEnd);
        this->plotDailyDistribution(data);
    }

    // fill group box data
    this->ui->lMonthEnergy->setText(QString("%1 kWh").arg(month.getEnergy(),0,'f',2));
    this->ui->lMonthSunhours->setText(QString("%1 h").arg(month.getDuration(),0,'f',2));
}

void QSolanizer::showCustomRange(QDate start, QDate end)
{
    this->startMonthPlot = start;
    double energy = sp.getEnergyInRange(start, end);
    this->ui->lMonthEnergy->setText(QString("%1 kWh").arg(energy,0,'f',2));
    this->ui->lMonthSunhours->setText(QString("%1 h").arg(sp.getSunhoursInRange(start, end),0,'f',2));

    if (sp.getSolarPlantProperties().getPeakPower() > 0) {
        this->ui->lMonthFullLoadHours->setText(QString("%1 h").arg(energy/sp.getSolarPlantProperties().getPeakPower(), 0, 'f', 1));
    } else {
        this->ui->lMonthFullLoadHours->setText(QString("-"));
    }

    if (this->ui->rEnergy->isChecked()) {
        QPair<QVector<QDate>, QVector<float>> data = sp.getEnergyValuesOfDays(start, end);
        this->plotDailyEnergyValues(data);
    } else {
        QVector<QList<QDateTime> > data = sp.getSignificantTimes(start, end);
        this->plotDailyDistribution(data);
    }
}

void QSolanizer::plotDailyEnergyValues(QPair<QVector<QDate>, QVector<float> > &data)
{
    this->ui->wMonthPlot->clearPlottables();
    QCPBarsEnhanced *bars = new QCPBarsEnhanced(this->ui->wMonthPlot->xAxis, this->ui->wMonthPlot->yAxis);

    this->ui->wColorScaleParent->setVisible(false);

    bars->setSelectable(QCP::SelectionType::stDataRange);
    QPen pen;
    pen.setWidthF(1.2);
    pen.setColor(QColor(255, 110, 0));
    bars->setPen(pen);
    bars->setBrush(QColor(255, 110, 0, 50));
    QVector<double> dates;
    QVector<double> values;

    for (int i=0; i<data.first.size(); ++i) {
        values << data.second.at(i);
        dates << data.first.at(i).startOfDay().toSecsSinceEpoch();
    }

    this->ui->wMonthPlot->yAxis->ticker().clear();
    QSharedPointer<QCPAxisTickerFixed> ticker(new QCPAxisTickerFixed);
    ticker->setTickCount(15);
    ticker->setTickStep(std::ceil(sp.getHighestDayEnergy()/10));
    this->ui->wMonthPlot->yAxis->setTicker(ticker);

    bars->setWidthType(QCPBars::WidthType::wtPlotCoords);
    bars->setWidth(0.9 * 24 * 3600);
    this->ui->wMonthPlot->xAxis->setTickLabelRotation(60);
    this->ui->wMonthPlot->xAxis->setTickLength(0,4);
    this->ui->wMonthPlot->xAxis->grid()->setVisible(false);
    double halfDay = 23 * 3600;
    this->ui->wMonthPlot->xAxis->setRange(dates.first() - halfDay, dates.last() + halfDay);

    this->ui->wMonthPlot->yAxis->setRange(0, sp.getHighestDayEnergy()*1.1);
    this->ui->wMonthPlot->yAxis->setPadding(5);

    this->ui->wMonthPlot->yAxis->grid()->setSubGridVisible(true);

    this->ui->wMonthPlot->yAxis->setLabel("Energie / kWh");

    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0,0,0,25));
    this->ui->wMonthPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    this->ui->wMonthPlot->yAxis->grid()->setSubGridPen(gridPen);
    bars->setData(dates, values);
    this->ui->wMonthPlot->replot();
}

void QSolanizer::plotDailyDistribution(QVector<QList<QDateTime> > &data)
{
    this->ui->wMonthPlot->clearPlottables();
    this->ui->wColorScaleParent->setVisible(true);

    for (int i=0; i<data.size(); i++) {

        QCPStatisticalBox *box = new QCPStatisticalBox(this->ui->wMonthPlot->xAxis, this->ui->wMonthPlot->yAxis);

        QDate date = data.at(i).at(1).date();

        float percent = this->sp.getDay(date).getEnergy() / this->sp.getHighestDayEnergy();

        int red = this->minEnergyColor.red() + (this->maxEnergyColor.red() - this->minEnergyColor.red()) * percent;
        int green = this->minEnergyColor.green() + (this->maxEnergyColor.green() - this->minEnergyColor.green()) * percent;
        int blue = this->minEnergyColor.blue() + (this->maxEnergyColor.blue() - this->minEnergyColor.blue()) * percent;

        QBrush boxBrush(QColor(red, green, blue));
        box->setBrush(boxBrush);
        double unix_time = date.startOfDay().toSecsSinceEpoch();
        box->addData(unix_time, data.at(i).at(0).time().msecsSinceStartOfDay()/1000, data.at(i).at(1).time().msecsSinceStartOfDay()/1000, data.at(i).at(2).time().msecsSinceStartOfDay()/1000, data.at(i).at(3).time().msecsSinceStartOfDay()/1000, data.at(i).at(4).time().msecsSinceStartOfDay()/1000);
        box->setWidth(0.9 * 24 * 3600);
    }


    this->ui->wMonthPlot->yAxis->ticker().clear();
    QSharedPointer<QCPAxisTickerTime> ticker(new QCPAxisTickerTime);
    ticker->setTimeFormat("%h:%m");
    this->ui->wMonthPlot->yAxis->setTicker(ticker);
    this->ui->wMonthPlot->xAxis->setTickLabelRotation(60);

    this->ui->wMonthPlot->yAxis->setLabel("");
    this->ui->wMonthPlot->yAxis->setRange(0, 24 * 3600);

    double halfDay = 23 * 3600;
    double rangeStart = data.first().at(1).date().startOfDay().toSecsSinceEpoch() - halfDay;
    double rangeEnd = data.last().at(1).date().startOfDay().toSecsSinceEpoch() + halfDay;
    this->ui->wMonthPlot->xAxis->setRange(rangeStart, rangeEnd);
    this->ui->wMonthPlot->replot();
}

void QSolanizer::plotYearData(int yearNumber)
{
    qDebug() << "Plot year " << yearNumber;
    Year year = sp.getYear(yearNumber);
    this->startYearPlot = year.getFirst();
    QPair<QVector<QDate>, QVector<float> > data = year.getEnergyValues();
    this->ui->wYearPlot->clearPlottables();


    QCPBarsEnhanced *bars = new QCPBarsEnhanced(this->ui->wYearPlot->xAxis, this->ui->wYearPlot->yAxis);

    QPen pen;
    pen.setWidthF(1.2);
    bars->setName(QString::number(yearNumber));
    pen.setColor(QColor(255, 131, 0));
    bars->setPen(pen);
    bars->setBrush(QColor(255, 131, 0, 50));

    QVector<double> ticks;

    QVector<double> values;
    for (int i=0; i<data.first.size(); ++i) {
        ticks << data.first.at(i).month();
        values << data.second.at(i);
    }
    this->ui->wYearPlot->xAxis->setTickLabelRotation(60);
    this->ui->wYearPlot->xAxis->setTickLength(0,4);
    this->ui->wYearPlot->xAxis->grid()->setVisible(false);

    QPen gridPen(Qt::SolidLine);
    gridPen.setColor(QColor(0,0,0,25));
    this->ui->wYearPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    this->ui->wYearPlot->yAxis->grid()->setSubGridPen(gridPen);
    bars->setData(ticks, values);
    this->ui->wYearPlot->replot();

    // set group box data
    this->ui->lYearDuration->setText(QString("%1 h").arg(year.getDuration(),0,'f',0));
    this->ui->lYearEnergy->setText(QString("%1 kWh").arg(year.getEnergy(),0,'f',0));
    if (sp.getSolarPlantProperties().getPeakPower() > 0) {
        this->ui->lYearFullLoadHours->setText(QString("%1 h").arg(year.getEnergy()/sp.getSolarPlantProperties().getPeakPower(), 0, 'f', 0));
    } else {
        this->ui->lYearFullLoadHours->setText(QString("-"));
    }

}

void QSolanizer::plotAllYearData()
{
    this->ui->wYearPlot->clearPlottables();
    QVector<double> months;
    QVector<QString> labels;

    QList<QVector<double>> allEnergyData;

    for (int i=sp.getBeginningDate().year(); i<=sp.getEndingDate().year(); ++i) {
        Year year = sp.getYear(i);
        QVector<double> yearEnergy;
        for (int month = 1; month <= 12; month++) {
            if (year.hasDataOfMonth(month)) {
                yearEnergy << year.getMonth((int)month).getEnergy();
            } else {
                yearEnergy << 0;
            }
        }
        allEnergyData << yearEnergy;
    }
    QCPBarsGroup *group = new QCPBarsGroup(this->ui->wYearPlot);
    for (int i=0; i<allEnergyData.size(); i++) {
        QVector<double> yearData = allEnergyData.at(i);
        QVector<double> months{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        QCPBarsEnhanced* bars = new QCPBarsEnhanced(this->ui->wYearPlot->xAxis, this->ui->wYearPlot->yAxis);
        bars->setData(months, yearData);
        QColor c = QColor(this->someColors.at(i % someColors.size()));
        bars->setPen(c);
        c.setAlpha(50);
        bars->setBrush(c);
        bars->addToLegend();
        bars->setWidth(2/(3*(1+(float)sp.getEndingDate().year()-sp.getBeginningDate().year())));
        bars->setBarsGroup(group);
        bars->setName(QString::number(sp.getBeginningDate().year()+i));
        bars->setDataValue(sp.getBeginningDate().year()+i);
    }


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
    this->startTotalPlot = sp.getBeginningDate().year();
    // get data
    QList<QVector<double>> allEnergyData;
    QVector<double> years;
    QVector<QString> labels;

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
        pen.setColor(this->yearColors.at(i-1));
        bar->setPen(pen);
        bar->addToLegend();
        QColor color = QColor(this->yearColors.at(i-1));
        color.setAlpha(150);
        bar->setBrush(color);
        bar->setData(years , allEnergyData.at(i-1));
        if (!bars.isEmpty()) {
            bar->moveAbove(bars.at(bars.size()-1));
        }
        bars.append(bar);
    }

    this->ui->wTotalPlot->xAxis->setRange(years.at(0)-1, years.at(years.size()-1)+2); //+2 to have space for the legend
    this->ui->wTotalPlot->yAxis->setRange(0, sp.getHighestYearEnergy()*1.1);
    this->ui->wTotalPlot->yAxis->setLabel("Energie / kWh");

    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0,0,0,25));
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
    if (sp.getSolarPlantProperties().getPeakPower() > 0) {
        this->ui->lTotalFullLoadHours->setText(QString("%1 h").arg(sp.getEnergy()/sp.getSolarPlantProperties().getPeakPower(), 0, 'f', 0));
    } else {
        this->ui->lTotalFullLoadHours->setText(QString("-"));
    }
}

int QSolanizer::getCurrentPlottingMode()
{
    int mode = 0x0;
    if (this->ui->cTheoreticalCurve->isChecked()) {
        mode |= THEORETICAL;
    }
    if (this->ui->cAvergeageCurve->isChecked()) {
        mode |= AVERAGE;
    }
    if (this->ui->cRealCurve->isChecked()) {
        mode |= REAL;
    }
    return mode;
}

void QSolanizer::drawColorScale()
{
    QLinearGradient gradient(this->ui->wColorScale->rect().bottomLeft(), this->ui->wColorScale->rect().topLeft());
    QPalette palette(this->ui->wColorScale->palette());
    int red = this->minEnergyColor.red() + (this->maxEnergyColor.red() - this->minEnergyColor.red()) * .5;
    int green = this->minEnergyColor.green() + (this->maxEnergyColor.green() - this->minEnergyColor.green()) * .5;
    int blue = this->minEnergyColor.blue() + (this->maxEnergyColor.blue() - this->minEnergyColor.blue()) * .5;
    gradient.setColorAt(0, this->minEnergyColor);
    gradient.setColorAt(.5, QColor(red, green, blue));
    gradient.setColorAt(1, this->maxEnergyColor);
    palette.setBrush(QPalette::Window, QBrush(gradient));
    this->ui->wColorScale->setAutoFillBackground(true);
    this->ui->wColorScale->setPalette(palette);
}

void QSolanizer::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    // for a reason, this must be implemented here
    drawColorScale();
}

void QSolanizer::closeEvent(QCloseEvent *event)
{
    this->writeSettings();
    event->accept();
}

void QSolanizer::on_calendarWidget_selectionChanged()
{
    QDate date = this->ui->calendarWidget->selectedDate();
    this->plotDayData(date, this->ui->cMultpleChoice->checkState() == Qt::Checked, this->getCurrentPlottingMode());
    this->shownDates.append(date);
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

            QDate min = month >= sp.getBeginningDate() ? month : sp.getBeginningDate();
            QDate max =lastDayOfMonth <= sp.getEndingDate() ? lastDayOfMonth : sp.getEndingDate();

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
    this->plotDayData(date, this->ui->cMultpleChoice->checkState() == Qt::Checked, this->getCurrentPlottingMode());
    this->shownDates.append(date);
}

void QSolanizer::on_dateEditStart_userDateChanged(const QDate &date)
{
    if (!this->ui->tMonthSelection->hasFocus()) {
        this->ui->dateEditEnd->setMinimumDate(date);
        this->showCustomRange(date, this->ui->dateEditEnd->date());
    }
}

void QSolanizer::on_dateEditEnd_userDateChanged(const QDate &date)
{
    if (!this->ui->tMonthSelection->hasFocus()) {
        this->ui->dateEditStart->setMaximumDate(date);
        this->showCustomRange(this->ui->dateEditStart->date(), date);
    }
}

void QSolanizer::on_rEnergy_toggled(bool checked)
{
    if (this->dataSuccessfullyLoaded) {
        this->showCustomRange(this->ui->dateEditStart->date(), this->ui->dateEditEnd->date());
    }
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
            this->initializePlots();
            this->fillDataWidgets();
            this->writeSerializedData();
            this->enableAllInputWidgets();
            QFileInfo fileInfo(QDir(this->path).absoluteFilePath(this->propertyname));
            if (fileInfo.exists()) {
                this->sp.getSolarPlantProperties().readProperties(QDir(this->path).absoluteFilePath(this->propertyname));
            }
        }
    }
}

void QSolanizer::on_actionOpenNew_triggered()
{
    if (this->getProperDir(true)) {
        if (this->readSerializedData() || this->readData()) {
            this->initializePlots();
            this->fillDataWidgets();
            this->writeSerializedData();
            this->enableAllInputWidgets();
            QFileInfo fileInfo(QDir(this->path).absoluteFilePath(this->propertyname));
            if (fileInfo.exists()) {
                this->sp.getSolarPlantProperties().readProperties(QDir(this->path).absoluteFilePath(this->propertyname));
            }
        }
    }
}

void QSolanizer::on_actionClose_triggered()
{
    this->close();
}

void QSolanizer::on_actionAbout_triggered()
{
    QMessageBox::about(this, "Über", "QSolanizer "+this->version+
                                "\nCarsten Menne"
                                "\nqsolanizer@lostbit.de"
                                "\nhttps://github.com/taxus13/QSolanizer");
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

void QSolanizer::monthItemClicked(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QDate plotDate;
        if (ui->rEnergy->isChecked()) {
            plotDate = startMonthPlot.addDays(dataIndex);
        } else if (ui->rDistribution->isChecked()) {
            QCPStatisticalBox *box = dynamic_cast<QCPStatisticalBox*>(plottable);
            plotDate = QDateTime::fromSecsSinceEpoch(box->data()->begin()->mainKey()).date();
        }
        this->plotDayData(plotDate, false, this->getCurrentPlottingMode());
        this->ui->tabWidget->setCurrentIndex(0);
        event->accept();
    } else {
        event->ignore();
    }
}

void QSolanizer::yearItemClicked(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QCPBarsEnhanced *bars = dynamic_cast<QCPBarsEnhanced*>(plottable);
        int offset = 0;
        int year = 0;
        if (bars) {
            offset = offset - 1 + qRound(bars->getKeyValueOfPixelPosition(event->x(), event->y()));
            year = bars->getDataValue();
        }
        if (year == 0) {
            year = this->startYearPlot.year();
        }
        QDate start = QDate(year, this->startYearPlot.month()+offset, 1);
        QDate end = start.addMonths(1).addDays(-1);
        if (start < sp.getBeginningDate()) {
            start = sp.getBeginningDate();
        }
        if (end > sp.getEndingDate()) {
            end = sp.getEndingDate();
        }
        this->showCustomRange(start, end);
        this->ui->dateEditEnd->setMinimumDate(start);
        this->ui->dateEditStart->setMaximumDate(end);
        this->ui->dateEditStart->setDate(start);
        this->ui->dateEditEnd->setDate(end);
        this->ui->tabWidget->setCurrentIndex(1);
        event->accept();
    } else {
        event->ignore();
    }
}

void QSolanizer::totalItemClicked(QCPAbstractPlottable *plottable, QMouseEvent *event)
{
    event->ignore();
}

void QSolanizer::legendItemClicked(QCPLegend *legend, QCPAbstractLegendItem *legendItem, QMouseEvent *event)
{
    QCPPlottableLegendItem *plottableItem = dynamic_cast<QCPPlottableLegendItem*>(legendItem);
    if (event->button() == Qt::LeftButton) {
        this->lastClickedItem = plottableItem;
        if (plottableItem) {
            int type = plottableItem->plottable()->property("type").toInt();
            QDate date = plottableItem->plottable()->property("date").toDate();
            Day d;
            if (type == REAL) {
                d = sp.getDay(date);
            } else if (type == THEORETICAL) {
                d = sp.getSolarPlantProperties().getTheoreticalPowerCurve(date, false);
            } else if (type == AVERAGE){
                d = sp.getAverageDay(date.month());
            }
            this->fillDayGroupbox(d);
            //plottableItem->plottable()->setSelected(true);
        }
        event->accept();
    } else {
        event->ignore();
    }
}

void QSolanizer::contextMenuTriggered(QAction *action)
{
    QCPAbstractPlottable* plottable = (QCPAbstractPlottable*) action->data().toInt();
    this->shownDates.removeOne(plottable->property("date").toDate());
    this->replotDayData(this->getCurrentPlottingMode());
}

void QSolanizer::on_actionSolarPlantProperties_triggered()
{
    SolarPlantPropertyDialog diag(this->sp.getSolarPlantProperties(), this);
    if (diag.exec() == QDialog::Accepted) {
        sp.setSolarPlantProperties(diag.getSolarPlantProperties());
        sp.getSolarPlantProperties().writePorperties(QDir(this->path).absoluteFilePath(this->propertyname));
        this->fillDataWidgets();
    }
}



void QSolanizer::on_cRealCurve_clicked()
{
    this->replotDayData(this->getCurrentPlottingMode());
}

void QSolanizer::on_cTheoreticalCurve_clicked()
{
    this->replotDayData(this->getCurrentPlottingMode());
}

void QSolanizer::on_cAvergeageCurve_clicked()
{
    this->replotDayData(this->getCurrentPlottingMode());
}
