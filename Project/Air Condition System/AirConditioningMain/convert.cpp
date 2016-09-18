#include "convert.h"

QString convertConnStatusToQStr(ConnectionStatus status){
    switch(status){
    case ConnectionStatus::connected:
        return "已连接";
    case ConnectionStatus::disconnected:
        return "已关机";
    case ConnectionStatus::lostconnect:
        return "失去连接";
    }
}

QString convertWindToQStr(Wind wind){
    switch(wind){
    case Wind::high:
        return "高速风";
    case Wind::medium:
        return "中速风";
    case Wind::low:
        return "低速风";
    case Wind::stop:
        return "停止";
    }
}

QString convertWorkModeToQStr(WorkMode workmode){
    switch(workmode){
    case WorkMode::Hot:
        return "制热";
    case WorkMode::Cold:
        return "制冷";
    }
}

QString convertStrategyToQStr(Strategy strategy){
    switch(strategy){
    case Strategy::FIFS:
        return "先来先服务";
    case Strategy::HP:
        return "高速风优先";
    case Strategy::RR:
        return "时间片轮转";
    }
}

Wind convertQStrToWind(QString windStr){
    if(QString::compare(windStr, "高速风")==0){
        return Wind::high;
    }
    else if(QString::compare(windStr, "中速风")==0){
        return Wind::medium;
    }
    else if(QString::compare(windStr,"低速风")==0){
        return Wind::low;
    }
    else
        return Wind::stop;
}

