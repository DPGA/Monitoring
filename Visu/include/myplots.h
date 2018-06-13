#ifndef MYPLOTS_H
#define MYPLOTS_H
#include <qcustomplot.h>
#include <arpa/inet.h>
#include "shdmem.h"

class MyPlots :  public QCustomPlot
{
    Q_OBJECT
    QWidget* m_mainWindow;

public:
	explicit MyPlots(int Board,int Ch);
	~MyPlots();
	void DisplayGraph(unsigned short Srout,unsigned short *Data,int len,bool display);
	void DisplayGraphStats(sStatFrame *PacketStats,bool display);
	void addPoint(double xval,double yval);
private slots:
    void ZoomGraph(unsigned short Srout,int len);
    void SetZoomGraph();

private :
    QVector<double> x;
    QVector<double> y;
    QMainWindow *WinZoom;
    QCustomPlot *PlotZoom;
    QCustomPlot *PlotCell;
    QCPTextElement *Title;
    bool ZoomActived = false;
    unsigned short SroutVal;
    int LenVal;
};


#endif // MYPLOTS_H
