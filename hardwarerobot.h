#ifndef HARDWAREROBOT_H
#define HARDWAREROBOT_H

#include <QDialog>
#include <stdbool.h>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class hardwarerobot;
}

class hardwarerobot : public QDialog
{
    Q_OBJECT

public:
    explicit hardwarerobot(QWidget *parent = 0);
     void updateGui();
    ~hardwarerobot();

signals:
      void txRobotPortName(QString str);

public slots:
     void rxRobotConnected(bool connected);

private slots:
     void on_bttn_connect_clicked();

     void on_bttn_close_clicked();

private:
    Ui::hardwarerobot *ui;

    QSerialPortInfo *info;

    bool roboCon;
    int roboInd;
};

#endif // HARDWAREROBOT_H
