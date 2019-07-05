#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>
#include <stdbool.h>
#include "hardwarerobot.h"
#include "cameracontrol.h"
#include <QThread>
#include <QTime>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include "SDK\Include\lucamapi.h"
#include "SDK\Include\lucamerr.h"
#include "SDK\Include\lucamsci.h"
#include "SDK\Include\lucamsequencing.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    float get_property(ULONG code);
    float get_property(ULONG code, long *flags);
    void set_property(ULONG code, float value, long flags = 0);
    void set_exposure(float value);
    void set_gain(float value);


signals:
    void txCameraCfgRequest();


    void txRobotConnected(bool b);
    void txRobotPort(QString str);
    void txRobotPos(int x, int y, int z);
    void txRobotStatusRequest();
    void txClearError();


public slots:
    void rxRobotConnected(bool b);
    void rxRobotPort(QString str);
    void fwdMoveCommand(int x, int y, int z);

private slots:
     void on_actionHardware_Settings_triggered();

     void on_bttn_moveCamera_clicked();

     void on_bttn_gain_clicked();

     void on_pushButton_open_file_clicked();

     void on_pushButton_setInitials_clicked();

     void on_pushButton_setSteps_clicked();

     void on_bttn_setNumberSteps_clicked();

     void on_bttn_close_clicked();

     void on_pushButton_clicked();

private:

     HANDLE handle;
     Ui::MainWindow *ui;
    //hardware control classes and threads

     hardwarerobot *hardSet;
     cameracontrol *rbtCtrl;


     QThread *cameraThread;
     //program state vars
     bool robotConnected;

     void sleep(int msec);
     QFile dataFile;
     QTextStream dataStream;

     int initial_x;
     int initial_y;
     int initial_z;


     int delta_x;
     int delta_y;
     int delta_z;

     int x_num;
     int y_num;
     int z_num;

     int point_spacing;
     int number_points;

      QString file_name;

      cameracontrol *control;



};

#endif // MAINWINDOW_H
