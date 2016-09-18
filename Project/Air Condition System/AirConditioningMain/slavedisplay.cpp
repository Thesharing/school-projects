#include "slavedisplay.h"
#include "convert.h"


void SlaveDisplay::updateLabel(Slave slave){
    roomLabel->setText(QString::number(slave.no));
    statusLabel->setText(convertConnStatusToQStr(slave.connectionStatus));
    currentLabel->setText(QString::number(slave.currentTemperature));
    targetLabel->setText(QString::number(slave.targetTemperature));
    usageLabel->setText(QString::number(slave.usage));
    windLabel->setText(convertWindToQStr(slave.wind));
}
