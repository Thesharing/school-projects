#include "account.h"

QString account::getNum(){
    return this->num;
}

void account::setNum(QString num){
    this->num=num;
}

QString account::getPassword(){
    return this->password;
}

void account::setPassword(QString password){
    this->password=password;
}

QString account::getBank(){
    return this->bank;
}

void account::setBank(QString bank){
    this->bank=bank;
}

void account::setId(QString id){
    this->id=id;
}

QString account::getId(){
    return this->id;
}

QString account::getContact(){
    return this->contact;
}

void account::setContact(QString contact){
    this->contact=contact;
}

float account::getBalance(){
    return this->balance;
}

void account::setBalance(float balance){
    this->balance=balance;
}

void account::addBalance(float add){
    this->balance+=add;
}

bool account::subBalance(float sub){
    if(this->balance<sub)
        return false;
    else{
        this->balance-=sub;
        return true;
    }
}

