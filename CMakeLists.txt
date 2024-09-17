cmake_minimum_required(VERSION 3.28)
project(AloneCompiler)

set(CMAKE_CXX_STANDARD 26)

include_directories(src)
include_directories(src/amasm)
include_directories(src/vm)
include_directories(src/vm/isa)

add_executable(AloneCompiler
        src/amasm/data_types.hpp
        src/amasm/info.cpp
        src/amasm/info.hpp
        src/amasm/lexer.cpp
        src/amasm/lexer.hpp
        src/amasm/parser.cpp
        src/amasm/parser.hpp
        src/amasm/scanner.cpp
        src/amasm/scanner.hpp
        src/amasm/util.hpp
        src/vm/isa/general.hpp
        src/vm/isa/linker.hpp
        src/vm/isa/system.hpp
        src/vm/info.hpp
        src/vm/util.hpp
        src/vm/virtual_machine.cpp
        src/vm/virtual_machine.hpp
        src/general.hpp
        src/main.cpp)

target_include_directories(AloneCompiler PRIVATE
        src
        src/amasm
        src/vm
        src/vm/isa)