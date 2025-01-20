# CMakeList.txt : CMake project for AloneCompiler, include source and define
# project specific logic here.

cmake_minimum_required(VERSION 3.28)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("AloneCompiler")

set(CMAKE_CXX_STANDARD 23)

include_directories(src)
include_directories(src/compiler)
include_directories(src/vm)
include_directories(src/shared)

add_executable(AloneCompiler
	"src/compiler/context.cpp"
	"src/compiler/functions.cpp"
	"src/compiler/instructions.cpp"
	"src/compiler/tokens.cpp"
	"src/compiler/variables.cpp" 
	"src/compiler/lexer.cpp"
	"src/compiler/parser.cpp"
	"src/compiler/scanner.cpp"
	"src/compiler/translator.cpp"

	"src/executor/context.cpp"
	"src/executor/util.cpp"
	"src/executor/virtual_machine.cpp"

	"src/shared/bytecode.cpp"
	"src/shared/literals.cpp"
		
	"src/main.cpp"
 "src/executor/instructions.cpp")

add_compile_definitions(DEBUG_STATUS)

# TODO: Add tests and install targets if needed.

target_include_directories(AloneCompiler PRIVATE
		src
		src/compiler
		src/vm
		src/shared
)

set_property(TARGET AloneCompiler PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/res")
