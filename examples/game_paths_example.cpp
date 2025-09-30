#include "utility/fSys.hpp"
#include "goods/goods.hpp"
#include <iostream>

int main() {
    // Initialize game paths for consistent behavior
    initializeGamePaths();
    
    std::cout << "=== Game Path Information ===\n";
    std::cout << "Executable: " << getExePath() << "\n";
    std::cout << "Exe Directory: " << getExeDir() << "\n";
    std::cout << "Working Directory: " << getCWD() << "\n";
    
    try {
        // Production way: Load files relative to executable
        auto goodsPath = findGameFile("data/goods.txt");
        
        if (goodsPath.empty()) {
            std::cerr << "Error: Could not find data/goods.txt\n";
            std::cerr << "Make sure data/goods.txt is in the same directory as the executable\n";
            return 1;
        }
        
        std::cout << "Found goods file at: " << goodsPath << "\n";
        
        // Load goods using the found path
        auto goodsList = Goods::loadFromFile(goodsPath.string());
        
        std::cout << "Successfully loaded " << goodsList.size() << " goods\n";
        
        // Create user save directory
        auto saveDir = getHomeDirectory() / ".flourish" / "saves";
        if (ensureDirectoryExists(saveDir)) {
            std::cout << "Save directory ready: " << saveDir << "\n";
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
