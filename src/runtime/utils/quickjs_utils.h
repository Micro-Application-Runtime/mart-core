#ifndef __QUICKJS_UTILS_H__
#define __QUICKJS_UTILS_H__

#include <quickjs.h>

int quickjs_load_js_file(JSContext *ctx, const char *file_path);

#endif // __QUICKJS_UTILS_H__