cmake_minimum_required(VERSION 3.0)
project(TransformationVisualizer)

# Set cmake standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.hpp"
)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(SFML_DIR external/SFML)
option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
option(IMGUI_SFML_IMGUI_DEMO "Build imgui_demo.cpp" ON)

# Add external libraries
add_subdirectory(external/SFML)
add_subdirectory(external/imguiSfml)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})




# Link libraries
target_link_libraries(${PROJECT_NAME}
    PUBLIC ImGui-SFML::ImGui-SFML
)

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
        external/SFML/include
        external/imgui
        external/imguiSfml
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
