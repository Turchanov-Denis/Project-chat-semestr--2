cmake_minimum_required(VERSION 3.25)
project(server1)

set(CMAKE_CXX_STANDARD 17)


# Добавьте библиотеку nlohmann/json в вашу цель сборки
include_directories("C:\\Users\\turch\\Desktop\\study\\Study-programming\\Project-chat-semestr--2\\server1\\include")
add_executable(server1 main.cpp httplib.h)

