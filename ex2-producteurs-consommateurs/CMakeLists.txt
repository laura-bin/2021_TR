# compilation in release mode
# cmake -DCMAKE_BUILD_TYPE=Release ..

# gdb ./main
# r / run
# bt / backtrace
# f / frame
# print var

cmake_minimum_required(VERSION 3.10)

project(Exercice2_Producteurs_Consommateurs VERSION 1.0)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wpedantic -Wextra")

include_directories(PUBLIC include)

set(SOURCE_FILES
    src/prodcon.c
    src/fifo.c)

find_package(Threads REQUIRED)

add_executable(prodcon ${SOURCE_FILES})

target_link_libraries(prodcon PRIVATE Threads::Threads)
