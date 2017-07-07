#ifndef QBARPLOT_H
#define QBARPLOT_H

#include "qdebug.h"
#include "qcustomplot.h"

class QBarPlot : public QCustomPlot
{
public:
    QBarPlot(QWidget *parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void showTooltip(QMouseEvent* event);
};

#endif // QBARPLOT_H
