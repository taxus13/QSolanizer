#include "qbarplot.h"

QBarPlot::QBarPlot(QWidget *parent) :
    QCustomPlot(parent)
{
    //connect(this, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(showTooltip(QMouseEvent*)));
}

void QBarPlot::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "mouse move event fired.";
    emit mouseMove(event);

    // call event of affected layout element:
    if (mMouseEventElement)
      mMouseEventElement->mouseMoveEvent(event);

    QWidget::mouseMoveEvent(event);
}

void QBarPlot::showTooltip(QMouseEvent *event)
{
    qDebug() << "Tooltip fired.";
    int x = this->xAxis->pixelToCoord(event->pos().x());
    int y = this->yAxis->pixelToCoord(event->pos().y());

    setToolTip(QString("%1, %2").arg(x).arg(y));
}



