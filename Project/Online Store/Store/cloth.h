#ifndef CLOTH_H
#define CLOTH_H

/* 继承自product的cloth子类，实现了getPrize的虚函数
 * 定义了rate和type等私有成员
 * 父类定义在product.h里 */

#include "product.h"
#include "type.h"

class cloth : public product
{
private:
    float rate;
    static const int type=CLOTH;
    QString style;

public:
    cloth()=default;
    cloth(QString s1, QString s2,float f1, int i1, float f2,QString s3):product(s1,s2,f1,i1),rate(f2),style(s3){
    }
    void setRate(float);
    int getType();
    float getPrice();
};

#endif // CLOTH_H
