# compilation in release mode
# cmake -DCMAKE_BUILD_TYPE=Release ..

# add information about source code
# cmake -DCMAKE_C_FLAGS=-g ..

# gdb ./main
# r / run
# bt / backtrace
# f / frame
# print var

cmake_minimum_required(VERSION 3.10)

project(Exercice3_Lecteurs_Ecrivains VERSION 1.0)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wpedantic -Wextra")

include_directories(PUBLIC include)

set(SOURCE_FILES
    src/main.c
    src/reader_writer_priority.c
    src/lightswitch.c
    src/data.c
    src/read_thread.c
    src/write_thread.c
    src/statistics.c)

find_package(Threads REQUIRED)

add_executable(reader_writer ${SOURCE_FILES})

target_link_libraries(reader_writer PRIVATE Threads::Threads)
