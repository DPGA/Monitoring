#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qcustomplot.h>
#include "myplots.h"
#include "shdmem.h"
#include "ringshm.h"
#include "decodeframe.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow , public DecodeFrame
{
    Q_OBJECT
    QWidget* m_mainWindow;

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots :
    void ReadFifo();
    void ReadShmData();
    void ReadShmNetwork();
private:

    Ui::MainWindow *ui;
    int FdFifo[MAX_ASM];
    MyPlots *Plot[24*MAX_ASM];
    MyPlots *PlotNet[24];
    QLCDNumber  *Num [6*MAX_ASM];
    double CptTrame = 0;
    QTabWidget *onglets;
    ShmRingBuffer<SharedMemory> *ShdMem;
    ShmRingBuffer<sStatFrame> *ShdNet;

};

#endif // MAINWINDOW_H
