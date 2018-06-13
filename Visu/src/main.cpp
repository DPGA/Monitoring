#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>

//#include "qcustomplot.h"
#include <frame.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    return a.exec();
}
