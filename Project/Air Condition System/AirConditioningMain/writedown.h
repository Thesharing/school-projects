#ifndef WRITEDOWN_H
#define WRITEDOWN_H

#include "wind.h"
#include <QSqlQuery>
#include <QDateTime>
#include "convert.h"
#include <QVariant>

void writeDownBegin(int num, float beginTemp, Wind wind);

void writeDownEnd(int num, float stopTemp);

void writeDownPowerOn(int num);

void writeDownPowerOff(int num);

void selectBigger();

#endif // WRITEDOWN_H
