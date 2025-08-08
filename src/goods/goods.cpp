#include "goods.hpp"

// Static member initialization
int Goods::nextID = 1;

// Constructor implementation
Goods::Goods(const std::string& name, int price, int weight, GoodType type)
    : goodName(name), goodID(nextID++), basePrice(price), transportWeight(weight), goodType(type) {
}

// Market behavior method implementation
int Goods::getCurrentPrice(float marketModifier) const {
    return static_cast<int>(basePrice * marketModifier);
}

