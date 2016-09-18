#include "book.h"

void book::setRate(float rate){
    this->rate=rate;
}

float book::getPrice(){
    return rate*fullPrice;
}

int book::getType(){
    return type;
}

