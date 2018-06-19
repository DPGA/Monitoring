#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>

class MyWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MyWindow(QWidget *parent = 0);

signals:
    void close();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
};

#endif // MYWINDOW_H
