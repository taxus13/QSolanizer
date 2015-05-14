#ifndef QSOLANIZER_H
#define QSOLANIZER_H

#include <QMainWindow>
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
private slots:
    void test();
};

#endif // QSOLANIZER_H
