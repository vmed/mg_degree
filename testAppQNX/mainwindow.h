#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include "PCI1753.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTextStream out;
    PCI1753 PCI1753_0;
private slots:
    void updateGraph(double, double);
    void on_bStart_clicked();
    void on_bStop_clicked();
    void writeToFile(double, double);

};

#endif // MAINWINDOW_H