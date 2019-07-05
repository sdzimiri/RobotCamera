#include "hardwarerobot.h"
#include "ui_hardwarerobot.h"
#include <QSerialPortInfo>
#include <QThread>
#include <QMessageBox>
#include <QDebug>


//default constructor
hardwarerobot::hardwarerobot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::hardwarerobot)
{
    ui->setupUi(this);

    //search for existing com ports
    QList<QSerialPortInfo> infoList= QSerialPortInfo::availablePorts();

    QStringList portList;
    for (int i =0; i < infoList.size();i++)
    {
        QSerialPortInfo p = infoList[i];
        portList.append(p.portName());
    }

    //Populate fields with ports
    ui->comboBox_port->addItems(portList);

    //initialize member variables
    roboCon = false;
    roboInd = -1;

}

//called to update the GUI based upon the connection
void hardwarerobot::updateGui()
{

    //if the robot is connected
    if(roboCon)
    {
        ui->comboBox_port->setCurrentIndex(roboInd);
        ui->label_connect->setText("Connected");
        ui->bttn_connect->setEnabled(false);
        ui->comboBox_port->setEnabled(false);

    }

}

hardwarerobot::~hardwarerobot()
{
    delete ui;
}

void hardwarerobot::on_bttn_connect_clicked()
{
    //get port name from combo box
    QString portname=ui->comboBox_port->currentText();
    emit txRobotPortName(portname);

    roboInd = ui->comboBox_port->currentIndex();

    ui->label_connect->setText("Connecting...");
    ui->bttn_connect->setEnabled(false);

}
//Slot for receiving connection status of robot
void hardwarerobot::rxRobotConnected(bool connected)
{
    if (connected)
    {
        ui->label_connect->setText("Connected");
        ui->bttn_connect->setEnabled(false);
        ui->comboBox_port->setEnabled(false);
        roboCon = true;
    }
    else
    {
        ui->label_connect->setText("Disconnected");
        ui->bttn_connect->setEnabled(true);
        roboCon = false;
    }
}

void hardwarerobot::on_bttn_close_clicked()
{
     this->close();
}
