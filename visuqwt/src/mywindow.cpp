#include "mywindow.h"
#include <QDebug>
#include <QCloseEvent>

MyWindow::MyWindow(QWidget *parent) : QMainWindow(parent)
{

}


void MyWindow::closeEvent(QCloseEvent *event)
{
    qInfo() << "Close event mywindow";
    emit close();
    event->accept();
}
