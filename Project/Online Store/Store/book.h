#ifndef BOOK_H
#define BOOK_H

#include "product.h"

#include "type.h"

class book : public product
{
private:
    float rate;
    static const int type=BOOK;
    QString author;

public:
    book()=default;
    book(QString s1,QString s2,float f1, int i1, float f2,QString s3):product(s1,s2,f1,i1),rate(f2),author(s3){
    }
    void setRate(float);
    int getType();
    float getPrice();
};

#endif // BOOK_H
