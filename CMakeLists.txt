cmake_minimum_required(VERSION 3.10.0)

project(LearnOpenGL)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)

link_directories(
    "/opt/homebrew/lib/"
    "/usr/local/lib/"
)

include_directories(
    "./"
    "/opt/homebrew/include/"
    "/opt/homebrew/include/Imath/"
    "/usr/local/include/"
    "./stb"
)


# file(GLOB_RECURSE SRC_FILES *.cpp *.c)

file(GLOB SRC_FILES 
    ./*.cpp 
    ./*.c
    ./glad/*.c
    ./stb/*.c
)

add_executable(main.out ${SRC_FILES})

target_link_libraries(
    main.out
    "-framework IOKit"
    "-framework Cocoa"
    "-framework CoreVideo"
    "-framework OpenGL"
    "glfw"
    "assimp"
    "OpenEXR"
    
)




