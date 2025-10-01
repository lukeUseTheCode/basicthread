/**
 * @file unittest_cmsg.cpp
 * @brief
 * @details
 * @author LukeUseTheCode
 * @date September 30, 2025
 * @version 1.0.0
 * 
 * @note
 * @warning
 * @see unittest_cmg.hpp
 * @todo
 * @deprecated
 * @def
 * 
 */
#include "unittest_cmsg.hpp"
#include <cstdlib>

bool call_test_cmsg() ;
bool call_test_cmsg() {
    const long unsigned int nthreads = 157;
    //std::cout << "\tnum. of thread : " << nthreads << "\n";
    const bool enable_object_cmsg = true;
    /*if constexpr ( enable_object_cmsg )
        std::cout << "\tAllocated object cunittest_cmg\n";
    else
        std::cout << "\tNOT Allocated object cunittest_cmg\n";*/
    return ns_unittest_cmg::test_cmsg<enable_object_cmsg>(nthreads);
}

int main(int argc,char* argv[]) {
    std::cout << "\n";
    std::cout << "==========================================\n";
    std::cout << "Start unit test of thread ...\n";
    std::cout << "==========================================\n";
    std::cout << "\n";
    if( argc > 1 ) {
        std::cout << "list of arguments:\n";
        for (int i=0; i<argc; i++)
            std::cout << "\t[" << i << "] : " << argv[i] << std::endl;
    }

    std::cout << "call_test_cmsg : ";
    if ( call_test_cmsg() )
        std::cout << "Test passed! :)\n";
    else {
        std::cout << "Test failed! :(\n"; 
        return EXIT_FAILURE;
    }

    std::cout << std::endl;
    return EXIT_SUCCESS;
}