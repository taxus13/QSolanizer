#include "qsolanizer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSolanizer w;
    w.show();

    return a.exec();
}
