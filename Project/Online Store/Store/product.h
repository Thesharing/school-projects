#ifndef PRODUCT_H
#define PRODUCT_H

/* product是food、cloth、book和elctro的基类
 * 定义了name-名字，description-描述，fullprice-价格
 * 和stock-库存，以及这些属性的get/set函数。
 * 最后还定义了一个getPrice的虚函数 */

#include <QString>

class product{
protected:
    QString name;
    QString description;
    float fullPrice;
    int stock;

public:
    product()=default;
    product(QString s1,QString s2,float f, int i):name(s1),description(s2),fullPrice(f),stock(i){
    }

    QString getName();
    void setName();
    QString getDescription();
    void setDescription(QString description);
    float getFullPrice();
    void setFullPrice(float price);
    int getStock();
    void setStock(int stock);
    virtual float getPrice();
};

#endif // PRODUCT_H
