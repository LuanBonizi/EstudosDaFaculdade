cmake_minimum_required(VERSION 3.0)

project(movie_manager)

add_executable(movie_manager main/main.c implementations/Dados.c implementations/IdxPrimario.c implementations/IdxSecundario.c)

target_include_directories(movie_manager PUBLIC includes)


