#ifndef __MODULES__STD__TIMER_H__
#define __MODULES__STD__TIMER_H__

#include <quickjs.h>
#include <uv.h>

JSValue js_setTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue js_clearTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue js_setInterval(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue js_clearInterval(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);

void close_all_timer(JSContext *ctx);

#endif // __MODULES__STD__TIMER_H__