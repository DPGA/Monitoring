#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLCDNumber>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QTimer>
#include <QLabel>



#include "shdmem.h"
#include "ringshm.h"
#include "decodeframe.h"
#include "myplotsqwt.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public DecodeFrame
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
       void ReadShmData();
       void ReadShmNetwork();
       void ClearGraph();
       void ClearCounter();
       void Persistence(bool c);

private:
    Ui::MainWindow *ui;

    double CptTrame = 0;
    ShmRingBuffer<SharedMemory> *ShdMem;
    ShmRingBuffer<sStatFrame> *ShdNet;
    QList <class MyPlotsQwt *> lMyPlotsQwt;
    QList <class MyPlotsQwt *> lMyPlotNet;
    QList <QLCDNumber *> lNumber;
    bool m_persist;

    QwtPointSeriesData* myDataPackets;
    QwtPointSeriesData* myDataFlow;
    QVector<QPointF>* samplesPacket;
    QVector<QPointF>* samplesFlow;
    QList<QVector<QPointF>*> allsamplePackets;
    QList<QVector<QPointF>*> allsampleFlow;

};

#endif // MAINWINDOW_H
