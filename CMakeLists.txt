# CMakeList.txt : CMake project for AloneCompiler, include source and define
# project specific logic here.

cmake_minimum_required(VERSION 3.28)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("AloneCompiler")

set(CMAKE_CXX_STANDARD 26)

include_directories(src)
include_directories(src/amasm)
include_directories(src/amasm/info)
include_directories(src/vm)
include_directories(src/shared)

# Add source to this project's executable.
add_executable(AloneCompiler
		"src/amasm/info/context.cpp"
		"src/amasm/info/instructions.cpp"
		"src/amasm/info/tokens.cpp"
		"src/amasm/info/variables.cpp" 
		"src/amasm/scanner.cpp"
		"src/amasm/lexer.cpp"
		"src/amasm/parser.cpp"

		"src/shared/bytecode.cpp"
		"src/shared/literals.cpp"
		
		"src/main.cpp")

add_compile_definitions(DEBUG_STATUS)

# TODO: Add tests and install targets if needed.

target_include_directories(AloneCompiler PRIVATE
		src
		src/amasm
		src/amasm/info
		src/vm
		src/shared)

set_property(TARGET AloneCompiler PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/res")
