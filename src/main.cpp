/**
 * @file main.cpp
 * @brief
 * @details
 * @author LukeUseTheCode
 * @date September 30, 2025
 * @version 1.0.0
 * 
 * @note
 * @warning
 * @see cbasicthread.h cbasicthread.cpp
 * @todo
 * @deprecated
 * @def
 * 
 */
#include <iostream>
#include "cbasicthread.h"
int main(int argc, char* argv[]) {
    std::cout << " start main ...\n";
    if (argc > 0 ){
        std::cout << "List of arguments:\n";
        for (int i=0; i<argc; i++)
            std::cout << "[" << i << "] : " << argv[i] << std::endl;
    }
    std::cout << " done!!!\n";
    return EXIT_SUCCESS;
}
