#!/bin/bash
gcc -o movie_manager main/main.c implementations/Dados.c implementations/IdxPrimario.c implementations/IdxSecundario.c -I./includes
./movie_manager
