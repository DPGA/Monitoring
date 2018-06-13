#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "frame.h"
#include <arpa/inet.h>
#include "qcustomplot.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "myplots.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    char Temp[50];
    ui->setupUi(this);

    const QString NameAsm[MAX_ASM] = {"ASMx10","ASMx11","ASMx12","ASMx13","ASMx14","ASMx15","ASMx16","ASMx17","ASMx18","ASMx19","ASMx1a","ASMx1b","ASMx1c","ASMx1d","ASMx1e","ASMx1f"};


    QMainWindow *fenetre = new QMainWindow;
    onglets  = new QTabWidget;

    for (int k=0;k<MAX_ASM;++k) {
        QGroupBox *groupbox  = new QGroupBox("Test group box");

        QGridLayout* LayoutVerticalPage1 = new QGridLayout;
        for (int i=0;i<24;i++) {

          Plot[k*24+i] = new MyPlots(k,i);
  //        CreatePlot(Plot[k*24+i],i);
          LayoutVerticalPage1->addWidget(Plot[k*24+i],i/4,i%4);
 //         connect(Plot[k*24+i],SIGNAL(mousePress(QMouseEvent*)),this,SLOT(ZoomGraph(QMouseEvent*)));
        }
        for (int l=0;l<6;++l) {
          Num[k*6+l] = new QLCDNumber();
          LayoutVerticalPage1->addWidget(Num[k*6+l],l,5);
        }
        groupbox->setLayout(LayoutVerticalPage1);
        onglets->addTab(groupbox,NameAsm[k]);
    }
    fenetre->setMinimumSize(1900, 800);
    fenetre->setCentralWidget(onglets);

    fenetre->show();

// Opening Queues
//===============================================
    for (int i=0;i< MAX_ASM;++i) {
        sprintf(&Temp[0],"/tmp/fifo%x.fifo",i);
        FdFifo[i] = open(Temp,O_RDONLY|O_NONBLOCK);
        printf("opening %s\n ",Temp);
        while (read(FdFifo[i],&Temp,1) >0);
    }
//===============================================

    QTimer *timer = new QTimer();
    timer->connect(timer, SIGNAL(timeout()),this,SLOT(ReadFifo()));
    timer->start(100);

}

void MainWindow::ReadFifo()
{
    struct S_HeaderFrame Header;
    unsigned short buf[5000];

    for (int NoQueue=0;NoQueue<MAX_ASM;++NoQueue) {
        int Nb=read(FdFifo[NoQueue],&Header,sizeof(S_HeaderFrame));
        if ( Nb > 0) {
            int FeID = ((ntohs(Header.FeIdK30) & 0x7f00) >> 8) - 0x10;
            unsigned long  trigthor = (ntohs(Header.CptTriggerThorMsb) << 16) + ntohs(Header.CptTriggerThorLsb);
            QString str = QString::number(trigthor,10);
            qInfo() << "trig= " << trigthor << " " << hex << ntohs(Header.CptTriggerThorMsb) << " " << ntohs(Header.CptTriggerThorLsb) << dec;
            ui->TriggerThor->display(str);
            CptTrame++;
            ui->CptTrame->display(CptTrame);
            unsigned short nbSamples = ntohs(Header.NbSample);
            qInfo() << "Frond Id = " << hex << FeID + 0x10 << endl;
            qInfo() << "NbSamples = " << nbSamples << " len = " << (nbSamples+2)*2<< endl;
            if ((FeID >= 0) && (FeID <= 0xf)) {
                for (int j=0;j<4;++j) {
                    Nb =read(FdFifo[NoQueue],&buf,(nbSamples+2)*2);  //+2 = identification channel + Srout
                    if (Nb >0) {
                        unsigned short Ch = (ntohs(buf[0]) >> 8) & 0x3f;
                        qInfo() << "NoQueue = " << NoQueue << "sync = " << hex << ntohs(buf[0]) << dec << "ch = " << Ch
                                << "Sample 0 " << hex << ntohs(buf[2]);
  //                    printf("NoBoard = %d sync = %x  ch = %d\n",NoBoard,ntohs(buf[0]),Ch);
                        int index = FeID*6 + (Ch/4);

                        if ((j==0) && (index <96)){
                            double temp = Num[index]->value() + 1;
                            Num[index]->display(temp);
                        }
                        if ((Ch+FeID*24) <= MAX_ASM*24) {
                            unsigned short Srout = ntohs(buf[1]) & 0x3ff;
                            qInfo() << "srout " << Srout;

                            Plot[Ch+FeID*24]->DisplayGraph(Srout,&buf[2],nbSamples);
                        }
                        else
                            qInfo() << " erreur ch + feID "<< hex << Ch << " " << FeID;
                    }
                }
                Nb =read(FdFifo[NoQueue],&buf,4); //crc end frame
                if (Nb>0)  qInfo() << "Crc = " << hex << ntohs(buf[0]) << " EndFrame " << ntohs(buf[1]);
            }
        }
    }

}



MainWindow::~MainWindow()
{
    delete ui;
}
