# CMakeList.txt : CMake project for AloneCompiler, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.28)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AloneCompiler")

set(CMAKE_CXX_STANDARD 26)

include_directories(src)
include_directories(src/amasm)
include_directories(src/vm)
include_directories(src/vm/isa)

# Add source to this project's executable.
add_executable(AloneCompiler
        "src/error_codes.hpp"
        "src/general.hpp"
        "src/general.cpp"
        "src/instructions.cpp"
        "src/instructions.hpp"
        "src/main.cpp"
        "src/vm_context.cpp"
        "src/vm_context.hpp"
        "src/amasm/info/consts.cpp"
        "src/amasm/info/consts.hpp"
        "src/amasm/info/data_types.cpp"
        "src/amasm/info/data_types.hpp"
        "src/amasm/info/enums.hpp"
        "src/amasm/info/extras.cpp"
        "src/amasm/info/extras.hpp"
        "src/amasm/info/include.hpp"
        "src/amasm/info/tokens.cpp"
        "src/amasm/info/tokens.hpp"
        "src/amasm/lexer.cpp"
        "src/amasm/lexer.hpp"
        "src/amasm/parser.cpp"
        "src/amasm/parser.hpp"
        "src/amasm/scanner.cpp"
        "src/amasm/scanner.hpp"
        "src/amasm/util.hpp"
        "src/vm/isa/general.hpp"
        "src/vm/isa/linker.hpp"
        "src/vm/isa/system.hpp"
        "src/vm/virtual_machine.cpp"
        "src/vm/virtual_machine.hpp")

add_compile_definitions(DEBUG_STATUS)

# TODO: Add tests and install targets if needed.

target_include_directories(AloneCompiler PRIVATE
        src
        src/amasm
        src/vm
        src/vm/isa)

set_property(TARGET AloneCompiler PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/res")
