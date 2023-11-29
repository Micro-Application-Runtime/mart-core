#ifndef __MODULES__STD__CONSOLE_H__
#define __MODULES__STD__CONSOLE_H__

#include <quickjs.h>

JSValue js_console_log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);

#endif // __MODULES__STD__CONSOLE_H__