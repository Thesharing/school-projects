#include "cloth.h"

void cloth::setRate(float rate){
    this->rate=rate;
}

float cloth::getPrice(){
    return rate*fullPrice;
}

int cloth::getType(){
    return type;
}
