# compilation in release mode
# cmake -DCMAKE_BUILD_TYPE=Release ..

# gdb ./race
# r / run
# bt / backtrace
# f / frame
# print var

cmake_minimum_required(VERSION 3.10)

project(Exercice1_Coureurs VERSION 1.0)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wpedantic -Wextra")

include_directories(PUBLIC include)

set(SOURCE_FILES
    src/main.c
    src/barrier.c)

find_package(Threads REQUIRED)

add_executable(race ${SOURCE_FILES})
target_link_libraries(race PRIVATE Threads::Threads)
