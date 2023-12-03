cmake_minimum_required(VERSION 3.20) # Specify the minimum version for CMake

project(C-Final-Project) # Replace with your project name
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(${PROJECT_NAME}
        main.cpp
        lib/src/data_processor.cpp
        lib/include/data_processor.h)


add_subdirectory(matplotplusplus-master)

target_link_libraries(${PROJECT_NAME} PUBLIC matplot)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

target_include_directories(${PROJECT_NAME} PRIVATE lib/include)
