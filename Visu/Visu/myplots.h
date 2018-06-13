#ifndef MYPLOTS_H
#define MYPLOTS_H
#include <qcustomplot.h>
#include <arpa/inet.h>

class MyPlots :  public QCustomPlot
{
    Q_OBJECT
    QWidget* m_mainWindow;

public:
    explicit MyPlots(int Board,int Ch);
    ~MyPlots();
    void DisplayGraph(unsigned short Srout,unsigned short *Data,int len);

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
};


#endif // MYPLOTS_H
