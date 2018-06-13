#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot.h>
#include "myplots.h"

const int MAX_ASM = 32;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QWidget* m_mainWindow;

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots :
    void ReadFifo();
private:

    Ui::MainWindow *ui;
    int FdFifo[MAX_ASM];
    MyPlots *Plot[24*MAX_ASM];
    QLCDNumber  *Num [6*MAX_ASM];
    double CptTrame = 0;
    QTabWidget *onglets;

};

#endif // MAINWINDOW_H
