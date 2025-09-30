cmake_minimum_required(VERSION 3.15)
project(Flourish)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to enable production mode
option(FLOURISH_PRODUCTION_MODE "Enable production mode (disable development file fallbacks)" OFF)

# Set up clean output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Create the output directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Add SDL3 submodule
add_subdirectory(external/SDL)

add_executable(Flourish src/main.cpp src/goods/goods.cpp src/utility/fSys.cpp src/globals.cpp)

# Link SDL3 to your executable
target_link_libraries(Flourish SDL3::SDL3)

# Set the production mode flag if enabled
if(FLOURISH_PRODUCTION_MODE)
    target_compile_definitions(Flourish PRIVATE FLOURISH_PRODUCTION_MODE)
    message(STATUS "Flourish: Production mode ENABLED - development file fallbacks disabled")
else()
    message(STATUS "Flourish: Development mode - all file fallbacks enabled")
endif()

# Copy SDL3.dll to the clean output directory after building
add_custom_command(TARGET Flourish POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Copying SDL3.dll to output directory"
)

# Copy data files to build directory for testing production behavior
add_custom_command(TARGET Flourish POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:Flourish>/data
    COMMENT "Copying data files to build directory"
)

# Optional: Create a production build target
add_custom_target(Flourish_Production
    COMMAND ${CMAKE_COMMAND} -DFLOURISH_PRODUCTION_MODE=ON -B build_production -S ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build build_production
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Building Flourish in production mode"
)