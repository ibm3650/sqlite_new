cmake_minimum_required(VERSION 3.27)
project(sqlite_new)

set(CMAKE_CXX_STANDARD 23)

add_executable(sqlite_new main.cpp
        cell.h
        iterator.h
        request.h
        row.h
        sqlite3.c
        sqlite3.h
        db.h
        exception.h)
