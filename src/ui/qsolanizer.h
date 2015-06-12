#ifndef QSOLANIZER_H
#define QSOLANIZER_H

#include <QMainWindow>
#include <QtConcurrent>
#include <QTreeWidgetItem>
#include <QProgressDialog>

#include "ui/solarplantpropertydialog.h"

#include "utils/csvreader.h"
#include "utils/qcpbarsenhanced.h"


namespace Ui {
class QSolanizer;
}

class QSolanizer : public QMainWindow
{
    Q_OBJECT

public:
    explicit QSolanizer(QWidget *parent = 0);
    ~QSolanizer();

    void drawColorScale();

    enum PlottingMode { BOTH, REAL, THEORETICAL};

private:
    Ui::QSolanizer *ui;
    // initializing, settings
    void initializeVariables();
    void readSettings();
    void writeSettings();
    // widget operations
    void fillDataWidgets();
    void disableAllInputWidgets();
    void enableAllInputWidgets();
    // data io
    bool getProperDir(bool changeDir);
    bool readData();
    bool readSerializedData();
    void writeSerializedData();


    // plotting and label filling functions
    void plotDayData(QDate date, bool keepOldGraphs, PlottingMode pm=REAL);
    void replotDayData(PlottingMode pm);
    void resetDayPlot();
    void showMonthData(QDate date);
    void showCustomRange(QDate start, QDate end);
    void plotDailyEnergyValues(QPair<QVector<QDate>, QVector<float> > &data);
    void plotDailyDistribution(QVector<QList<QDateTime> > &data);
    void plotYearData(int year);
    void plotAllYearData();
    void plotTotalData();

    PlottingMode getCurrentPlottingMode();

    void resizeEvent(QResizeEvent* event);
    void closeEvent(QCloseEvent *event);


    //variables
    static const int fileFormatVersion = 1;
    QString version;

    SolarPart sp;
    QList<QColor> someColors;
    QList<QColor> dayColors;
    QList<QColor> dayColorsDark;

    QColor maxEnergyColor;
    QColor minEnergyColor;


    QString path;
    QString filename;
    QString propertyname;
    bool dataSuccessfullyLoaded;

    // plotting
    QVector<double> dayTicksMSecs;
    QVector<QString> dayLabelMSecs;

    QList<QDate> shownDates;


    // information which data is currently plotted and shown
    QDate startMonthPlot;
    QDate startYearPlot;
    int startTotalPlot;


private slots:
    void on_calendarWidget_selectionChanged();
    void on_tMonthSelection_itemSelectionChanged();
    void on_listWidget_itemSelectionChanged();
    void on_dateEdit_dateChanged(const QDate &date);
    void on_dateEditStart_userDateChanged(const QDate &date);
    void on_dateEditEnd_userDateChanged(const QDate &date);
    void on_rEnergy_toggled(bool checked);
    void on_cMultpleChoice_toggled(bool checked);
    void on_bReset_clicked();
    void on_actionReload_triggered();
    void on_actionOpenNew_triggered();
    void on_actionClose_triggered();
    void on_actionAbout_triggered();
    void on_cCompareYears_stateChanged(int arg1);

    void monthItemClicked(QCPAbstractPlottable *plottable, QMouseEvent* event);
    void yearItemClicked(QCPAbstractPlottable *plottable, QMouseEvent* event);
    void totalItemClicked(QCPAbstractPlottable *plottable, QMouseEvent* event);
    void on_actionSolarPlantProperties_triggered();
    void on_rTheoreticalCurve_clicked();
    void on_rBoth_clicked();
    void on_rRealCurve_clicked();
};

#endif // QSOLANIZER_H
