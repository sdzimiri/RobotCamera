#ifndef ROBOTHARDWARE_H
#define ROBOTHARDWARE_H

#include <QMainWindow>
#include <QObject>
#include <QDialog>
#include <QWidget>


namespace Ui {

class robothardware;

}
class robothardware : public QDialog
{

    Q_OBJECT

public:
    explicit robothardware(QWidget *parent = 0);

    ~robothardware();
};

#endif // ROBOTHARDWARE_H
