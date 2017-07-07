#ifndef QCPBARSENHANCED_H
#define QCPBARSENHANCED_H

#include "qcustomplot.h"

class QCPBarsEnhanced : public QCPBars
{
public:
    QCPBarsEnhanced(QCPAxis *keyAxis, QCPAxis *valueAxis);
    double getKeyValueOfPixelPosition(int x, int y);
    int getDataValue() { return dataValue; }
    void setDataValue(int value) { dataValue = value; }
private:
    int dataValue;
};

#endif // QCPBARSENHANCED_H
