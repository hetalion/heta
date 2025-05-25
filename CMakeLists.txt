cmake_minimum_required(VERSION 3.31)
project(core C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(core
        include/het.h
        include/het_conf.h
        include/het_limits.h
        include/het_object.h
        include/het_opcodes.h
        include/het_prefix.h
        src/het_opcodes.c
        src/het_object.c
        include/het_tm.h)
