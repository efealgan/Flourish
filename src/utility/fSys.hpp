#ifndef FSYS_HPP
#define FSYS_HPP

#include <filesystem>
#include <vector>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else //linux
#include <limits.h>
#include <unistd.h>
#endif

/**
 * @brief Gets the path of the executable with filename. ../bin/Flourish.exe etc.
 */
std::filesystem::path getExePath();

/**
 * @brief Gets the directory containing the executable.
 * @return std::filesystem::path Executable path without filename.
 */
std::filesystem::path getExeDir();

/**
 * @brief Gets the current working directory.
 * @return std::filesystem::path
 */
std::filesystem::path getCWD();

/** 
 * @brief Sets current working directory to the provided path.
 * @param path Path to set CWD. eg. "C:/Users"
 */
void setCWD(std::filesystem::path path);

/**
 * @brief Finds a file by searching up the directory tree from a starting point
 * @param filename Name of file to find (e.g., "data/goods.txt")
 * @param startPath Starting directory (default: current directory)
 * @param maxLevels Maximum levels to search up (default: 5)
 * @return std::filesystem::path Empty path if not found
 */
std::filesystem::path findFileUpwards(const std::string& filename, const std::filesystem::path& startPath = getCWD(), int maxLevels = 5);

/**
 * @brief Creates a directory and all parent directories if they don't exist
 * @param dirPath Directory path to create
 * @return bool True if successful or directory already exists
 */
bool ensureDirectoryExists(const std::filesystem::path& dirPath);

/**
 * @brief Gets the project root directory by looking for marker files
 * @param markerFiles Files that indicate project root (e.g., CMakeLists.txt, .git)
 * @param startPath Starting search path (default: executable directory)
 * @return std::filesystem::path Empty path if not found
 */
std::filesystem::path getProjectRoot(const std::vector<std::string>& markerFiles = {"CMakeLists.txt", ".git", "README.md"},
                                    const std::filesystem::path& startPath = getExeDir());

/**
 * @brief Resolves a relative path from the project root (development only)
 * @param relativePath Path relative to project root (e.g., "data/goods.txt")
 * @return std::filesystem::path Absolute path, or empty if project root not found
 */
std::filesystem::path resolveProjectPath(const std::string& relativePath);

/**
 * @brief Resolves a relative path from the executable directory (production)
 * @param relativePath Path relative to executable (e.g., "data/goods.txt")
 * @return std::filesystem::path Absolute path relative to executable
 */
std::filesystem::path resolveGamePath(const std::string& relativePath);

/**
 * @brief Finds a game file by trying multiple locations (exe dir, cwd, etc.)
 * Development fallbacks are disabled when FLOURISH_PRODUCTION_MODE is defined
 * @param relativePath Path to find (e.g., "data/goods.txt")
 * @return std::filesystem::path Full path if found, empty path if not found
 */
std::filesystem::path findGameFile(const std::string& relativePath);

/**
 * @brief Verbose version of findGameFile that shows search attempts
 * @param relativePath Path to find (e.g., "data/goods.txt")
 * @param showAttempts Whether to print search attempts to console
 * @return std::filesystem::path Full path if found, empty path if not found
 */
std::filesystem::path findGameFileVerbose(const std::string& relativePath, bool showAttempts = true);

/**
 * @brief Initializes game paths by setting CWD to executable directory
 * Call this at startup for consistent file loading behavior
 */
void initializeGamePaths();

/**
 * @brief Check if compiled in production mode (FLOURISH_PRODUCTION_MODE defined)
 * @return bool True if production mode, false if development mode
 */
bool isProductionMode();

/**
 * @brief Safely checks if a file exists and is readable
 * @param filePath Path to check
 * @return bool True if file exists and is readable
 */
bool isFileReadable(const std::filesystem::path& filePath);

/**
 * @brief Gets the user's home directory (cross-platform)
 * @return std::filesystem::path Home directory path
 */
std::filesystem::path getHomeDirectory();

#endif //FSYS_HPP
