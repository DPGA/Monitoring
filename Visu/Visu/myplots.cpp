
#include "myplots.h"

MyPlots::MyPlots(int Board,int Ch)
{
    x.reserve(1024);
    y.reserve(1024);
    this->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    QString Text = "Board Id = 0x"+ QString::number(Board+16,16) +" Chan " + QString::number(Ch,10);
    this->replot();

    WinZoom  = new QMainWindow;

    PlotZoom = new QCustomPlot();
    PlotZoom->addGraph();
    PlotZoom->addGraph();
    PlotZoom->addGraph();
    PlotZoom->addGraph();
    PlotZoom->xAxis->setRange(0, 1024);
    PlotZoom->yAxis->setRange(0, 4096);
    WinZoom->setMinimumSize(800, 600);
    WinZoom->setCentralWidget(PlotZoom);

    PlotZoom->plotLayout()->insertRow(0);
    PlotZoom->plotLayout()->addElement(0, 0, new QCPTextElement(PlotZoom,Text , QFont("sans", 12, QFont::Bold)));
    connect(this,SIGNAL(mousePress(QMouseEvent*)),this,SLOT(SetZoomGraph()));
}

MyPlots::~MyPlots()
{

}

void MyPlots::DisplayGraph(unsigned short Srout,unsigned short *Data,int len)
{
    x.clear();
    y.clear();
    if (len <= 1024) {
        for (int i=0;i<len;++i) {
            x.push_back((double) i);
            unsigned int d = ntohs(Data[i]) & 0xFFF;
            y.push_back((double) (d));
        }
        for (int i=len;i<1024;i++)
           x.push_back((double) i);

        // create graph and assign data to it:
        this->addGraph();
        this->xAxis->setRange(10, 1024);
        this->yAxis->setRange(10, 4100);
        this->graph(0)->setData(x,y);
        this->graph(0)->setLineStyle(QCPGraph::lsNone);
        this->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
        this->graph(0)->setPen(QPen(Qt::red));
        this->replot();
        if (ZoomActived) ZoomGraph(Srout,len);

    }
}

void MyPlots::SetZoomGraph()
{
 // if (!ZoomActived) {
    WinZoom->show();
    ZoomActived = true;
  //}
}

void MyPlots::ZoomGraph(unsigned short Srout,int len)
{
    QVector<double> liney(4096),linex(4096),cell0(4096);
    QVector<double> yrescale(3000);
    for (int k=0;k<4096;++k) {
        liney[k] = k;
        linex[k] = Srout;
        double temp = len - Srout;
        cell0[k] = 1024-Srout;
    }
    qInfo() << "Srout = " << hex << Srout << endl;

    int j=Srout;
    for (int i=0;i<len;++i)
    {
   //   qInfo() << "I = " << i << " j=" << j << " Srout "<< Srout << endl;
      yrescale[j] = y[i]+200;
      j = (j+1) % 1024;
    }
    //    if (ZoomActived && (ChZoom == ch) && (NoBoard == BoardZoom)) {
    double min = *std::min_element(y.constBegin(), y.constEnd()) -200;
    double max = *std::max_element(y.constBegin(), y.constEnd());
    max = floor((max+200)/100)*100;
    PlotZoom->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
//    PlotZoom->yAxis->setRange(min, max);
    PlotZoom->yAxis->setRange(0, 4096);
    PlotZoom->graph(0)->setData(x,y);
    PlotZoom->graph(0)->setLineStyle(QCPGraph::lsNone);
    PlotZoom->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    PlotZoom->graph(0)->setPen(QPen(Qt::blue));
    QString Text = "RawData: NbSample =  "+ QString::number(len,10);
    PlotZoom->graph(0)->setName(Text);
    PlotZoom->graph(1)->setPen(QPen(Qt::green));
    QString SroutVal = "Srout " +QString::number(Srout);
    PlotZoom->graph(1)->setName(SroutVal);
    PlotZoom->graph(1)->setData(linex,liney);
    PlotZoom->graph(2)->setData(cell0,liney);
    PlotZoom->graph(2)->setPen(QPen(Qt::red));
    PlotZoom->graph(2)->setName("Cell 0");
    PlotZoom->graph(3)->setData(x,yrescale);
    PlotZoom->graph(3)->setLineStyle(QCPGraph::lsNone);
    PlotZoom->graph(3)->setScatterStyle(QCPScatterStyle::ssCross);
    PlotZoom->graph(3)->setPen(QPen(Qt::yellow));
    PlotZoom->graph(3)->setName("Begin cell 0");
    // now you can access it at any time with title->setText, for example

    PlotZoom->legend->setVisible(true);
    PlotZoom->replot();
   //     }

}
