/**
 * @file cbasicthread.h
 * @brief This source file contains the declaration of the ns_basic_thread namespace
 * @details
 * @author LukeUseTheCode
 * @date September 30, 2025
 * @version 1.0.0
 * 
 * @note Ensure that this source file header is included in the implementation file.
 * @warning
 * @see cbasicthread.cpp
 * @todo
 * @deprecated
 * @def
 * 
 * #if defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) 
 */
#ifndef CBASICTHREAD_H_
#define CBASICTHREAD_H_

#include <iostream>// std::cout
#include <thread>// std::thread
#include <mutex>// std::mutex
#include <condition_variable>// std::condition_variable
#include <list>
#include <utility>// std::pair
#include <sstream>
#include <any>// std::any
#include <map>//std::map
#include <vector>//std::vactor
#include <string> //std::string
#include <unordered_map>
#include <stdexcept>
#include <functional>

#if defined(__posix) || defined(__linux__) || defined(__uinux__)
    #include <unistd.h>
    #include <sys/syscall.h>
#endif

#if defined(_WIN64) || defined(_WIN32)
    #include <windows.h>
#endif

/**
 * @namespace ns_basic_thread
 * @brief Gather all class, struct, enum etc to handling thread
 */
namespace ns_basic_thread {
    /**
    * @class cmsg
    * @brief The class 'cmg' handles and synchronizes print of messages amount multi-threaded environment.
     */
    class cmsg {
    public:
        /**
         * @brief Costructure of cmg class
         */
        cmsg() { }
        /**
         * @brief Destructure of cmg class
         */
        ~cmsg() { }
        /**
         * @brief A thread-safe interface function for setting mss_msg
         * @param _ss stringsatream for input message
         */
        static void set_msg( std::stringstream& _ss ) {
            std::unique_lock<std::mutex> lck(mmtx_msg);
            mss_msg << _ss.str();
        }
        /**
         * @brief A thread-safe interface function for getting mss_msg
         * @return string of message
         */
        static std::string get_msg() {
            std::string ret;
            std::unique_lock<std::mutex> lck( mmtx_msg );
            ret = mss_msg.str();
            lck.unlock();
            return ret;
        }
    protected:
        /**
         * @brief mmtx_ms = mutex to synchronizes messages
         */
        static std::mutex mmtx_msg;
        /**
         * @brief mss_ms = stringstream to store and load messagges
         */
        static std::stringstream mss_msg;
    };

    /**
     * @class cfunctionpointer
     * @brief  The class 'cfuntionpointer' handles a function that will be executed in a thread implemented by the user/client."
     */
    template <typename ReturnType, typename... Args>
    class cfuntionpointer : public cmsg {
    public:
        /**
         * @brief Costructor of cfuntionpointer class
         */
        cfuntionpointer() { }
        /**
         * @brief Destructor of cfuntionpointer class
         */
        ~cfuntionpointer() { }
    private:
        /**
         * @brief Pointer to the thread function, implemented by client/user
         */
        ReturnType (*ptrfunc)(Args...);
        /**
         * @brief The mutex mtx_data allows synchronization of the map data 
         */
        std::mutex mtx_data;
        /**
         * @struct data_in_out
         * @brief
         */
        struct data_in_out {
            std::any returnValue;
            std::vector<std::any> arguments;
        };
        /**
         * @brief
         */
        std::map<std::string, data_in_out> data;
        /**
         * @brief
         */
        void set_return( std::string& key, ReturnType return_data ) {
            std::unique_lock<std::mutex> lck(mtx_data);
            data_in_out tmp_data;
            tmp_data = data[key];
            tmp_data.returnValue = return_data;
            data[key] = {tmp_data};
        }
        /**
         * @brief Function to retrieve arguments from the map
         */
        void get_arguments( std::string& key, Args&... args ) {
            std::unique_lock<std::mutex> lck(mtx_data);
            auto it = data.find(key);
            if (it == data.end())
                throw std::runtime_error("Key not found");
            const auto& arguments = it->second.arguments;
            if (arguments.size() != sizeof...(args))
                throw std::runtime_error("Argument count mismatch");
            unpack_arguments(arguments, args...);
        }
        /**
         * @brief Unpacking function
         */
        template<typename T>
        void unpack_arguments( const std::vector<std::any>& arguments, T& first, Args&... rest ) {
            if (arguments.size() == 0) 
                return;
            if (arguments[0].type() == typeid(T))
                first = std::any_cast<T>(arguments[0]);
            else
                throw std::runtime_error("Type mismatch first");
            unpack_arguments(std::vector<std::any>(arguments.begin() + 1, arguments.end()), rest...);
        }
        /**
         * @brief
         */
        template<typename T>
        void unpack_arguments( const std::vector<std::any>& arguments, T& last ) {
            if (arguments.size() == 0)
                return;
            if (arguments[0].type() == typeid(T))
                last = std::any_cast<T>(arguments[0]);
            else 
                throw std::runtime_error("Type mismatch last");
        }
    protected:
        /**
         * @brief
         * @param key [IN] String bind to arguments
         */
        void call_thread( std::string &key ) {
            // Create a tuple to hold the arguments
            std::tuple<Args...> args; // Use a tuple to hold the arguments
            if (ptrfunc) {
                get_arguments<ReturnType>(key, std::get<Args>(args)...); // Retrieve arguments
                ReturnType result = ptrfunc(std::get<Args>(args)...); // data_in_out the function
                set_return(key, result); // Store the result
            } else
                set_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << " Function not set!\n" );
        }
        /**
         * @brief
         */
        void set_arguments( std::string& key, Args&&... args ) {
            std::unique_lock<std::mutex> lck(mtx_data);
            data_in_out tmp_data;
            tmp_data.arguments = { std::any(std::forward<Args>(args))... };
            data[key] = {tmp_data};
        }
        /**
         * @brief
         */
        ReturnType get_return( std::string &key ) {
            ReturnType ret;
            std::unique_lock<std::mutex> lck(mtx_data);
            ret = data[key].returnValue;
            lck.unlock();
            return ret;
        }
        /**
         * @brief
         */
        void setFunction( ReturnType (*f)(Args...) ) { ptrfunc = f; }
    };

    /**
     * @class cbasicthread
     * @brief This class create, init, and handling the basic_thread_func
     */
    template <typename ReturnType, typename... Args>
    class cbasicthread : public cfuntionpointer<ReturnType, Args...> {
    public:
        /**
         * @enum THREAD_STATUS
         * @brief The possible statuses are:
         * - INIT: Thread is in the initialization status.
         * - CREATE: Thread is in the creation status.
         * - WAIT: Thread is in the waiting status.
         * - RUN: Thread is currently running.
         * - EXIT: Thread has exited.
         */
        enum class THREAD_STATUS : unsigned char{
            INIT,
            CREATE,
            WAIT,
            RUN,
            EXIT
        };
        /**
         * @enum THREAD_ERRCODE
         * @brief Describe possible errors generated by cbasicthread class
        */
        enum class THREAD_ERRCODE : unsigned char {
            CREATE_MEM_NO_ALLOWED
        };
        /**
         * @brief Costructure of cbasicthread class
         */
        cbasicthread();
        /**
         * @brief Destructure of cbasicthread class
         */
        virtual ~cbasicthread();
        /**
         * @brief Creates a thread set to cbasicthread::basic_thread_func and runs the thread while waiting for its status. 
         */
        int create();
        /**
         * @brief Runs processing thread set up by function pointer mp_thr.
         *          If no arguments are provided, they will not be stored in the map cfuntionpointer::data.
         * @param ckey [IN] Unique string to store args in map data
         * @param args [IN] Argument pass to thread
         */
        void run( std::string& ckey = 0, Args &...args );
        /**
         * @brief exit
         */
        void exit();
        /**
         * @brief  get_status
         * @return Returns thread status
         */
        THREAD_STATUS get_status();
        /**
         * @brief Returns whether the thread is in a waiting status.
         * @return TRUE The hread is waiting status.
         * @return TRUE The hread is waiting status.
         */
        bool get_status_wait();
        /**
         * @brief  Returns whether the thread is in a running status.
         * @return TRUE The hread is running status.
         * @return FALSE The hread is NOT running status.
         */
        bool get_status_run();
        /**
         * @brief  Returns whether the thread has exited.
         * @return TRUE The thread has exited
         * @return FALSE The thread has NOT exited
         */
        bool get_status_exit();
        /**
         * @brief
         * @return Returns executed thread counter
         */
        int get_executed_thread_counter();
        /**
         * @brief Returns thread id
         * @return Returns thread id
         */
        #if defined(__linux__)
        long int get_thread_id();
        #elif defined(__FreeBSD__)
        get_thread_id 
        #elif defined(__NetBSD__)
        get_thread_id
        #elif defined(__OpenBSD__)
        get_thread_id
        #else
        get_thread_id
        #endif
    private:
        /**
         * @brief Mutex for handling the critical section of mthr_name
         */
         std::mutex mmtx_name;
         /**
         * @brief Name of thread
         */
        std::string mthr_name;
        /**
         * @brief Setup name of thread, IT IS OPTIONAL
         */
        int set_thread_name( const std::string& _name );
        /**
         * @brief Get name of thread
         * @return Return string with name of thread otherwise its returns an empty string
         */
        std::string get_thread_name();
        /**
         * @brief Stores The thread Id assigned by operative system
         * @note std::thread::id mthrid ??
         */
        #if defined(__linux__)
        std::atomic<long int>mthr_id;
        #elif defined(__FreeBSD__)
        mthr_id;
        #elif defined(__NetBSD__)
        mthr_id
        return _lwp_self();
        #elif defined(__OpenBSD__)
        mthr_id
        #else
        mthr_id
        #endif
        /**
         * @brief Set the thread Id assigned by operative system into mthr_id
         */
        void set_thread_id();
        /**
         * @brief Set thread ID mthr_id
         */
        std::thread* mp_thr = nullptr;
        /**
         * @brief Commands for managing the thread basic_thread_func()
         */
        struct command_data{
            std::string key;
            std::pair< bool,bool > brun_bexit;
        };
        /**
         * @brief Mutex for handling the critical section of mcommand
         */
        std::mutex mmtx_command;
        /**
         * @brief List Command for the thread basic_thread_func().
         *        commands : run or exit.
         */
        std::list< command_data > mcommand;
        /**
         * @brief Returns the first element of the list "mcommand"
         * @param key_ [OUT] String of key for argument thread. String is empty if no arguments are passed to the thread.
         * @param run_ [OUT] True thread will be running status. FALSE thread in waiting status.
         * @param exit_ [OUT] TRUE thread will be exiting status (run_ must be true). FALSE thread will be running or waiting status.
         */
        void get_command( std::string& key_, bool& run_,bool& exit_ );
        /**
         * @brief Counter for the number of times cfuntionpointer::call_thread is called
         */
        std::atomic<int> mcnt_thr_executed;
        /**
         * @brief Increase counter mcnt_thread_executed
         */
        void increase_cnt_executed();
        /**
         * @brief Stores status of thread
         */
        std::atomic< enum cbasicthread::THREAD_STATUS > mthr_status;
        /**
         * @brief Set the status of thread
         * @param _s The status of thread
         */
        void set_status( enum cbasicthread::THREAD_STATUS _s );
        /**
         * @brief Mutex for handling critical section of Condition Variable mcv
         */
        std::mutex mmtx_cv;
        /**
         * @brief Condition Variable for thread basic_thread_func()
         */
        std::condition_variable mcv;
        /**
         * @brief The real thread where called cfuntionpointer::call_thread()
         */
        void basic_thread_func();
        /**
         * @brief Initialize thread
         */
        void init();
        /**
         * @brief Destroy thread, delete pointer mp_thr and set mp_thr to nullptr.
         */
        void destroy();
    };
} // namespace ns_manage_thread

std::mutex ns_basic_thread::cmsg::mmtx_msg;
std::stringstream ns_basic_thread::cmsg::mss_msg;

#endif // CBASICTHREAD_H_