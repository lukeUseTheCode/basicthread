/**
 * @file cbasicthread.cpp
 * @brief This source file contains the declaration of the ns_basic_thread namespace
 * @details
 * @author LukeUseTheCode
 * @date September 30, 2025
 * @version 1.0.0
 * 
 * @note Ensure that this source file header is included in the implementation file.
 * @warning
 * @see cbasicthread.h
 * @todo
 * @deprecated
 */

#include "cbasicthread.h"

namespace ns_basic_thread {

template <typename ReturnType, typename... Args>
cbasicthread<ReturnType,Args...>::cbasicthread()
    : cfuntionpointer<ReturnType, Args...>() {
    init();
}

template <typename ReturnType, typename... Args>
cbasicthread<ReturnType,Args...>::~cbasicthread() {
    destroy();
}

template <typename ReturnType, typename... Args>
void cbasicthread<ReturnType, Args...>::init() {
    mthr_id = -1;
    mp_thr = nullptr;
    mthr_status = THREAD_STATUS::INIT;
    mcnt_thr_executed.store(0,std::memory_order_relaxed);
    mcommand.clear();
    mthr_name = "";
    set_print_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "init called");
}

template <typename ReturnType, typename... Args>
int cbasicthread<ReturnType, Args...>::create(void) {
    set_print_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "create start\n");
    mp_thr = new (std::nothrow) std::thread(&cbasicthread::basic_thread_func,this);
    if(mp_thr == nullptr){
        set_print_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "ERROR: Memory Allocate failed operator new\n\n");
        return static_cast<int>(THREAD_ERRCODE::CREATE_MEM_NO_ALLOWED);
    }
    /*try{
        mp_thr = new std::thread(&cbasicthread::basic_thread_func,this);
    }
    catch (const std::bad_alloc& e){
        std::cout << __LINE__ << __FILE__ << " \n !!! ERROR operator [new] failed !!!! " << e.what() << std::endl;
    }*/
    set_status(THREAD_STATUS::CREATE);
    while ( !get_status_wait() ) { ; }
    set_print_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "create end\n");
    return EXIT_SUCCESS;
}

template <typename ReturnType, typename... Args>
void cbasicthread<ReturnType, Args...>::destroy(void) {
    set_print_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "destroy thread[" << mthr_id << "] start");
    if ( mp_thr != nullptr ) {
        exit();
        set_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "destroy thread[" << mthr_id << "] waiting until joinable");
        while(!mp_thr->joinable()) {;}
        mp_thr->join();
        set_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "destroy thread[" << mthr_id << "] joined");
        delete mp_thr;
    }
//  set_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << "destroy thread[" << mthr_id << "] end");
    init();
}

template <typename ReturnType, typename... Args>
void cbasicthread<ReturnType, Args...>::run( std::string& ckey, Args &...args ) {
    std::unique_lock<std::mutex> lck( mmtx_command );
    bool run = true;
    bool exit = false;

    command_data tmp_cmd;
    tmp_cmd.brun_bexit = std::make_pair(run, exit);

    // check if args is void
    if constexpr ((std::is_void_v<Args> || ...)) {
        set_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "At least one of the arguments is void.");
        tmp_cmd.key = ""; // set empty because there is not arguments
    }
    else {
        set_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "None of the arguments are void.");
        if( ckey.empty() )
            set_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "key is empty");
        else
            tmp_cmd.key = ckey;// set with value because there is arguments 
        set_arguments(ckey, std::forward<Args>(args)...);
    }

    mcommand.push_back( tmp_cmd );
    lck.unlock();

    std::unique_lock<std::mutex> lckcv( mmtx_cv );
    mcv.notify_one();
    lckcv.unlock();
}

template <typename ReturnType, typename... Args>
void cbasicthread<ReturnType, Args...>::exit() {
    std::unique_lock<std::mutex> lck( mmtx_command );
    bool run = true;
    bool exit = true;
    command_data tmp_cmd;
    tmp_cmd.key = "exit";
    tmp_cmd.brun_bexit = std::make_pair(run, exit);
    mcommand.push_back( tmp_cmd );
    lck.unlock();

    std::unique_lock<std::mutex> lckcv( mmtx_cv );
    mcv.notify_one();
    lckcv.unlock();
}

template <typename ReturnType, typename... Args>
void cbasicthread<ReturnType, Args...>::get_command(std::string& key_, bool& run_, bool& exit_ ) {
    std::unique_lock<std::mutex> lck( mmtx_command );
    if( mcommand.size() > 0 ) {
        command_data tmp = mcommand.front();
        mcommand.pop_front();
        key_ = tmp.key;
        run_ = tmp.brun_bexit.first;
        exit_ = tmp.brun_bexit.second;
    }
    else {
        // first time
        key_ = "";
        run_ = false;
        exit_ = false;
    }
}

template <typename ReturnType, typename... Args>
void cbasicthread<ReturnType, Args...>::basic_thread_func() {
    set_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "thread[" << mthr_id << "] start\n");
    bool brun = false;
    bool bexit = false;
    std::string key;
    mthr_id = get_thread_id();
    set_status(THREAD_STATUS::CREATE);

    while ( !bexit ) {
        set_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "thread[" << mthr_id << "] exit loop\n");
        get_command(key, brun, bexit );
        std::unique_lock<std::mutex> lck(mmtx_cv);
        while ( !brun ) {
            set_status(THREAD_STATUS::WAIT);
            set_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "waiting until run\n");
            mcv.wait( lck );
            get_command(key, brun, bexit );
        }
        lck.unlock();

        if( bexit ) {
            set_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "thread[" << mthr_id << "] command exit\n");
            break;
        } // !brun loop

        set_status(THREAD_STATUS::RUN);
        //////////////////////////////////////
        // do somethings start ...
        call_thread<ReturnType,Args...>(key);
        key.clear();
        key="";
        set_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "thread[" << mthr_id << "] executed\n");

        increase_cnt_executed();
        // do somethings ... end
        //////////////////////////////////////

    } // !bexit loop

    set_status(THREAD_STATUS::EXIT);
    set_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "thread[" << mthr_id << "] exit - end\n");
}

template <typename ReturnType, typename... Args>
#if defined(__linux__)
long int cbasicthread<ReturnType, Args...>::get_thread_id() {
#elif defined(__FreeBSD__)
get_thread_id 
#elif defined(__NetBSD__)
get_thread_id
#elif defined(__OpenBSD__)
get_thread_id
#else
get_thread_id
#endif
    return mthr_id.load(std::memory_order_relaxed);
}

template <typename ReturnType, typename... Args>
void cbasicthread<ReturnType, Args...>::set_thread_id() {
    //mthr_id = std::this_thread::get_id();
    #if defined(__linux__)
    mthr_id = syscall(SYS_gettid);
    #elif defined(__FreeBSD__)
    thr_self(&mthr_id);
    #elif defined(__NetBSD__)
    mthr_id
    return _lwp_self();
    #elif defined(__OpenBSD__)
    mthr_id
    #else
    mthr_id
    #endif
}

template <typename ReturnType, typename... Args>
int cbasicthread<ReturnType, Args...>::set_thread_name(const std::string& _name) {
    int ret = EXIT_SUCCESS;
    if ( mp_thr != nullptr ) {
        //#if defined(__linux__) || \ //	defined(__FreeBSD__) || \ //	defined(__NetBSD__) || \ //	defined(__OpenBSD__)
        #if defined(__posix) 
        //auto handle = mp_thr->native_handle();
        std::thread::native_handle_type handle = mp_thr->native_handle();
        // pthread_setname_np has different signatures on different platforms
        // On Linux, it takes (pthread_t, const char*)
        pthread_setname_np(handle,_name.substr(0, 15).c_str());  // Max 16 chars including null terminator
        #endif

        #if defined(_WIN64) || defined(_WIN32)
        const std::wstring name = _name;
        auto handle = static_cast<HANDLE>(mp_thr->native_handle());
        // SetThreadDescription is available on Windows 10 (1607+) and Windows Server 2016+
        HRESULT hr = SetThreadDescription(handle, name.c_str());
        if (FAILED(hr)) {
            set_print_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << L"Failed to set thread name\n" );
            ret = EXIT_FAILURE;
        }
        #endif
        
        if ( ret == EXIT_SUCCESS ) {
            std::unique_lock<std::mutex> lck(mmtx_name);
            mthr_name = _name;
        }
    }
    else {
        set_print_msg( std::stringstream() << __FILE__ << ":" << __LINE__ << "-" << __FUNCTION__ << "thread pointers is null\n" );
        ret = EXIT_FAILURE;
    }
    return ret;
}

template <typename ReturnType, typename... Args>
std::string cbasicthread<ReturnType, Args...>::get_thread_name( void ) {
    std::string ret = "";
    std::unique_lock<std::mutex> lck(mmtx_name);
    if ( mthr_name.size() > 0 )
        ret = mthr_name;
    lck.unlock();
    return ret;
}

template <typename ReturnType, typename... Args>
void cbasicthread<ReturnType, Args...>::set_status(cbasicthread::THREAD_STATUS _s) {
    mthr_status.store(_s,std::memory_order_relaxed); // se
}

template <typename ReturnType, typename... Args>
THREAD_STATUS cbasicthread<ReturnType, Args...>::get_status() {
    THREAD_STATUS ret = mthr_status.load(std::memory_order_relaxed);
    return ret;
}

template <typename ReturnType, typename... Args>
bool cbasicthread<ReturnType, Args...>::get_status_wait() {
    bool ret = false;
    ret = ( mthr_status.load(std::memory_order_relaxed) == THREAD_STATUS::WAIT ) ? true : false;
    return ret;
}

template <typename ReturnType, typename... Args>
bool cbasicthread<ReturnType, Args...>::get_status_run() {
    bool ret = false;
    ret = ( mthr_status.load(std::memory_order_relaxed) == THREAD_STATUS::RUN ) ? true : false;
    return ret;
}

template <typename ReturnType, typename... Args>
bool cbasicthread<ReturnType, Args...>::get_status_exit() {
    bool ret = false;
    ret = ( mthr_status.load(std::memory_order_relaxed) == THREAD_STATUS::EXIT ) ? true : false;
    return ret;
}

template <typename ReturnType, typename... Args>
void cbasicthread<ReturnType, Args...>::increase_cnt_executed() {
    int x;
    x = mcnt_thr_executed.load(std::memory_order_relaxed); // get value atomically
    x++;
    mcnt_thr_executed.store(x,std::memory_order_relaxed); // set value atomically
}



} /* namespace ns_manage_thread */