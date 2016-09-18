#include "product.h"

QString product::getName(){
    return name;
}

void product::setName(){
    this->name=name;
}

QString product::getDescription(){
    return description;
}

void product::setDescription(QString description){
    this->description=description;
}

float product::getFullPrice(){
    return fullPrice;
}

void product::setFullPrice(float price){
    this->fullPrice=price;
}

int product::getStock(){
    return stock;
}

void product::setStock(int stock){
    this->stock=stock;
}

float product::getPrice(){
    return 0;
}
