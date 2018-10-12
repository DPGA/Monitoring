
#include "myplotsqwt.h"
#include "histogramme.h"
#include <qwt_scale_engine.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>

#include <qwt_symbol.h>
#include <zoomer.h>
#include <QEvent>
#include <QKeyEvent>

MyPlotsQwt::MyPlotsQwt(QString title, const bool zoom, QWidget *parent)
{
	DisplayHisto = false;
	CenterPlotMean = false;
	Zoom = zoom;
	Title = title;
	curve.clear();
	
	QwtText titre(title);
	titre.setFont(QFont("Helvetica", 10));
	setTitle(titre);

	QwtPlotCanvas *canvas = new QwtPlotCanvas();
	canvas->setPalette( Qt::gray );
	canvas->setBorderRadius( 10 );
	setCanvas( canvas );
	mX = NULL;
	h = NULL;
	plotLayout()->setAlignCanvasToScales( true );

	this->replot();

/*	QwtLegend *legend = new QwtLegend;
	legend->setDefaultItemMode( QwtLegendData::Checkable );
	insertLegend( legend, QwtPlot::RightLegend );

	replot(); // creating the legend items
*/
   
   
   FisrtDisplay = true;
 //  rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    zoomer = new MyZoomer(this->canvas());
    zoomer->setRubberBandPen(QPen(Qt::black, 2,Qt::DotLine));
	zoomer->setTrackerPen(QPen(Qt::black));
	zoomer->setMousePattern( QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
	zoomer->setMousePattern( QwtEventPattern::MouseSelect3,Qt::RightButton );

	QwtPlotPanner *panner = new QwtPlotPanner( this->canvas());
	panner->setAxisEnabled( QwtPlot::yRight, false );
	panner->setMouseButton( Qt::MidButton );

}

void MyPlotsQwt::addTitle(QString title)
{
	Title = title;
	setTitle(title);
}

bool MyPlotsQwt::eventFilter (QObject *object, QEvent *event)
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent* key = static_cast<QKeyEvent*>(event);
		if (key->key()==Qt::Key_Home) {
			setAxisAutoScale(QwtPlot::xBottom,true);
			replot();
		}
	}
	
	if ((!Zoom) && (!ZoomActived))
		if (event->type() == QEvent::MouseButtonDblClick)
		{
			ZoomGraph();
			return true;
		}
  return false;
}

void MyPlotsQwt::clearCurve()
{
    if ((DisplayHisto) && (h)){
		h->detach();
		if (d_marker) {
			d_marker->detach();
			delete (d_marker);
		}
	}
	else {
		for (int i=0;i<curve.size();i++) 
			curve.at(i)->detach();
		curve.clear();
	}
    this->replot();
}

QwtPlotCurve *MyPlotsQwt::CreateCurve(const QColor c,const QwtPlotCurve::CurveStyle cs, const QwtPlot::Axis axes)
{
    DisplayHisto = false;
    QwtPlotCurve *curvetemp = new QwtPlotCurve;

    curvetemp->setPen(QPen(c));

    curvetemp->setStyle(cs);
    curvetemp->setPen(c,2.0);
    curvetemp->setYAxis(axes);
    curvetemp->attach(this);
    return (curvetemp);
}


void MyPlotsQwt::addPoint(QPointF p,const QColor c,const QwtPlotCurve::CurveStyle cs,const bool add)
{

    if (curve.size() == 0) {
        curve.push_back(CreateCurve(c,cs));
    }


    lCurveData.push_back(QPointF(p));

    curve.back()->setSamples(lCurveData);
    if (ZoomActived) {
        curve.back()->attach(PlotZoom);
        PlotZoom->replot();
    }
    else this->replot();
}


void MyPlotsQwt::setData(QVector<QPointF> p,const QColor c,const QwtPlotCurve::CurveStyle cs,const QwtPlot::Axis axes)
{
    curve.push_back(CreateCurve(c,cs,axes));
    curve.back()->setSamples(p);

    if (ZoomActived) {
        for (auto& item : curve)
          item->attach(PlotZoom);
        PlotZoom->replot();
    }
    else this->replot();

}

void MyPlotsQwt::setData(QVector<double> x,QVector<double> y,const QColor c,const  QwtPlotCurve::CurveStyle cs)
{
	DisplayHisto = false;
	QwtPlotCurve *curvetemp = new QwtPlotCurve;
	curvetemp->setSamples(x,y);
	curvetemp->setPen(QPen(c));

	curvetemp->setStyle(cs);
	curvetemp->setPen(c,2.0);
	curvetemp->setStyle(QwtPlotCurve::Dots);
	curvetemp->attach(this);
	if (mX) mX->attach(this);
	curve.push_back(curvetemp);
   if (ZoomActived) {
        for (auto& item : curve)
          item->attach(PlotZoom);
        if (mX) mX->attach(PlotZoom);
        PlotZoom->replot();
    }
    else this->replot();


}
void MyPlotsQwt::setHisto(unsigned int num,long *histo,const QColor c)
{
	double *dHisto = (double *) malloc(sizeof(double)*num);
	DisplayHisto = true;
	if (!h) 	h = new Histogram( "",c);
	h->setColor(Qt::red);
	for (int i=0;i<num;i++) dHisto[i] = (double) histo[i];
	h->setValues(num,dHisto);
	//this->setAxisScale( QwtPlot::xBottom,h->GetMean()-10,h->GetMean()+10,10);
	
	if (ZoomActived) {
		h->attach(PlotZoom);
      PlotZoom->replot();
    }
    else {
		 h->attach(this);
		 this->replot();
	 }
	
	
	replot();
	zoomer->setZoomBase();
	free(dHisto);
}

void MyPlotsQwt::setHisto(QVector<double> *histo,const QColor c)
{
	DisplayHisto = true;
	h = new Histogram( "",c);
	h->setValues(histo);
	h->attach(this);
	this->setAxisScale( QwtPlot::xBottom,h->GetMean()-10,h->GetMean()+10,10);
	
	replot();
	zoomer->setZoomBase();
}


void MyPlotsQwt::setHisto(Histogram *histo)
{
	h = histo;
	h->attach(this);
}

void MyPlotsQwt::setMarker(const QString s,const int size)
{
	d_marker = new QwtPlotMarker();
	d_marker->setLineStyle( QwtPlotMarker::VLine );
	d_marker->setLabelAlignment( Qt::AlignRight | Qt::AlignTop );
	d_marker->setLinePen( QColor( Qt::green ), 0, Qt::DashDotLine );
//	d_marker->setSymbol( new QwtSymbol( QwtSymbol::Diamond,QColor( Qt::yellow ), QColor( Qt::green ), QSize( 8, 8 ) ) );
	d_marker->attach(this);
	double value;
	if (h) value = h->GetMean();
	else value = 0.0;
	QString label = s + " : "+ QString::number(h->GetMean(),'f',2) + "\nStdDev " + QString::number(h->GetStdDev(),'f',2);

	QwtText text( label );
	text.setFont( QFont( "Helvetica", size, QFont::Bold ) );
	text.setColor( QColor( 0, 0, 0 ) );

	d_marker->setValue(h->GetMean(),0 );
	d_marker->setLabel( text );
	setAutoReplot( true );
}

void MyPlotsQwt::closeEvent()
{
    // do what you need here
    // then call parent's procedure
    ZoomActived = false;
    qInfo() <<"close window " << Zoom;
    if (Zoom)
		if (DisplayHisto) h->detach();
		else for (auto &item:curve) item->detach();
    else 
		if (DisplayHisto) h->attach(this);
		else for (auto &item:curve) item->attach(this);
}

void MyPlotsQwt::setCenterPlotMean()

{
	CenterPlotMean = true;
}

void MyPlotsQwt::ZoomGraph()
{

        ZoomActived = true;
        WinZoom  = new MyWindow; //QMainWindow;
   
        WinZoom->setMinimumSize(800, 600);
        PlotZoom = new MyPlotsQwt(Title,true);
        WinZoom->setCentralWidget(PlotZoom);
        
        QBrush brush = canvasBackground();
		  brush.setColor(Qt::black);
		  PlotZoom->setCanvasBackground(brush);

    if (DisplayHisto) {
       PlotZoom->setHisto(h);
            //PlotZoom->setAxisScale( QwtPlot::xBottom,h->GetMean()-10,h->GetMean()+10,1 );
       PlotZoom->setMarker("Mean");
    }
    else {
      for (auto& item : curve) {
        item->attach(PlotZoom);
        mX->attach(PlotZoom);
      }
    }

	PlotZoom->setAxisScale( QwtPlot::xBottom,axisScaleDiv(QwtPlot::xBottom).lowerBound(),axisScaleDiv(QwtPlot::xBottom).upperBound() );
	PlotZoom->replotzoom();

	WinZoom->show();
   connect(WinZoom,SIGNAL(close()),this,SLOT(closeEvent()));    
}

void MyPlotsQwt::replotzoom()
{
	replot();
	zoomer->setZoomBase();
}

void MyPlotsQwt::setMarker(double posx)
{  
	if (!mX)	mX = new QwtPlotMarker();
   mX->setLabel(QwtText("Srout="+ QString::number(posx)));
   if (posx <512) mX->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
	else mX->setLabelAlignment(Qt::AlignLeft | Qt::AlignTop);
	mX->setLinePen(Qt::yellow);
   mX->setLineStyle(QwtPlotMarker::VLine);
   mX->setXValue(posx);
}
MyPlotsQwt::~MyPlotsQwt()
{
}



