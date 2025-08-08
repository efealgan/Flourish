#ifndef GOODS_HPP
#define GOODS_HPP

#include "../globals.hpp"
#include <string>

// Enum for good types - combine with class for best of both worlds
enum class GoodType{
    STRUCTURAL,
    CONSUMER_STAPLE,
    CONSUMER_DISCRETIONARY,
    UTILITIES,
    LUXURY,
    MILITARY,
    MEDICINE,
    MISC
};

enum class ManufacturingStep{
    RAW_MATERIAL,
    INTERMEDIARY,
    MANUFACTURED
};

class Goods{
    public:
        Goods(const std::string& name, int price, int weight, GoodType type);
        
        // Getters
        const std::string& getName() const {
            return goodName;
        }
        int getID() const {
            return goodID;
        }
        int getBasePrice() const {
            return basePrice;
        }
        int getWeight() const {
            return transportWeight;
        }
        GoodType getType() const {
            return goodType;
        }

    private:
        std::string goodName;
        const int goodID;
        int basePrice;
        int transportWeight;
        GoodType goodType;
        
        static int nextID;
};

#endif //GOODS_HPP