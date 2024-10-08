cmake_minimum_required(VERSION 3.28)
project(AloneCompiler)

set(CMAKE_CXX_STANDARD 26)

include_directories(src)
include_directories(src/amasm)
include_directories(src/vm)
include_directories(src/vm/isa)

add_executable(AloneCompiler
        src/general.hpp
        src/main.cpp
        src/amasm/lexer.hpp
        src/amasm/lexer.cpp
        src/amasm/parser.hpp
        src/amasm/parser.cpp
        src/amasm/scanner.hpp
        src/amasm/scanner.cpp
        src/amasm/util.hpp
        src/amasm/info/enums.hpp
        src/amasm/info/tokens.hpp
        src/amasm/info/tokens.cpp
        src/amasm/info/data_types.cpp
        src/amasm/info/data_types.hpp
        src/amasm/info/parse_rules.cpp
        src/amasm/info/parse_rules.hpp
        src/amasm/info/consts.cpp
        src/amasm/info/consts.hpp
        src/amasm/info/include.hpp
        src/vm/isa/general.hpp
        src/vm/isa/linker.hpp
        src/vm/isa/system.hpp
        src/vm/info.hpp
        src/vm/util.hpp
        src/vm/virtual_machine.hpp
        src/vm/virtual_machine.cpp
        src/error_codes.hpp)

add_compile_definitions(DEBUG_STATUS)

target_include_directories(AloneCompiler PRIVATE
        src
        src/amasm
        src/vm
        src/vm/isa)