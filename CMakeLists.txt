﻿# CMakeList.txt : CMake project for AloneCompiler, include source and define
# project specific logic here.

cmake_minimum_required(VERSION 3.28)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("AloneCompiler")

set(CMAKE_CXX_STANDARD 23)
include(FetchContent)

FetchContent_Declare(
	FrozenLib
	GIT_REPOSITORY https://github.com/serge-sans-paille/frozen
	GIT_TAG master
)

FetchContent_MakeAvailable(FrozenLib)

include_directories(src)
include_directories(src/compiler)
include_directories(src/compiler/info)
include_directories(src/vm)
include_directories(src/library)

add_executable(AloneCompiler
	"src/compiler/info/constant_builder.cpp"
	"src/compiler/info/datatype_builder.cpp"
	"src/compiler/info/function_builder.cpp"
	"src/compiler/info/instruction_builder.cpp"
	"src/compiler/info/inst_info_factory.cpp"
	"src/compiler/info/scope_element.cpp"
	"src/compiler/info/scope_proxy.cpp"
	"src/compiler/info/tokens.cpp"
	"src/compiler/info/variable_builder.cpp"
	 
	"src/compiler/context.cpp"
	"src/compiler/lexer.cpp"
	"src/compiler/parser.cpp"
	"src/compiler/scanner.cpp"
	"src/compiler/translator.cpp"

	"src/executor/context.cpp"
	"src/executor/instruction_builder.cpp"
	"src/executor/isa_factory.cpp"
	"src/executor/util.cpp"
	"src/executor/virtual_machine.cpp"

	"src/library/logger.cpp"
	
	"examples/main_example.cpp"
)

add_compile_definitions(DEBUG_STATUS)
#add_compile_definitions(CONSTEXPR_COMPILE_DATA)

target_include_directories(AloneCompiler PRIVATE
	examples
	src/compiler
	src/compiler/info
	src/vm
	src/library
)
target_link_libraries(AloneCompiler PRIVATE frozen-headers)

set_property(TARGET AloneCompiler PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/res")
