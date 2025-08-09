#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include <filesystem>

#include "goods.hpp"

//Statics
int Goods::nextID = 1;

//Constructor
Goods::Goods(const std::string& name, int price, int weight, Sector sector)
    : goodName(name), goodID(nextID++), basePrice(price), transportWeight(weight), goodSector(sector) {
}

//Getters
const std::string& Goods::getName() const {
    return goodName;
}

int Goods::getID() const {
    return goodID;
}

int Goods::getBasePrice() const {
    return basePrice;
}

int Goods::getWeight() const {
    return transportWeight;
}

Sector Goods::getGoodSector() const {
    return goodSector;
}


//Load From File
Sector Goods::stringToSector(const std::string& sectorStr) {
    static const std::unordered_map<std::string, Sector> sectorMap = {
        {"AGRICULTURE", Sector::AGRICULTURE},
        {"BASIC_MATERIALS", Sector::BASIC_MATERIALS},
        {"CONSUMER_STAPLE", Sector::CONSUMER_STAPLE},
        {"CONSUMER_DISCRETIONARY", Sector::CONSUMER_DISCRETIONARY},
        {"INDUSTRIALS", Sector::INDUSTRIALS},
        {"UTILITIES", Sector::UTILITIES},
        {"MILITARY", Sector::MILITARY},
        {"HEALTHCARE", Sector::HEALTHCARE},
        {"MISC", Sector::MISC}
    };
    
    auto it = sectorMap.find(sectorStr);
    return (it != sectorMap.end()) ? it->second : Sector::MISC;
}


std::vector<std::unique_ptr<Goods>> Goods::loadFromFile(const std::string& filename) {
    std::vector<std::unique_ptr<Goods>> goodsList;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return goodsList;
    }
    
    std::string line;
    int lineNumber = 0;
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        // Remove carriage return for Windows compatibility
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::stringstream ss(line);
        std::string name, priceStr, weightStr, sectorStr;
        
        // Parse CSV: name,price,weight,sector
        if (std::getline(ss, name, ',') &&
            std::getline(ss, priceStr, ',') &&
            std::getline(ss, weightStr, ',') &&
            std::getline(ss, sectorStr)) {
            
            try {
                int price = std::stoi(priceStr);
                int weight = std::stoi(weightStr);
                Sector sector = stringToSector(sectorStr);
                
                goodsList.push_back(std::make_unique<Goods>(name, price, weight, sector));
                
            } catch (const std::exception& e) {
                std::cerr << "Error parsing line " << lineNumber << ": " << line 
                          << " - " << e.what() << "\n";
            }
        } else {
            std::cerr << "Error: Invalid format on line " << lineNumber << ": " << line << "\n";
        }
    }
    
    file.close();
    
    if (goodsList.empty()) {
        throw std::runtime_error("No valid goods were loaded from file: " + filename + ". Check file format and content.");
    }
    
    std::cout << "Loaded " << goodsList.size() << " goods from " << filename << "\n";
    return goodsList;
}

// Get the directory where the executable is located
std::string Goods::getExecutableDirectory() {
    try {
        std::filesystem::path execPath = std::filesystem::current_path();
        return execPath.string();
    } catch (const std::exception& e) {
        std::cerr << "Error getting executable directory: " << e.what() << "\n";
        return "."; // Fallback to current directory
    }
}

// Load from file relative to project root (more reliable)
std::vector<std::unique_ptr<Goods>> Goods::loadFromProjectFile(const std::string& relativePath) {
    // Try multiple possible project root locations
    std::vector<std::string> possiblePaths = {
        relativePath,                           // Current directory
        "../" + relativePath,                   // One level up (if running from build/)
        "../../" + relativePath,                // Two levels up (if running from build/bin/)
        std::filesystem::current_path().string() + "/" + relativePath
    };
    
    for (const auto& path : possiblePaths) {
        if (std::filesystem::exists(path)) {
            std::cout << "Found goods file at: " << path << "\n";
            return loadFromFile(path);
        }
    }
    
    // If no file found, show what we tried
    std::string errorMsg = "Could not find goods file. Tried paths:\n";
    for (const auto& path : possiblePaths) {
        errorMsg += "  - " + path + "\n";
    }
    throw std::runtime_error(errorMsg);
}

