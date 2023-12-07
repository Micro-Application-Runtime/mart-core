#ifndef __RUNTIME__UTILS__QUICKJS_UTILS_H__
#define __RUNTIME__UTILS__QUICKJS_UTILS_H__

#include <quickjs.h>

int quickjs_load_js_file(JSContext *ctx, const char *file_path);

#endif // __RUNTIME__UTILS__QUICKJS_UTILS_H__