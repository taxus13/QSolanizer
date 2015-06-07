#include "qcpbarsenhanced.h"

QCPBarsEnhanced::QCPBarsEnhanced(QCPAxis *keyAxis, QCPAxis *valueAxis) :
    QCPBars(keyAxis, valueAxis)
{

}

double QCPBarsEnhanced::getKeyValueOfPixelPosition(int x, int y)
{
    double key;
    double value;
    pixelsToCoords(x, y, key, value);
    return key;
}


