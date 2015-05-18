#ifndef QSOLANIZER_H
#define QSOLANIZER_H

#include <QMainWindow>
#include <QtConcurrent>
#include <QTreeWidgetItem>
//#include <qwt_plot_curve.h>
//#include <qwt_plot.h>
//#include <qwt_point_data.h>
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
    void plotMonthData(QDate date);
    void plotYearData(int year);
    void plotAllYearData();
    void plotTotalData();


    SolarPart sp;
    QList<QColor> someColors;

private slots:
    void on_calendarWidget_selectionChanged();
    void on_bTest_clicked();
    void on_tMonthSelection_itemSelectionChanged();
    void on_listWidget_itemSelectionChanged();
    void on_checkBox_stateChanged(int checkState);
    void on_dateEdit_dateChanged(const QDate &date);
};

#endif // QSOLANIZER_H
