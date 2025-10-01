cmake_minimum_required( VERSION 3.16.3 )
project( unit_test_thread )
# Set warning flags for g++
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
    -Wextra -Wpedantic -Werror -Wshadow -Wconversion -Wsign-conversion \
    -Wfloat-equal -Wuninitialized -Wdeprecated-declarations -Wnull-dereference \
    -Wformat -Wlogical-op -Wcast-align -Wstrict-aliasing -Wmissing-declarations \
    -Wmissing-include-dirs -Wfloat-conversion -Wduplicated-branches -Wformat-security -Wcast-qual -Wnon-virtual-dtor ")
endif()
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -O3 -g3" )

################################################################################
#
# Unit Test cmsg Class
# source file
file( GLOB_RECURSE UNIT_TEST_CMSG_SRC "./unit/src/unittest_cmsg.cpp" )
# executable
set( UNIT_TEST_CMSG_BIN unit_test_cmsg )
add_executable( ${UNIT_TEST_CMSG_BIN} ${UNIT_TEST_CMSG_SRC} )
#
#
#