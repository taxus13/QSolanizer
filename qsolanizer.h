#ifndef QSOLANIZER_H
#define QSOLANIZER_H

#include <QMainWindow>
#include <QtConcurrent>
#include <QTreeWidgetItem>
#include <QProgressDialog>

#include <csvreader.h>



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

    void fillDataWidgets();
    void readData();

    // plotting and label filling functions
    void plotDayData(QDate date);
    void showMonthData(QDate date);
    void showCustomRange(QDate start, QDate end);
    void plotDailyEnergyValues(QPair<QVector<QDate>, QVector<float> > &data);
    void plotDailyDistribution(QVector<QList<QDateTime> > &data);
    void plotYearData(int year);
    void plotAllYearData();
    void plotTotalData();


    SolarPart sp;
    QList<QColor> someColors;
    int count;
    bool locked;

private slots:
    void on_calendarWidget_selectionChanged();
    void on_bTest_clicked();
    void on_tMonthSelection_itemSelectionChanged();
    void on_listWidget_itemSelectionChanged();
    void on_checkBox_stateChanged(int checkState);
    void on_dateEdit_dateChanged(const QDate &date);
    void on_dateEditStart_editingFinished();
    void on_dateEditEnd_editingFinished();
    void on_bReadSerialized_clicked();
    void on_bWriteSerialized_clicked();
    void on_dateEditStart_userDateChanged(const QDate &date);
    void on_dateEditEnd_userDateChanged(const QDate &date);
    void on_radioButton_toggled(bool checked);
};

#endif // QSOLANIZER_H
