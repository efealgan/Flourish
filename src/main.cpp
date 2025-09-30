// #include "globals.hpp"
// #include "goods/goods.hpp"
// #include "utility/fSys.hpp"

// #include <iostream>

// namespace fs = std::filesystem;

// int main(int argc, char *argv[]){
//     fs::path initialCWD = getCWD();
//     std::cout << "Program called from: " << initialCWD << ".\n";

//     fs::path exeLocation = getExePath();
    
//     std::cout << "Executable is located in: " << exeLocation << ".\n";
//     setCWD("C:\\");
// }

#include "utility/fSys.hpp"
#include "goods/goods.hpp"
#include <iostream>

int main() {
    std::cout << "=== Flourish File System Test ===\n";
    std::cout << "Production Mode: " << (isProductionMode() ? "ENABLED" : "DISABLED") << "\n\n";
    
    // Initialize game paths
    initializeGamePaths();
    
    // Show verbose search for debugging
    std::cout << "=== File Search Debug ===\n";
    auto goodsPath = findGameFileVerbose("data/goods.txt", true);
    
    if (goodsPath.empty()) {
        std::cout << "\n❌ Could not find data/goods.txt\n";
        std::cout << "Make sure to:\n";
        if (isProductionMode()) {
            std::cout << "  - Copy data/goods.txt to the same directory as the executable\n";
        } else {
            std::cout << "  - Run from project root, OR\n";
            std::cout << "  - Copy data/goods.txt to executable directory, OR\n";
            std::cout << "  - Define FLOURISH_PRODUCTION_MODE to test production behavior\n";
        }
        return 1;
    }
    
    std::cout << "\n✅ Found goods file!\n";
    std::cout << "Final path: " << goodsPath << "\n\n";
    
    try {
        auto goodsList = Goods::loadFromFile(goodsPath.string());
        std::cout << "✅ Successfully loaded " << goodsList.size() << " goods\n";
        
        // Show first few goods
        int count = 0;
        for (const auto& good : goodsList) {
            if (count++ >= 3) break; // Show first 3
            std::cout << "  - " << good->getName() 
                      << " ($" << good->getBasePrice() << ")\n";
        }
        
    } catch (const std::exception& e) {
        std::cout << "❌ Error loading goods: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
