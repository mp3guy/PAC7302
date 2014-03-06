#include "tescocam.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TescoCam w;
    w.show();
    return a.exec();
}
