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
#include <QtNetwork/QUdpSocket>
#include <QFileDialog>


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
       void ReadHistoSrout();
       void ClearGraph();
       void ClearCounter();
       void Persistence(bool c);
       void ReadUdpData();

       void on_SelectFile_clicked();

       void on_DisplayAvrg_clicked();

private:
    Ui::MainWindow *ui;
    void ReadFileCalib(QFile *f);
    double CptTrame = 0;
    ShmRingBuffer<SharedMemory> *ShdMem;
    ShmRingBuffer<sStatFrame>  *ShdNet;
    ShmRingBuffer<sHistoSrout> *ShdSrout;
    QList <class MyPlotsQwt *> lMyPlotshistoSrout;
    QList <class MyPlotsQwt *> lMyPlotsQwt;
    QList <class MyPlotsQwt *> lMyPlotsQwtQuartet;
    QList <class MyPlotsQwt *> lMyPlotNet;
    QList <QLCDNumber *> lNumber;
    bool m_persist;

    QwtPointSeriesData* myDataPackets;
    QwtPointSeriesData* myDataFlow;
    QVector<QPointF>* samplesPacket;
    QVector<QPointF>* samplesFlow;
    QList<QVector<QPointF>*> allsamplePackets;
    QList<QVector<QPointF>*> allsampleFlow;
    QTime TimeDaq;
    qreal calib[12][24][1024];
    QUdpSocket *udpSocket;

};

#endif // MAINWINDOW_H
