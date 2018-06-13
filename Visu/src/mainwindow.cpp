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
#include <shdmem.h>
#include "ringshm.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    DecodeFrame(),
   
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
 //        connect(Plot[k*24+i],SIGNAL(mousePress(QMouseEvent*)),this,SLOT(ZoomGraph(QMouseEvent*)));
        }
        for (int l=0;l<6;++l) {
          Num[k*6+l] = new QLCDNumber();
          LayoutVerticalPage1->addWidget(Num[k*6+l],l,5);
        }
        groupbox->setLayout(LayoutVerticalPage1);
        onglets->addTab(groupbox,NameAsm[k]);
    }
    
    QGroupBox *groupbox  = new QGroupBox("NetWork Monitoring");
    QGridLayout* LayoutVerticalPage1 = new QGridLayout;
    
    for (int i=0;i<MAX_ASM;i++) {
		PlotNet[i] = new MyPlots(0,i);
		QCPTextElement *title = new QCPTextElement(PlotNet[i]);
		QString text = "Packets/s Front-end " + QString::number(i+16,16);
		title->setText(text);
		title->setFont(QFont("sans", 12, QFont::Bold));
		PlotNet[i]->plotLayout()->insertRow(0); // insert an empty row above the axis rect
		PlotNet[i]->plotLayout()->addElement(0, 0, title); // place the title in the empty cell we've just created
		LayoutVerticalPage1->addWidget(PlotNet[i],i/4,i%4);
	}
/*    for (int l=0;l<6;++l) {
      Num[k*6+l] = new QLCDNumber();
      LayoutVerticalPage1->addWidget(Num[l],l,5);
    }
 */   
	groupbox->setLayout(LayoutVerticalPage1);
	onglets->addTab(groupbox,"Network");
    int largeurEcran = QApplication::desktop()->screenGeometry().width();
    int hauteurEcran = QApplication::desktop()->screenGeometry().height();
  //  fenetre->setMinimumSize(largeurEcran, hauteurEcran);
	fenetre->setCentralWidget(onglets);

	fenetre->show();

// Opening SHM
//===============================================
    ShdMem = new ShmRingBuffer<SharedMemory>(CAPACITY,false,SHM_ASM_DATA);
    ShdNet = new ShmRingBuffer<sStatFrame>(CAPACITY,true,SHM_NETWORK);
//===============================================


//=== Display Regular interval network information
    QTimer *timernet = new QTimer();
    timernet->connect(timernet, SIGNAL(timeout()),this,SLOT(ReadShmNetwork()));
    timernet->start(2000); //in ms

//=== Display Regular interval Asm datas
    QTimer *timer = new QTimer();
    timer->connect(timer, SIGNAL(timeout()),this,SLOT(ReadShmData()));
    timer->start(1000); //in ms
    

}

void MainWindow::ReadShmNetwork()
{
//===============================================
	while (ShdNet->begin() - ShdNet->end()) {
		sStatFrame PacketStats = ShdNet->dump_front();
		bool Display = true ? (onglets->currentIndex() == MAX_ASM) : false;
		if ((PacketStats.MemFeId > 0x9) && (PacketStats.MemFeId < 0x1c)) {
			
			PlotNet[PacketStats.MemFeId-0x10]->DisplayGraphStats(&PacketStats,Display);
		}
	}
}

void MainWindow::ReadShmData()
{
//===============================================
   struct S_HeaderFrame *Header;
   uint16_t *buf;
	

	
   while (ShdMem->begin() != ShdMem->end()) {
//		qInfo() << "Traitement Fragment";
		SharedMemory Packet = ShdMem->dump_front();
		SetPacket((uint16_t *) &Packet.raw[0]);
		Header = (struct S_HeaderFrame *) &Packet.raw[0];

		int FeID = GetFeId(); //((ntohs(Header->FeIdK30) & 0x7f00) >> 8) - 0x10;
		if (FeID >= 0x10) FeID -= 0x10;
		
//		unsigned long  trigthor = (ntohs(Header->CptTriggerThorMsb) << 16) + ntohs(Header->CptTriggerThorLsb);

//		QString str = QString::number(trigthor,10);
		QString str = QString::number(GetCptTriggerThor(),10);
		ui->TriggerThor->display(str);
//		qInfo() << "trig= " << trigthor << " " << hex << ntohs(Header->CptTriggerThorMsb) << " " << ntohs(Header->CptTriggerThorLsb) << dec;
		
		CptTrame++;
		
		ui->CptTrame->display(CptTrame);
		unsigned short nbSamples = GetNbSamples(); //ntohs(Header->NbSample);

		qInfo() << "Frond Id = " << hex << FeID + 0x10 << endl;
		qInfo() << "NbSamples = " << nbSamples << " len = " << (nbSamples+2)*2<< endl;
		if ((FeID >= 0) && (FeID <= 0xf)) {
			int addr= sizeof(S_HeaderFrame);
			for (int j=0;j<4;++j) {
				
				buf = GetChannel(j);
				unsigned short Ch = GetCh(); //(ntohs(buf[0]) >> 8) & 0x3f;
				int index = FeID*6 + (Ch/4);

				if ((j==0) && (index <96)){
					double temp = Num[index]->value() + 1;
					Num[index]->display(temp);
				}
				if ((Ch+FeID*24) <= MAX_ASM*24) {

					unsigned short Srout = GetSrout();
//					qInfo() << "onlet index  " << onglets->currentIndex();
					bool display = true ? (onglets->currentIndex() == FeID) : false; // Affiche le plot si l'onglet est actif			
					Plot[Ch+FeID*24]->DisplayGraph(Srout,&buf[2],nbSamples,display);
				}
				else
					qInfo() << " erreur ch + feID "<< hex << Ch << " " << FeID;
			}
		}
	}
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
									 
                            Plot[Ch+FeID*24]->DisplayGraph(Srout,&buf[2],nbSamples,true);
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
	qInfo() << " End Visu";
	if (ShdMem) delete(ShdMem);
	if (ShdNet) delete(ShdNet);
	if (ui) delete ui;
}
