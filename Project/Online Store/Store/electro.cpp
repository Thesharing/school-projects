#include "electro.h"

void electro::setRate(float rate){
    this->rate=rate;
}

float electro::getPrice(){
    return rate*fullPrice;
}

int electro::getType(){
    return type;
}
