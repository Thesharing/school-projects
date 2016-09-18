#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>

/* account.h定义了银行账户信息
 * 其中默认构造函数直接使用C++11标准，account()=default */

class account{
private:
    QString num;            //卡号
    QString password;       //密码
    QString bank;           //银行
    QString id;             //身份证号
    QString contact;        //联系方式
    float balance;          //账户余额

public:
    /* 构造函数 */
    account()=default;
    account(QString s1, QString s2, QString s3, QString s4, QString s5, float f1):num(s1),password(s2),bank(s3),id(s4),contact(s5),balance(f1)
{}
    /* 私有成员的get/set函数 */
   QString getNum();
   void setNum(QString num);
   QString getPassword();
   void setPassword(QString password);
   QString getBank();
   void setBank(QString bank);
   QString getId();
   void setId(QString id);
   QString getContact();
   void setContact(QString contact);
   float getBalance();
   void setBalance(float balance);
   /* addBanlance和subBalance提供对账户余额的直接增减 */
   void addBalance(float add);
   bool subBalance(float sub);
};

#endif // ACCOUNT_H
