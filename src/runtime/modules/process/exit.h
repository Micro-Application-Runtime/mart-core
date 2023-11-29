#ifndef __MODULES_PROCESS_EXIT_H__
#define __MODULES_PROCESS_EXIT_H__

#include <quickjs.h>

JSValue js_process_exit(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);

#endif // __MODULES_PROCESS_EXIT_H__