# 电子商务系统

## 简要介绍

C++课程设计作业。主要使用C++完成，基于Qt的图形化界面，使用SQLite作为数据库，涉及了基本的 `select` 、 `delete` 等语法。两个程序（Store和Bank）之间通过Socket通信，使用TCP协议。

![银行界面](http://7xr64j.com1.z0.glb.clouddn.com/school-project/ecommerce/1.png)

![商店界面](http://7xr64j.com1.z0.glb.clouddn.com/school-project/ecommerce/2.png)

## 架构

![架构图](http://7xr64j.com1.z0.glb.clouddn.com/school-project/ecommerce/%E7%BB%98%E5%9B%BE1.jpg)

### 数据结构

设计了银行账户类`account`，以及具有继承关系的商品类`product`以及它的子类们书籍类 `book`、食品类 `food`等等。

### 数据库

为账户信息、商品信息和购物车设置了表 `accountInfo`、`productInfo` 和 `cart`。通过商品id将 `productInfo` 和 `accountInfo`相关联，通过账户名将 `accountInfo` 和 `cart` 关联起来。

### Socket通信

使用了 Qt 内置的类 `QtTcpSocket` ，通过在银行设置 `QTcpServer` 作为服务器端，在商店设置 `QTcpSocket` 作为客户端，当用户购买物品结算时，商店向银行发出请求进行验证，验证成功则银行扣款，商店确认订单。
