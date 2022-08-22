#include "libdeno_plugin_extern.h"


#include "stdio.h"
#include <dlfcn.h>
#include <iostream>
#include <thread>

void* libdeno_handle_ = NULL;

typedef void *deno_lib_socket_callback_func(const char* message);

void (*libmain_)(int8_t *cmd, size_t length) = NULL;
void (*init_lib_socket_send_out_callback_)(deno_lib_socket_callback_func func) = NULL;
void (*lib_socket_send_)(int8_t *cmd) = NULL;


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

    init_lib_socket_send_out_callback_ = (void (*)(deno_lib_socket_callback_func func))dlsym(libdeno_handle_, "init_lib_socket_send_out_callback");
    if (init_lib_socket_send_out_callback_ == NULL) {
        return -3;
    }

    lib_socket_send_ = (void (*)(int8_t *cmd))dlsym(libdeno_handle_, "lib_socket_send");
    if (lib_socket_send_ == NULL) {
        return -4;
    }
    return 0;   
}

std::thread* lib_deno_thread;
std::thread* lib_deno_thread2;


bool has_run_deno_thread = false;

ls_message_callback_t lib_deno_callback = nullptr;

void set_ls_message_callback(ls_message_callback_t callback) {
    lib_deno_callback = callback;
}
void send_ls_msg (const char* msg) {
    // while (true)
    // {
        std::string cmd_s = std::string(msg);
        int len = cmd_s.length() + 1;
        int8_t *cmd_i_p = new int8_t[len];
        for (int x = 0; x < len; x++)
        {
            cmd_i_p[x] = (int8_t)cmd_s.c_str()[x];
        }
        // memcpy(cmd_i_p, (const char *)cmd.c_str(), cmd.length());
        cmd_i_p[len] = (int8_t)0;
        // printf("send to libsocket\n");
        lib_socket_send_(cmd_i_p);
        // std::this_thread::sleep_for(std::chrono::seconds(3));
    // }
    
}

void (*f)(const char*) = [](const char* message)
{
    printf("inner cpp ls message response callback\n");
    lib_deno_callback((char*) message);
};


int lib_main_libdeno(const char* cmd) {
    if (has_run_deno_thread) {
        return -100;
    } 

    has_run_deno_thread = true;
    printf("cmd: \n%s\n\n", cmd);
    std::string cmd_s = std::string((char*)cmd);
    int len = cmd_s.length() + 1;
    int8_t *cmd_i_p = new int8_t[len];
    for (int x = 0; x < len; x++)
    {
        cmd_i_p[x] = (int8_t)cmd[x];
    }
    // memcpy(cmd_i_p, (const char *)cmd.c_str(), cmd.length());
    cmd_i_p[len] = (int8_t)0;
    // libmain_(cmd_i_p, 0);
    // printf("%d %d %d", sizeof(cmd)+1, sizeof(cmd_i_p) +1, cmd_s.length());



    init_lib_socket_send_out_callback_((deno_lib_socket_callback_func *)lib_deno_callback);
    lib_deno_thread = new std::thread(libmain_, cmd_i_p, 0);
    lib_deno_thread->detach();
    
    return 0;   
}