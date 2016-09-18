#ifndef DATASTRUCT
#define DATASTRUCT

#include "book.h"
#include "cloth.h"
#include "food.h"
#include "electro.h"
#include "type.h"

struct Store{
    product * Product [100];
    int ProductCount;

    book * Book[100];
    int BookCount;

    cloth * Cloth[100];
    int ClothCount;

    food * Food[100];
    int FoodCount;

    electro * Electro[100];
    int ElectroCount;
};

#endif // DATASTRUCT
