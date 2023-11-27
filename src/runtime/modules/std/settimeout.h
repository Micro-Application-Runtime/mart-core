#ifndef __MODULES_STD_SETTIMEOUT_H__
#define __MODULES_STD_SETTIMEOUT_H__

#include <quickjs.h>

JSValue js_setTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);

#endif // __MODULES_STD_SETTIMEOUT_H__