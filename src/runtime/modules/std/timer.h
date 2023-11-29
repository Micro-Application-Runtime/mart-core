#ifndef __MODULES__STD__TIMER_H__
#define __MODULES__STD__TIMER_H__

#include <quickjs.h>

typedef struct
{
    JSContext *ctx;
    JSValue func;
} timer_callback_data_t;

JSValue js_setTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue js_clearTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue js_setInterval(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);

#endif // __MODULES__STD__TIMER_H__