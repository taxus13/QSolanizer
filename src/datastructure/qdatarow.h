#ifndef QDATAROW_H
#define QDATAROW_H

#include <QVector>

class QDataRow : public QVector<double>
{
public:
    QDataRow();
    QDataRow(const QVector<double> &copy);

    double getSum();

    QDataRow operator+(const QDataRow& right);
    QDataRow operator+=(const QDataRow& right);
    QDataRow operator-(const QDataRow& right);
    QDataRow operator-=(const QDataRow& right);

   friend QDataRow operator *(const QDataRow& left, const double& right);
   friend QDataRow operator *(const double& left, const QDataRow& right);
   friend QDataRow operator /(const QDataRow& left, const double& right);
   QDataRow operator /=(const double& right);
   QDataRow operator *=(const double& right);
};

#endif // QDATAROW_H
