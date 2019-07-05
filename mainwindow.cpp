#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>
#include "hardwarerobot.h"
#include "cameracontrol.h"
#include <QDebug>
#include <QProcess>
#include <string>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

bool pressed;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //initialize the member classes


    control = new cameracontrol();
    handle=LucamCameraOpen(TRUE);
    //cameraThread =new QThread();
    hardSet=new hardwarerobot();
    rbtCtrl=new cameracontrol();
    pressed=true;


    ui->spinBox_numSteps->setValue(1);
    ui->spinBox_numY->setValue(1);
    ui->spinBox_numZ->setValue(1);
    //Connect Signals/Slots for Hardware Settings Window
    connect(hardSet,SIGNAL(txRobotPortName(QString)),this,SLOT(rxRobotPort(QString)));
    connect(this,SIGNAL(txRobotConnected(bool)),hardSet,SLOT(rxRobotConnected(bool)));


   //thread and connect the signals to the slots
   // connect(this, SIGNAL(txCameraCfgRequest()),rbtCtrl, SLOT(SetupCamera()) );

    //Connect Signals/Slots for Robot Control Class
    connect(rbtCtrl,SIGNAL(PortConnected(bool)),this,SLOT(rxRobotConnected(bool)));
    connect(this,SIGNAL(txRobotPort(QString)),rbtCtrl,SLOT(ConnectPort(QString)));
    connect(this,SIGNAL(txRobotPos(int,int,int)),rbtCtrl,SLOT(moveAbsolute(int,int,int)));
    connect(this,SIGNAL(txRobotStatusRequest()),rbtCtrl,SLOT(getStatus()));
    connect(this,SIGNAL(txClearError()),rbtCtrl,SLOT(alarmReset()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

float MainWindow::get_property(ULONG code)
{

    long flags;
    return get_property(code, &flags);

}

float MainWindow::get_property(ULONG code, long *flags)
{
    float value;
    LucamGetProperty(handle, code, &value, flags);
    return value;
}


void MainWindow::set_property(ULONG code, float value, long flags)
{
    LucamSetProperty(handle, code, value, flags);
}

void MainWindow::set_exposure(float value)
{
    set_property(LUCAM_PROP_STILL_EXPOSURE, value);
}

void MainWindow::set_gain(float value)
{
     set_property(LUCAM_PROP_GAIN, value);

}

//Robot Control Class has transmitted its connection status, forward to hardware config window
void MainWindow::rxRobotConnected(bool b)
{
    emit txRobotConnected(b);

    robotConnected = b;
}

//Slot for receiving port for the robot
void MainWindow::rxRobotPort(QString str)
{
    emit txRobotPort(str);
}

void MainWindow::fwdMoveCommand(int x, int y, int z)
{
      emit txRobotPos(x,y,z);
}


//User selects to open hardwar settings
void MainWindow::on_actionHardware_Settings_triggered()
{
    hardSet->updateGui();
    hardSet->show();
}

//user clicks on the move camera button
void MainWindow::on_bttn_moveCamera_clicked()
{


//    //fwdMoveCommand(initial_i, initial_j, initial_z);
//    if(initial_x + (x_num-1)*delta_x > 132000 | initial_y + (y_num-1)* delta_y > 65000 | initial_z +(z_num-1)*delta_z > 15000)
//    {
//        //throw an error, otherwise continue

//        QString errMessage="Values inputed are beyond the robot's x, y or z measurements";
//        QMessageBox message;
//        message.warning(0, "Error", errMessage);
//        message.show();

//    }
//    else
//    {


        ui->bttn_moveCamera->setEnabled(false);


        initial_z=100000;
        initial_x=25000;
        initial_y=50000;
        fwdMoveCommand(0, 20000, 50000);

        //control->moveAbsolute(100000, 25000, 50000); 7

        sleep(5000);
        for(int i=0; i< x_num; i++)
        {
            for(int j=0; j<y_num; j++)
            {
                //for(int jp=0; jp< number_points; jp++ )
                //{
                    for(int k=0; k< z_num; k++)
                    {
                        //for(int ip=0; ip<number_points; ip++)
                        //{


                           //move the camera to location
                        //initial_z +
                          fwdMoveCommand( 0 + k*delta_x, 25000,50000);

                          qDebug() << "Steppp";
                          //fwdMoveCommand(initial_z + k*delta_x, initial_x + i*delta_x + ip*point_spacing, initial_y + j*delta_x +jp*point_spacing);
                          QProcess p;

                          QStringList params;

                          QString python =  QString("C:\\Users\\labuser\\Documents\\RobotCamera\\test.py");
                          QString fname =  QString("--fname");
                          QString directory_r=QString(file_name + "_x=%1_y=%2_z=%3.tif").arg(i).arg(j).arg(k); //.arg(ip).arg(jp);
                          //QString directory = QString("C:\\Users\\labuser\\Documents\\lucam_python\\test_%1_%2.tif").arg(i).arg(j);
                          params.append(python);
                          params.append(fname);
                          params.append(directory_r);
                          p.start("python",  params);
                          p.waitForFinished(-1);

                          QString p_stdout = p.readAll();

                          sleep(1500);

                       // }

                    }

                //}

            }

        }

    //}
    //emit txCameraCfgRequest();

}

void MainWindow::on_bttn_gain_clicked()
{
    if(!LucamCameraOpen(1))
    {
       QMessageBox::information(this, "Title", "Camera is not connected!");
    }
    else{
        set_exposure(ui->doubleSpinBox_EXP->value());
        set_gain(ui->doubleSpinBox_GAIN->value());
    }

}

void MainWindow::sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 2000);

}

void MainWindow::on_pushButton_open_file_clicked()
{
    if(dataFile.isOpen())
    {
        //if file is open, close it
       ui->pushButton_open_file->setText("Open File");
       dataFile.close();

    }
    else {
        //if file isn't open, open it

        file_name = QFileDialog::getSaveFileName(this, "Specify a file", QDir::homePath());

        //QMessageBox::information(this, "...", file_name);

        //open file
        dataFile.setFileName(file_name);
        if(!dataFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
             QMessageBox::warning(this, "Title", "File not open");
        }
        else {

            dataStream.setDevice(&dataFile);
            pressed=false;

            ui->pushButton_open_file->setText("Close File");

        }

    }

}


void MainWindow::on_pushButton_setInitials_clicked()
{

    //Z-x, y-i, z-j, the robot's x, y and z are changed
    int initial_X= ui->spinBox_X->value()*1000;
    int initial_Y=ui->spinBox_Y->value()*1000;
    int initial_Z =ui->spinBox_Z->value()*1000;


    initial_x=initial_X;
    initial_y=initial_Y;
    initial_z=initial_Z;
}

void MainWindow::on_pushButton_setSteps_clicked()
{


    int change_x=ui->spinBox_changeX->value()*1000;

    int spacing =ui->spinBox_spacing->value()*1000;

    int number=ui->spinBox_number->value();

    number_points=number;
    point_spacing=spacing;
    delta_x=change_x;

}

void MainWindow::on_bttn_setNumberSteps_clicked()
{
    int number_x=ui->spinBox_numSteps->value();
    int number_y=ui->spinBox_numY->value();
    int number_z=ui->spinBox_numZ->value();

    x_num=number_x;
    y_num=number_y;
    z_num=number_z;
}

void MainWindow::on_bttn_close_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_clicked()
{

    if(!LucamCameraOpen(1))
    {
       QMessageBox::information(this, "Title", "Camera is not connected!");
    }
    else {
        ui->doubleSpinBox_GAIN->setValue(get_property(LUCAM_PROP_GAIN));
        ui->doubleSpinBox_EXP->setValue(get_property(LUCAM_PROP_EXPOSURE));
    }

}
