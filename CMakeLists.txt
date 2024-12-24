cmake_minimum_required(VERSION 3.29)
project(DataStructure_Assignment C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(
        DataStructure_Assignment
        TImoveis.c
        interface.c
        node.c
        bp_tree.c
)
