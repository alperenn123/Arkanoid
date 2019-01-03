# Only the root CMake file must start with a version declaration
cmake_minimum_required(VERSION 3.7)
# Name of project (executable)
project(arkanoid)

set(SFML_DIR "C:\\vcpkg\\packages\\sfml_x86-windows\\share\\sfml")
set(CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake_modules"
     ${CMAKE_MODULE_PATH})
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

# Add all SOURCE files to object and link
add_executable(arkanoid src/Arkanoid.cpp)
target_compile_features(arkanoid
                        PRIVATE
                        cxx_std_17)
target_link_libraries(arkanoid sfml-system sfml-window sfml-graphics)