cmake_minimum_required(VERSION 3.10)
project(Mnemosyne)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional: Automatic vcpkg toolchain detection
if(NOT CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Find required packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(GLEW REQUIRED)

# Add all source files
set(SOURCES
    "src/main.cpp"
    "src/Camera.cpp"
    "src/Shader.cpp"
    "src/Mesh.cpp"
    "src/Rooms.cpp"
)

# Create executable
add_executable(Mnemosyne ${SOURCES})

# Link libraries
target_link_libraries(Mnemosyne PRIVATE
    glfw
    glm::glm
    assimp::assimp
    nlohmann_json::nlohmann_json
    GLEW::GLEW
)

# Include directories
target_include_directories(Mnemosyne PRIVATE include)

# Add compile definitions
target_compile_definitions(Mnemosyne PRIVATE MNEMOSYNE_APP)
