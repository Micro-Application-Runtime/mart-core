#ifndef __MODULES_STD_CONSOLE_LOG_H__
#define __MODULES_STD_CONSOLE_LOG_H__

#include <quickjs.h>

JSValue js_console_log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);

#endif // __MODULES_STD_CONSOLE_LOG_H__