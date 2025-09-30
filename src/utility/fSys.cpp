#include "fSys.hpp"
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>

std::filesystem::path getExePath(){
#ifdef _WIN32
    wchar_t path[MAX_PATH] = { 0 };
    GetModuleFileNameW(NULL, path, MAX_PATH);
    return path;
#else
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
#endif
}

std::filesystem::path getExeDir(){
    return getExePath().parent_path();
}

std::filesystem::path getCWD(){
    auto path = std::filesystem::current_path();
    return path;
}

void setCWD(std::filesystem::path path){
    std::filesystem::current_path(path);
}

std::filesystem::path findFileUpwards(const std::string& filename, const std::filesystem::path& startPath, int maxLevels){
    std::filesystem::path currentPath = std::filesystem::absolute(startPath);
    
    for (int level = 0; level < maxLevels; ++level) {
        std::filesystem::path candidate = currentPath / filename; //The operator "/" handles intelligent path concatenation. 
        
        if (std::filesystem::exists(candidate)) {
            return candidate;
        }
        
        std::filesystem::path parent = currentPath.parent_path();

        if (parent == currentPath) {
            //Reached root
            break;
        }
        currentPath = parent;
    }
    return {}; //Return empty if not found.
}

bool ensureDirectoryExists(const std::filesystem::path& dirPath){
    try {
        return std::filesystem::exists(dirPath) || std::filesystem::create_directories(dirPath);
    } 
    catch (const std::filesystem::filesystem_error& fse){
                std::cerr << "Failed to create directory \"" << dirPath 
                          << "\": " << fse.what() << "\n";
    return false;
}
}

std::filesystem::path getProjectRoot(const std::vector<std::string>& markerFiles, const std::filesystem::path& startPath) {
    for (const auto& marker : markerFiles) {
        std::filesystem::path found = findFileUpwards(marker, startPath);
        if (!found.empty()) {
            return found.parent_path();
        }
    }
    return {}; // Not found
}

std::filesystem::path resolveProjectPath(const std::string& relativePath) {
    std::filesystem::path projectRoot = getProjectRoot();
    if (projectRoot.empty()) {
        return {}; // Project root not found
    }
    return projectRoot / relativePath;
}

std::filesystem::path resolveGamePath(const std::string& relativePath) {
    // For shipped games: resolve relative to executable directory
    return getExeDir() / relativePath;
}

std::filesystem::path findGameFile(const std::string& relativePath) {
    // First try relative to executable (production)
    std::filesystem::path exePath = getExeDir() / relativePath;
    if (isFileReadable(exePath)) {
        return exePath;
    }
    
#ifndef FLOURISH_PRODUCTION_MODE
    // Development fallbacks - only compiled in development builds
    
    // Fallback: try relative to current working directory (development)
    std::filesystem::path cwdPath = getCWD() / relativePath;
    if (isFileReadable(cwdPath)) {
        return cwdPath;
    }
    
    // Fallback: try one level up from executable (if exe is in build/bin/)
    std::filesystem::path upPath = getExeDir().parent_path() / relativePath;
    if (isFileReadable(upPath)) {
        return upPath;
    }
    
    // Fallback: try two levels up from executable (if exe is in build/bin/)
    std::filesystem::path upTwoPath = getExeDir().parent_path().parent_path() / relativePath;
    if (isFileReadable(upTwoPath)) {
        return upTwoPath;
    }
#endif
    
    return {}; // Not found
}

std::filesystem::path findGameFileVerbose(const std::string& relativePath, bool showAttempts) {
    if (showAttempts) {
        std::cout << "Searching for: " << relativePath << "\n";
    }
    
    // First try relative to executable (production)
    std::filesystem::path exePath = getExeDir() / relativePath;
    if (showAttempts) {
        std::cout << "  Trying (production): " << exePath << " - ";
    }
    if (isFileReadable(exePath)) {
        if (showAttempts) std::cout << "FOUND\n";
        return exePath;
    }
    if (showAttempts) std::cout << "not found\n";
    
#ifndef FLOURISH_PRODUCTION_MODE
    if (showAttempts) {
        std::cout << "  Production mode: DISABLED (development fallbacks enabled)\n";
    }
    
    // Development fallbacks - only compiled in development builds
    
    // Fallback: try relative to current working directory (development)
    std::filesystem::path cwdPath = getCWD() / relativePath;
    if (showAttempts) {
        std::cout << "  Trying (dev - cwd): " << cwdPath << " - ";
    }
    if (isFileReadable(cwdPath)) {
        if (showAttempts) std::cout << "FOUND\n";
        return cwdPath;
    }
    if (showAttempts) std::cout << "not found\n";
    
    // Fallback: try one level up from executable (if exe is in build/bin/)
    std::filesystem::path upPath = getExeDir().parent_path() / relativePath;
    if (showAttempts) {
        std::cout << "  Trying (dev - up1): " << upPath << " - ";
    }
    if (isFileReadable(upPath)) {
        if (showAttempts) std::cout << "FOUND\n";
        return upPath;
    }
    if (showAttempts) std::cout << "not found\n";
    
    // Fallback: try two levels up from executable
    std::filesystem::path upTwoPath = getExeDir().parent_path().parent_path() / relativePath;
    if (showAttempts) {
        std::cout << "  Trying (dev - up2): " << upTwoPath << " - ";
    }
    if (isFileReadable(upTwoPath)) {
        if (showAttempts) std::cout << "FOUND\n";
        return upTwoPath;
    }
    if (showAttempts) std::cout << "not found\n";
    
#else
    if (showAttempts) {
        std::cout << "  Production mode: ENABLED (no development fallbacks)\n";
    }
#endif
    
    if (showAttempts) {
        std::cout << "  Result: FILE NOT FOUND\n";
    }
    return {}; // Not found
}

void initializeGamePaths() {
    // Set working directory to executable directory for consistent behavior
    setCWD(getExeDir());
}

bool isProductionMode() {
#ifdef FLOURISH_PRODUCTION_MODE
    return true;
#else
    return false;
#endif
}

bool isFileReadable(const std::filesystem::path& filePath) {
    try {
        return std::filesystem::exists(filePath) && 
               std::filesystem::is_regular_file(filePath) &&
               (std::filesystem::status(filePath).permissions() & 
                std::filesystem::perms::owner_read) != std::filesystem::perms::none;
    } catch (const std::filesystem::filesystem_error&) {
        return false;
    }
}

std::filesystem::path getHomeDirectory() {
#ifdef _WIN32
    const char* userProfile = std::getenv("USERPROFILE");
    if (userProfile) {
        return std::filesystem::path(userProfile);
    }
    
    const char* homeDrive = std::getenv("HOMEDRIVE");
    const char* homePath = std::getenv("HOMEPATH");
    if (homeDrive && homePath) {
        return std::filesystem::path(homeDrive) / homePath;
    }
    
    return "C:\\"; // Fallback
#else
    const char* home = std::getenv("HOME");
    if (home) {
        return std::filesystem::path(home);
    }
    return "/tmp"; // Fallback
#endif
}
