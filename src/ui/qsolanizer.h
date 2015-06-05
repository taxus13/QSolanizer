#ifndef QSOLANIZER_H
#define QSOLANIZER_H

#include <QMainWindow>
#include <QtConcurrent>
#include <QTreeWidgetItem>
#include <QProgressDialog>

#include <utils/csvreader.h>



namespace Ui {
class QSolanizer;
}

class QSolanizer : public QMainWindow
{
    Q_OBJECT

public:
    explicit QSolanizer(QWidget *parent = 0);
    ~QSolanizer();

private:
    Ui::QSolanizer *ui;

    void initializeVariables();
    void readSettings();
    void writeSettings();

    void fillDataWidgets();

    bool getProperDir(bool changeDir);
    bool readData();

    // plotting and label filling functions
    void plotDayData(QDate date, bool keepOldGraphs);
    void resetDayPlot();

    void showMonthData(QDate date);
    void showCustomRange(QDate start, QDate end);
    void plotDailyEnergyValues(QPair<QVector<QDate>, QVector<float> > &data);
    void plotDailyDistribution(QVector<QList<QDateTime> > &data);
    void plotYearData(int year);
    void plotAllYearData();
    void plotTotalData();

    bool readSerializedData();
    void writeSerializedData();

    void disableAllInputWidgets();
    void enableAllInputWidgets();

    void closeEvent(QCloseEvent *event);

    SolarPart sp;
    QList<QColor> someColors;
    QList<QColor> dayColors;

    QString path;
    QString filename;

    int count;

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
};

#endif // QSOLANIZER_H
