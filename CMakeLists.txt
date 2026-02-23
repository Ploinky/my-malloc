cmake_minimum_required(VERSION 3.28)

project(scanner)

add_executable(scanner
    src/main.c
    src/my_mem.c
)

target_compile_options(scanner PRIVATE /W4 /WX)