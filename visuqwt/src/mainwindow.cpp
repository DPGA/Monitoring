#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollArea>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    DecodeFrame(),
    m_persist(false)
{
    ui->setupUi(this);



    lMyPlotsQwt.clear();
    // Opening SHM
    //===============================================
    ShdMem = new ShmRingBuffer<SharedMemory>(CAPACITY,false,SHM_ASM_DATA);
    ShdNet = new ShmRingBuffer<sStatFrame>(CAPACITY,true,SHM_NETWORK);
    ShdSrout = new ShmRingBuffer<sHistoSrout>(CAPACITY,false,SHM_SROUT);
    //===============================================



    for (int nbasm=0;nbasm<12;nbasm++) {
        QGridLayout *gBox = new QGridLayout;
        QGroupBox   *gTab = new QGroupBox();

        // Indicateur numérique
        QGroupBox *BoxIndicateur = new QGroupBox();
        QVBoxLayout *vBox = new QVBoxLayout;

        for (int j=0;j<6;j++) {
            lNumber << new QLCDNumber();
            vBox->addWidget(lNumber.back());
        }
        BoxIndicateur->setAlignment(Qt::AlignTop);
        BoxIndicateur->setLayout(vBox);


        gBox->addWidget(BoxIndicateur,1,6,6,6);

// Creation des plots
        QwtText xLabelCell("Cell");
        xLabelCell.setFont(QFont("Calibri",8));
        QwtText yLabelLeft("Cx Adc");
        yLabelLeft.setFont(QFont("Calibri",8));
        for (int row=0;row<6;row++) {
            for (int col=0;col<4;col++) {
                lMyPlotsQwt << new MyPlotsQwt("Ch" + QString::number(row*4+col),false);
                lMyPlotsQwt.back()->setAxisTitle( QwtPlot::xBottom, xLabelCell );
                lMyPlotsQwt.back()->setAxisTitle( QwtPlot::yLeft, yLabelLeft );
                lMyPlotsQwt.back()->setAxisFont(QwtPlot::yLeft, QFont("Calibri", 8, false));
                lMyPlotsQwt.back()->setAxisScale(QwtPlot::xBottom,0.0,1024.0,400.0);
                lMyPlotsQwt.back()->setAxisScale(QwtPlot::yLeft,0.0,4095.0,1000.0);
                gBox->addWidget(lMyPlotsQwt.back(),row+1,col);
            }
            lMyPlotshistoSrout << new MyPlotsQwt("Srout " + QString::number(row),false); 
            lMyPlotshistoSrout.back()->setAxisScale(QwtPlot::xBottom,0.0,1024.0,500.0);
            lMyPlotshistoSrout.back()->setAxisFont(QwtPlot::yLeft, QFont("Calibri", 8, false));
           
            gBox->addWidget(lMyPlotshistoSrout.back(),row+1,5);
        }
        gTab->setLayout(gBox);
        ui->tabWidget->addTab(gTab,"asm"+QString::number(nbasm));
    }

    QwtText xLabelCell("Cell");
    xLabelCell.setFont(QFont("Calibri",8));
    QwtText yLabelLeft("Cx Adc");
    yLabelLeft.setFont(QFont("Calibri",8));
    
 

    QGridLayout *gBox = new QGridLayout;
    QGroupBox   *gTab = new QGroupBox(); 
    
    QWidget *client = new QWidget;
	QScrollArea *scrollArea = new QScrollArea;
	scrollArea->setWidgetResizable(true);
    
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scrollArea->setWidget(client);
	client->setLayout(gBox);


    
	QTabWidget *tabPage = new QTabWidget;
	QWidget *pageWidget = new QWidget;
	pageWidget->setLayout(new QVBoxLayout);
	pageWidget->layout()->addWidget(scrollArea);
	tabPage->addTab(pageWidget, "all Asm");    

	QString title = QString("All Asm ");
	ui->tabWidget->addTab(tabPage, title);


    for (int row=0;row<12;row++) {
       for (int col=0;col<6;col++) {
  //  for (int row=0;row<6;row++) {    
   //     for (int col=0;col<12;col++) {        
            lMyPlotsQwtQuartet << new MyPlotsQwt("ASM" + QString::number(row),false);
            lMyPlotsQwtQuartet.back()->setAxisTitle( QwtPlot::xBottom, xLabelCell );
            lMyPlotsQwtQuartet.back()->setAxisTitle( QwtPlot::yLeft, yLabelLeft );
            lMyPlotsQwtQuartet.back()->setAxisFont(QwtPlot::yLeft, QFont("Calibri", 8, false));
            lMyPlotsQwtQuartet.back()->setAxisScale(QwtPlot::xBottom,0.0,1024.0,400.0);
            lMyPlotsQwtQuartet.back()->setAxisScale(QwtPlot::yLeft,0.0,4095.0,1000.0);
            gBox->addWidget(lMyPlotsQwtQuartet.back(),row+1,col);
        }
    }

   // scroll->setLayout(gBox);
   // ui->tabWidget->addTab(scroll,"all asm");


    QGroupBox *groupbox  = new QGroupBox("NetWork Monitoring");
    QGridLayout* LayoutVerticalPage1 = new QGridLayout;


    /*QwtPointSeriesData* myDataPackets;
    QwtPointSeriesData* myDataFlow;
    QVector<QPointF>* samplesPacket;
    QVector<QPointF>* samplesFlow;
    QList<QVector<QPointF>*> allsamplePackets;
    QList<QVector<QPointF>*> allsampleFlow;
    */
    QwtText xLabel("Time [s]");
    xLabel.setFont(QFont("Calibri",8));
    QwtText LabelLeft("Packets/s");
    LabelLeft.setFont(QFont("Calibri",8));
    LabelLeft.setColor("#0000ff");
    QwtText LabelRight("Mbits/s");
    LabelRight.setFont(QFont("Calibri",8));
    LabelRight.setColor("#ff0000");

    for (int i=0;i<12;i++) {
        lMyPlotNet << new MyPlotsQwt("ASM" + QString::number(i),false);
        lMyPlotNet.back()->enableAxis( QwtPlot::yRight );
        lMyPlotNet.back()->setAxisTitle( QwtPlot::xBottom, xLabel );
        lMyPlotNet.back()->setAxisTitle( QwtPlot::yLeft, LabelLeft );
        lMyPlotNet.back()->setAxisTitle( QwtPlot::yRight, LabelRight );
        lMyPlotNet.back()->setAxisFont(QwtPlot::yLeft, QFont("Calibri", 8, false));
        lMyPlotNet.back()->setAxisFont(QwtPlot::yRight, QFont("Calibri", 8, false));
 //       lMyPlotNet.back()->setAxisScale	(QwtPlot::xBottom,5.0,60.0,30.0);
		  
//		  QwtScaleDiv scalediv = QwtScaleDiv(0,30);
//		  lMyPlotNet.back()->setAxisScaleDiv(QwtPlot::xBottom,scalediv);
//		  lMyPlotNet.back()->setAxisLabelRotation(QwtPlot::xBottom, 90.0);
//		  QDateTime now = QDateTime::currentDateTime();
		  lMyPlotNet.back()->setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw(TimeDaq.currentTime()));

        samplesPacket = new QVector<QPointF>;
        samplesFlow   = new QVector<QPointF>;
        allsamplePackets.push_back(samplesPacket);
        allsampleFlow.push_back(samplesFlow);

        QString text = "Packets/s Front-end " + QString::number(i+16,16);
 //       title->setText(text);
 //       title->setFont(QFont("sans", 12, QFont::Bold));
 //       lMyPlotNet.back()->plotLayout()->insertRow(0); // insert an empty row above the axis rect
 //       lMyPlotNet.back()->plotLayout()->addElement(0, 0, title); // place the title in the empty cell we've just created
        LayoutVerticalPage1->addWidget(lMyPlotNet.back(),i/4,i%4);
    }

    groupbox->setLayout(LayoutVerticalPage1);
    ui->tabWidget->addTab(groupbox,"Network");



    connect(ui->ClearCounters,SIGNAL(clicked()),this,SLOT(ClearCounter()));
    connect(ui->ClearGraphs,SIGNAL(clicked()),this,SLOT(ClearGraph()));
    QTimer *timer = new QTimer();
    timer->connect(timer, SIGNAL(timeout()),this,SLOT(ReadShmData()));
    timer->start(1000); //in ms
    QTimer *timer1 = new QTimer();
    timer1->connect(timer1, SIGNAL(timeout()),this,SLOT(ReadHistoSrout()));
    timer1->start(1000); //in ms
    
    connect(ui->Persistence,SIGNAL(clicked(bool)),this,SLOT(Persistence(bool)));
    QTimer *timernet = new QTimer();
    timernet->connect(timer, SIGNAL(timeout()),this,SLOT(ReadShmNetwork()));
    timernet->start(2000); //in ms
    TimeDaq.start();

}





MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Persistence(bool c)
{
    if (c) m_persist = true;
    else m_persist = false;

}

void MainWindow::ClearCounter()
{
   int index = ui->tabWidget->currentIndex()-1;
   if (index < 12)
     for (auto q :lNumber)
        q->display(0);
}

void MainWindow::ClearGraph()
{
    int index = ui->tabWidget->currentIndex()-1;
    qDebug() << "index = " << index;
    switch (index) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:for (int Ch=0;Ch<24;Ch++) lMyPlotsQwt.at(Ch+index*24)->clearCurve();break;
        case 12:for (auto quartet:lMyPlotsQwtQuartet) quartet->clearCurve();break;
        case 13:for (auto net:lMyPlotNet) net->clearCurve();
    }
}



void MainWindow::ReadHistoSrout()
//===============================================
//===============================================

{
	while (ShdSrout->begin() != ShdSrout->end()) {
		sHistoSrout hSrout = ShdSrout->dump_front();
		u8 feId = hSrout.noBoard;
		if (feId > 0xf) feId -= 0x10;
		qDebug() << "drs " << hSrout.nohalfDrs << " fe " << feId;
		for (int i=0;i<6;i++)
			lMyPlotshistoSrout.at(i+feId*6)->setHisto((unsigned int)1024,&hSrout.HistoSrout[i][0],Qt::red);
	}
	
}
void MainWindow::ReadShmData()
//===============================================
//===============================================
{
   struct S_HeaderFrame *Header;
   uint16_t *buf;

   u16 srout;

   while (ShdMem->begin() != ShdMem->end()) {
 //       qInfo() << "Traitement Fragment";
        SharedMemory Packet = ShdMem->dump_front();
        SetPacket((uint16_t *) &Packet.raw[0]);
        Header = (struct S_HeaderFrame *) &Packet.raw[0];

        int FeID = GetFeId(); //((ntohs(Header->FeIdK30) & 0x7f00) >> 8) - 0x10;
        if (FeID >= 0x10) FeID -= 0x10;

//		unsigned long  trigthor = (ntohs(Header->CptTriggerThorMsb) << 16) + ntohs(Header->CptTriggerThorLsb);

//		QString str = QString::number(trigthor,10);
//        QString str = QString::number(GetCptTriggerThor(),10);
 //       ui->TriggerThor->display(str);
//		qInfo() << "trig= " << trigthor << " " << hex << ntohs(Header->CptTriggerThorMsb) << " " << ntohs(Header->CptTriggerThorLsb) << dec;

        CptTrame++;

//        ui->CptTrame->display(CptTrame);
        unsigned short nbSamples = GetNbSamples(); //ntohs(Header->NbSample);

//        qInfo() << "Frond Id = " << hex << FeID + 0x10;
//        qInfo() << "NbSamples = " << nbSamples << " len = " << (nbSamples+2)*2;
        QVector <double> x,y;

 //       if (ui->FrontEnd->value() == FeID)
        {
            for (int j=0;j<4;++j) {
                x.clear();y.clear();
                buf = GetChannel(j);
                unsigned short Ch = GetCh();
                if (j==0)lMyPlotsQwtQuartet.at((Ch/4)+FeID*6)->clearCurve();
                srout = GetSrout();
                qDebug() << "srout "<< srout << " ch " << Ch << " feid " << FeID << " nbsamples " << nbSamples << "check " << ui->EnableSrout->isChecked();
                QVector <QPointF> qPoint;
                for (int k=0;k<nbSamples;k++) {
                    qreal temp = (qreal) ntohs(buf[2+k]);
                    qreal xtemp;
                    if (ui->EnableSrout->isChecked()) xtemp = (qreal) ((k+srout) % 1024);
                    else xtemp = k;
                    qPoint.push_back(QPointF(xtemp,temp));
                    x.push_back(xtemp);y.push_back(temp);
//                    x.push_back(k);
//                    double temp=(double)(ntohs(buf[2+k]));
//                    y.push_back((double) (ntohs(buf[2+k]) & 0xfff));

 /*                   if ((k<10) && (temp > 2000)){
                        if (k==0) qDebug() << "channel " << Ch;
                        qDebug() << ntohs(buf[2+k]) << " " << y.back() << " " << temp;
                    }
  */              }

					qDebug() << " info after channel";
                    QColor color;
					if ((Ch >=0) && (Ch < 24)) {
                        int halfDrs = Ch % 4;
                        qDebug() << "half Drs " << halfDrs;
                        switch (halfDrs) {
                            case 0 : color = Qt::red;break;
                        case 1 : color = Qt::green;break;
                        case 2 : color = Qt::blue;break;
                        case 3 : color = Qt::magenta;break;
                        }
                        qDebug() << "Color " << color;
                      //if (FeID <6) 
                      lMyPlotsQwtQuartet.at((Ch/4)+FeID*6)->setData(x,y,color);
                      if (!m_persist) lMyPlotsQwt.at(Ch+FeID*24)->clearCurve();
                      lMyPlotsQwt.at(Ch+FeID*24)->setData(x,y);
                      lMyPlotsQwt.at(Ch+FeID*24)->setMarker(srout);
                      if (j==0) lNumber.at((Ch+FeID*24)/4)->display(lNumber.at((Ch+FeID*24)/4)->value()+1);
                    }
           }

        }
    }
}

void MainWindow::ReadShmNetwork()
{
//===============================================

    while (ShdNet->begin() - ShdNet->end()) {
        sStatFrame PacketStats = ShdNet->dump_front();
        bool Display = true ? (ui->tabWidget->currentIndex() == 12) : false;
        if ((PacketStats.MemFeId > 0x9) && (PacketStats.MemFeId < 0x1c)) {
  //        samplesPacket->push_back(QPointF((PacketStats.deltaMillisec/1000),(double)((PacketStats.NumPkts - PacketStats.lastPkts))));
			 samplesPacket->push_back(QPointF((TimeDaq.elapsed()/1000),(double)((PacketStats.NumPkts - PacketStats.lastPkts))));
			 samplesFlow->push_back(QPointF((TimeDaq.elapsed()/1000),(double)(((PacketStats.NumBytes*8/1000000) - (PacketStats.lastByte*8/1000000)))));
 //         samplesFlow->push_back(QPointF((PacketStats.deltaMillisec/1000),(double)(((PacketStats.NumBytes*8/1000000) - (PacketStats.lastByte*8/1000000)))));
          lMyPlotNet.at(PacketStats.MemFeId-0x10)->clearCurve();
          double temp = PacketStats.deltaMillisec/1000;
          double min,max;
          if (temp <60.0) {
            min = 0;max = 60;
          }
          else {
              min=temp-60.0;
              max=temp;
          }
          lMyPlotNet.at(PacketStats.MemFeId-0x10)->setAxisScale(QwtPlot::xBottom,(int)(TimeDaq.elapsed()/1000)-122,(int)(TimeDaq.elapsed()/1000), 60.0 );
 //         lMyPlotNet.at(PacketStats.MemFeId-0x10)->setAxisScale(QwtPlot::xBottom,min,max,30.0);
          lMyPlotNet.at(PacketStats.MemFeId-0x10)->setData(*samplesPacket,"#0000FF");
          lMyPlotNet.at(PacketStats.MemFeId-0x10)->setData(*samplesFlow,"#FF0000",QwtPlotCurve::Lines,QwtPlot::yRight);

//          lMyPlotNet.at(PacketStats.MemFeId-0x10)->addPoint(QPointF((PacketStats.deltaMillisec/1000),
//                                                   (double)((PacketStats.NumPkts - PacketStats.lastPkts))),Qt::green);
//            lMyPlotNet.at(PacketStats.MemFeId-0x10)->DisplayGraphStats(&PacketStats,Display);
        }
    }
}
