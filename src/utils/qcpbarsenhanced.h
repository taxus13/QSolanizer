#ifndef QCPBARSENHANCED_H
#define QCPBARSENHANCED_H

#include "qcustomplot.h"

class QCPBarsEnhanced : public QCPBars
{
public:
    QCPBarsEnhanced(QCPAxis *keyAxis, QCPAxis *valueAxis);
    double getKeyValueOfPixelPosition(int x, int y);


};

#endif // QCPBARSENHANCED_H
