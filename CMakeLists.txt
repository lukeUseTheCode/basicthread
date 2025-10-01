# imposta una versione CMake minima necessaria per valutare lo script corrente.
cmake_minimum_required( VERSION 3.16.3 )

# avvia un nuovo progetto CMake. Ciò attiverà molta logica interna di
# CMake, in particolare il rilevamento del compilatore C e C ++ predefinito.
project( std_thread )

#include_directories( ${PROJECT_SOURCE_DIR} )

#set(CMAKE_CXX_STANDARD 20)
#set( CMAKE_CXX_STANDARD_REQUIRED ON )


#for g++ compiler
#
#GPT-4o mini
#Here’s a detailed list of warning flags specifically for the g++ compiler (GNU C++ Compiler). These flags can help you enable various warnings to improve code quality and catch potential issues during compilation.
#
#Warning Flag	Description
#-Wall	Enables most warning messages.
#-Wextra	Enables additional warning messages beyond -Wall.
#-Wpedantic	Enforces strict ISO C++ compliance.
#-Werror	Treats all warnings as errors, causing compilation to fail if any warnings are present.
#-Wshadow	Warns when a variable shadows another variable.
#-Wconversion	Warns for implicit conversions that may alter values.
#-Wsign-conversion	Warns for implicit conversions between signed and unsigned types.
#-Wfloat-equal	Warns when comparing floating-point values for equality.
#-Wuninitialized	Warns about uninitialized variables.
#-Wdeprecated-declarations	Warns about the use of deprecated declarations.
#-Wnull-dereference	Warns about dereferencing null pointers.
#-Wformat	Warns about format string issues.
#-Wlogical-op	Warns about logical operations that may be problematic.
#-Wcast-align	Warns about pointer casts that may result in misalignment.
#-Wstrict-aliasing	Warns about violations of strict aliasing rules.
#-Wmissing-declarations	Warns if a function is defined without a prior declaration.
#-Wmissing-include-dirs	Warns if a header file is included without its directory being specified.
#-Wfloat-conversion	Warns about conversions between floating-point types that may lose precision.
#-Wduplicated-branches	Warns about duplicated branches in conditional statements.
#-Wformat-security	Warns about potential security issues with format strings.
#-Wcast-qual	Warns about casting away const or volatile qualifiers.
#-Wnon-virtual-dtor	Warns if a class with virtual functions does not have a virtual destructor.

# Set warning flags for g++
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
    -Wextra -Wpedantic -Werror -Wshadow -Wconversion -Wsign-conversion \
    -Wfloat-equal -Wuninitialized -Wdeprecated-declarations -Wnull-dereference \
    -Wformat -Wlogical-op -Wcast-align -Wstrict-aliasing -Wmissing-declarations \
    -Wmissing-include-dirs -Wfloat-conversion -Wduplicated-branches -Wformat-security -Wcast-qual -Wnon-virtual-dtor ")
endif()

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -O3 -g3 " )
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -O3 -g3 -pedantic -Wall -Wextra" )
#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O3 -g3 -pedantic -Wall -Wextra -fpermissive" )

# source file
file( GLOB_RECURSE STD_THREAD_SRC "./src/*.cpp" )

# executable
set( STD_THREAD_BIN std_thread_bin )
 
# Con add_executable(app main.cpp) viene creata app destinazione build che invoca il
# compilatore configurato con alcuni flag predefiniti per l'impostazione corrente per compilare
# app eseguibile dal file di origine specificato main.cpp .
add_executable( ${STD_THREAD_BIN} ${STD_THREAD_SRC} )
