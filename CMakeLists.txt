cmake_minimum_required(VERSION 3.27)
project(sqlite_new)

set(CMAKE_CXX_STANDARD 23)

add_executable(sqlite_new main.cpp
        iterator.h
        request.h
        cell.h
        row.h
        sqlite3.c
        sqlite3.h
        db.h
        exception.h
        src/column.cpp)
add_subdirectory(Google_tests)