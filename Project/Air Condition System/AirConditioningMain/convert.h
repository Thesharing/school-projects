#ifndef CONVERT_H
#define CONVERT_H

#include "status.h"
#include "wind.h"
#include "workmode.h"
#include "strategy.h"
#include <QString>

QString convertConnStatusToQStr(ConnectionStatus status);

QString convertWindToQStr(Wind wind);

QString convertWorkModeToQStr(WorkMode workmode);

QString convertStrategyToQStr(Strategy trategy);

Wind convertQStrToWind(QString windStr);


#endif // CONVERT_H
