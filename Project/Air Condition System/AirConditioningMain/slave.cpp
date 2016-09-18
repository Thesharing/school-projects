#include "slave.h"

QString Slave::generateStr(){
    return "3+"+QString::number(cost)+"+"
            +QString::number(usage)+"+"
            +QString::number(master.refreshFreq)+"+"
            +QString::number(no)+"+"
            +QString::number((int)wind)
            +"+"+QString::number((int)master.workMode);
}
