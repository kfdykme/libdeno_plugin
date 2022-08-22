#ifndef LIBDENO_PLUGIN_EXTERN_H
#define LIBDENO_PLUGIN_EXTERN_H


#ifdef __cplusplus
extern "C"{
#endif
void test_libdeno();


int test_libdeno_version(int v);

int load_libdeno_by_path(const char* path);

int lib_main_libdeno(const char* path);

void send_ls_msg (const char* msg);

typedef void(*ls_message_callback_t)(const char *);
void set_ls_message_callback(ls_message_callback_t);

#ifdef __cplusplus
}
#endif
//  void test_libdenoh() {
//     // test_libdeno();
//  }

#endif