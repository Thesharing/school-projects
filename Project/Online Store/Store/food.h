#ifndef FOOD_H
#define FOOD_H

/* 继承自product的food子类，实现了getPrize的虚函数
 * 定义了rate和type等私有成员
 * 父类定义在product.h里 */

#include "product.h"
#include "type.h"

class food : public product{
private:
    float rate;
    static const int type=FOOD;
    QString classification;

public:
    food()=default;
    food(QString s1,QString s2,float f1, int i1, float f2, QString s3):product(s1,s2,f1,i1),rate(f2),classification(s3){
    }
    void setRate(float);
    int getType();
    float getPrice();
};

#endif // FOOD_H
