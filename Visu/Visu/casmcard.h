#ifndef CASMCARD_H
#define CASMCARD_H


#include <QMainWindow>
#include <QPushButton>
#include "qcustomplot.h"


class cAsmCard : public QWidget
{
    Q_OBJECT
    QPushButton *Daq;
    QWidget* AsmWidget;
    QCustomPlot *Plot[24];
    QCustomPlot *plot;
public :
    cAsmCard(QWidget *parent);
    void plotfunc(QFrame *frame);
    ~cAsmCard();
};


#endif // CASMCARD_H
