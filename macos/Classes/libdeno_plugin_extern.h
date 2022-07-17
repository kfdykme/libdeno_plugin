#ifndef LIBDENO_PLUGIN_EXTERN_H
#define LIBDENO_PLUGIN_EXTERN_H


#ifdef __cplusplus
extern "C"{
#endif
void test_libdeno();


int test_libdeno_version(int v);

int load_libdeno_by_path(const char* path);

int lib_main_libdeno(const char* path);
#ifdef __cplusplus
}
#endif
//  void test_libdenoh() {
//     // test_libdeno();
//  }

#endif