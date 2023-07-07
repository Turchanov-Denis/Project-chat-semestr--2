cmake_minimum_required(VERSION 3.25)
project(server)

set(CMAKE_CXX_STANDARD 17)


# Добавьте библиотеку nlohmann/json в вашу цель сборки
include_directories("include")
add_executable(server main.cpp include/httplib.h include/sqlite/sqlite3.c DataBaseManager.h Logger.h)


