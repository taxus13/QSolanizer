#include "qdatarow.h"

QDataRow::QDataRow()
{

}

QDataRow::QDataRow(const QVector<double> &copy)
{
    foreach (double d, copy) {
        this->append(d);
    }
}

double QDataRow::getSum()
{
    double sum = 0;
    for (int i=0; i<this->size(); i++) {
        sum+=this->at(i);
    }
    return sum;
}

void QDataRow::applyMaximumValues(QDataRow &qdr)
{
    if (this->size() == qdr.size()) {
        for (int i=0; i<this->size(); i++) {
            this->replace(i, qMax(this->at(i), qdr[i]));
        }
    }
}

QDataRow QDataRow::operator+(const QDataRow &right)
{
    if (this->size() == right.size()) {
        QDataRow qdr;
        for (int i=0; i<this->size(); i++){
            qdr.append(this->at(i) + right.at(i));
        }
        return qdr;
    } else {
        return *this;
    }
}

QDataRow QDataRow::operator+=(const QDataRow &right)
{
    if (this->size() == right.size()) {
        for (int i=0; i<this->size(); i++){
            this->replace(i, this->at(i) + right.at(i));
        }

    }
    return *this;
}

QDataRow QDataRow::operator+(const int &right)
{
    QDataRow qdr;
    for (int i=0; i<this->size(); i++){
        qdr.append(this->at(i) + right);
    }
    return qdr;
}

QDataRow QDataRow::operator+=(const int &right)
{
    for (int i=0; i<this->size(); i++){
        this->replace(i, this->at(i) + right);
    }
    return *this;
}

QDataRow QDataRow::operator-(const QDataRow &right)
{
    if (this->size() == right.size()) {
        QDataRow qdr;
        for (int i=0; i<this->size(); i++){
            qdr.append(this->at(i) - right.at(i));
        }
        return qdr;
    } else {
        return *this;
    }
}

QDataRow QDataRow::operator-=(const QDataRow &right)
{
    if (this->size() == right.size()) {
        for (int i=0; i<this->size(); i++){
            this->replace(i, this->at(i) - right.at(i));
        }

    }
    return *this;
}

QDataRow QDataRow::operator /=(const double &right)
{
    for (int i=0; i<this->size(); i++){
        this->replace(i, this->at(i) / right);
    }
    return *this;
}

QDataRow QDataRow::operator *=(const double &right)
{
    for (int i=0; i<this->size(); i++){
        this->replace(i, this->at(i) * right);
    }
    return *this;
}

QDataRow operator /(const QDataRow &left, const double &right)
{
    QDataRow qdr;
    for (int i=0; i<left.size(); i++){
        qdr.append(left.at(i) / right);
    }
    return qdr;
}

QDataRow operator *(const double &left, const QDataRow &right)
{
    return right*left;
}

QDataRow operator *(const QDataRow &left, const double &right)
{
    QDataRow qdr;
    for (int i=0; i<left.size(); i++){
        qdr.append(left.at(i) * right);
    }
    return qdr;
}

