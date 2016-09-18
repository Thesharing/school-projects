#ifndef SLAVEDISPLAY_H
#define SLAVEDISPLAY_H

#include <QLabel>
#include "slave.h"

class SlaveDisplay{
public:
    QLabel * roomLabel;
    QLabel * statusLabel;
    QLabel * currentLabel;
    QLabel * targetLabel;
    QLabel * usageLabel;
    QLabel * windLabel;

    SlaveDisplay(){
        roomLabel = nullptr;
        statusLabel = nullptr;
        currentLabel = nullptr;
        targetLabel = nullptr;
        usageLabel = nullptr;
        windLabel = nullptr;
    }

    SlaveDisplay(QLabel* room, QLabel* status, QLabel* current, QLabel* target,
                 QLabel* usage, QLabel* wind):roomLabel(room),statusLabel(status),currentLabel(current),
        targetLabel(target),usageLabel(usage),windLabel(wind){}
    void updateLabel(Slave slave);
};

#endif // SLAVEDISPLAY_H
