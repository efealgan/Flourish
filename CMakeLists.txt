cmake_minimum_required(VERSION 3.15)
project(Flourish)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set up clean output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Create the output directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Add SDL3 submodule
add_subdirectory(external/SDL)

add_executable(Flourish src/main.cpp)

# Link SDL3 to your executable
target_link_libraries(Flourish SDL3::SDL3)

# Copy SDL3.dll to the clean output directory after building
add_custom_command(TARGET Flourish POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Copying SDL3.dll to output directory"
)