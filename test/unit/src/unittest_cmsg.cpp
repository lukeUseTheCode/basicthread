/**
 * @file unittest_cmsg.hpp
 * @brief Implement namespace ns_unittest_cmgs and main for unit test of class cmg
 * @details
 * @author LukeUseTheCode
 * @date September 30, 2025
 * @version 1.0.0
 * 
 * @note
 * @warning
 * @see cbasicthread.h
 * @todo
 * @deprecated
 * @def
 */
#include <cstdlib>
#include "../../../src/cbasicthread.h"

namespace ns_unittest_cmg {
    class cunittest_cmg : public ns_basic_thread::cmsg {
    public:
        cunittest_cmg() : cmsg() { }
        ~cunittest_cmg() { }
    };

    //bool test_cmsg(long unsigned int& nthreads,const bool& enable_pcmg);
    template <bool enable_pcmg>
    bool test_cmsg(const long unsigned int& nthreads) {
        bool success = true;
        cunittest_cmg* pcmg = nullptr;
        if constexpr ( enable_pcmg ) {
            pcmg = new (std::nothrow) cunittest_cmg();
            if(pcmg == nullptr) {
                std::cerr << "ERROR: Memory Allocate failed operator new\n";
                return false;
            }
        }
        std::vector<std::thread> threads;
        std::vector<std::string> expectedMessages;

        // Thread to set messages
        threads.emplace_back([&nthreads,&pcmg,&expectedMessages]() {
            for (unsigned long int i = 0; i < nthreads; ++i) {
                std::stringstream ss;
                ss << "TEST_MESSAGE" << i;
                expectedMessages.push_back(ss.str()); // Store expected message
                if constexpr ( enable_pcmg )
                    pcmg->set_msg(ss);
                ns_basic_thread::cmsg::set_msg(ss);
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
            }
        });
        // Thread to get messages
        threads.emplace_back([&success,&nthreads,&pcmg,&expectedMessages]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Ensure set messages are ready
            for (long unsigned int i = 0; i < nthreads; ++i) {
                std::string msg = ns_basic_thread::cmsg::get_msg();
                //std::cout << "Retrieved: " << msg << std::endl;
                // Check if the retrieved message matches the expected messages
                if constexpr ( enable_pcmg ) {
                    if ( pcmg->get_msg().find(expectedMessages[i]) == std::string::npos &&
                        pcmg->get_msg().size() != expectedMessages[i].size() ) {
                        success = false;
                    }
                }
                if ( msg.find(expectedMessages[i]) == std::string::npos &&
                    msg.size() != expectedMessages[i].size() ) {
                    success = false; // Mark as failed if message not found
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(150)); // Simulate work
            }
        });
        // Join threads
        for (auto& th : threads)
            th.join();

        if constexpr ( enable_pcmg ) {
            if ( pcmg ) 
                delete pcmg;
            pcmg = nullptr;
        }
        return success; // Return true if all messages matched, false otherwise
    }
}

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
    std::cout << "Start unit test of cmsg class...\n";
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