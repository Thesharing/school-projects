#include "food.h"

void food::setRate(float rate){
    this->rate=rate;
}

float food::getPrice(){
    return rate*fullPrice;
}

int food::getType(){
    return type;
}
