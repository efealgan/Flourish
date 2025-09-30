#ifndef GOODS_HPP
#define GOODS_HPP

#include <string>
#include <unordered_set>
#include <vector>
#include <memory>

#include "../globals.hpp"

// Enum for good types - combine with class for best of both worlds
enum class Sector{
    AGRICULTURE,
    BASIC_MATERIALS,
    CONSUMER_STAPLE,
    CONSUMER_DISCRETIONARY,
    INDUSTRIALS,
    UTILITIES,
    MILITARY,
    HEALTHCARE,
    MISC
};

enum class GoodTier{
    RAW_MATERIAL,
    REFINED_MATERIAL,
    INTERMEDIATE,
    MANUFACTURED
};

class Goods{
    public:
        Goods(const std::string& name, int price, int weight, Sector sector);
        
        //Load from file
        static std::vector<std::unique_ptr<Goods>> loadFromFile(const std::string& filename);
        static Sector stringToSector(const std::string& sectorStr);

        //Getters
        const std::string& getName() const;
        int getID() const;
        int getBasePrice() const;
        int getWeight() const;
        Sector getGoodSector() const;
        
    private:
        std::string goodName;
        const int goodID;
        int basePrice;
        int transportWeight;
        Sector goodSector;
        
        static int nextID;
};

#endif //GOODS_HPP