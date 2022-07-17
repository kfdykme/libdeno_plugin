#include "libdeno_plugin_extern.h"


#include "stdio.h"
#include <dlfcn.h>
#include <iostream>
#include <thread>

void* libdeno_handle_ = NULL;

void (*libmain_)(int8_t *cmd, size_t length) = NULL;

void test_libdeno(){
    printf("test_libdeno");
}

int test_libdeno_version (int v) {
    return  v;
    return  121;
}


int load_libdeno_by_path(const char* path) {
   libdeno_handle_ = dlopen(path, RTLD_LAZY | RTLD_LOCAL | RTLD_FIRST);
    if (libdeno_handle_ == NULL) {
        return  -1;
    }
    libmain_ = (void (*)(int8_t *, size_t))dlsym(libdeno_handle_, "lib_main"); 
    if (libmain_ == NULL) {
        return  -2;
    }
    return 0;
}

int lib_main_libdeno(const char* cmd) {
    printf("\n%s\n\n", cmd);
    int8_t *cmd_i_p = new int8_t[sizeof(cmd) + 1];
    for (int x = 0; x < sizeof(cmd) +1; x++)
    {
        cmd_i_p[x] = (int8_t)cmd[x];
    }
    // memcpy(cmd_i_p, (const char *)cmd.c_str(), cmd.length());
    cmd_i_p[sizeof(cmd) +1] = (int8_t)0;
    // libmain_(cmd_i_p, 0);

    std::cout<< "lib_main_libdeno" << std::endl;
    auto lib_thread_ = new std::thread(libmain_, cmd_i_p, 0);
    lib_thread_->detach();
    
    return 0;   
}