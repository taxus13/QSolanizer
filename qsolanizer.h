#ifndef QSOLANIZER_H
#define QSOLANIZER_H

#include <QMainWindow>
#include <QtConcurrent>
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
    SolarPart sp;
private slots:
    void test();
    void on_calendarWidget_selectionChanged();
    void on_bTest_clicked();
};

#endif // QSOLANIZER_H
