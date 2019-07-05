#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <stdbool.h>
#include <windows.h>
#include <algorithm>
#include "SDK\Include\lucamapi.h"
#include "SDK\Include\lucamerr.h"
#include "SDK\Include\lucamsci.h"
#include "SDK\Include\lucamsequencing.h"

class cameracontrol : public QObject
{
    Q_OBJECT

public:
    cameracontrol();




signals:
    void PortConnected(bool b);
    void txStatus(bool moving);
    void errorGenerated();

public slots:
    void SetupCamera();
    void ConnectPort(QString port);
    void moveAbsolute(int x, int y, int z);
    void getStatus();
    void alarmReset();


private slots:
    void ReadData();
    void timeOut();
    void commandTimeOut();

private:

    HANDLE handle;
    LUCAM_SNAPSHOT pSettings;
    LUCAM_VERSION self;



    BYTE pData;
    BYTE pDest;
    //const CHAR *pFilename= "C:\\Users\\labuser\\Documents\\image.tif";
    LUCAM_IMAGE_FORMAT imageFormat;
    LUCAM_FRAME_FORMAT format;
    FLOAT pFrameRate;

    void VideoStream();
    void TakeSnap();
    void saveSnap();
    QTimer *queryTimer;
    QTimer *commandTimer;
    QString num2HexStr(int num);
    QSerialPort *serial;
    int state;  //0 = disconnected
                 //1 = waiting for ack
                 //2 = connected
                 //3 = waiting for robot status
    void addCheckSumWrite(QString str);
    enum state {disconnected, waiting, connected, waitforstat};
    bool commandTime;

};

#endif // CAMERACONTROL_H
