#ifndef __SET_TIMEOUT_H__
#define __SET_TIMEOUT_H__

#include <quickjs.h>
#include <uv.h>

typedef struct
{
    JSContext *ctx;
    JSValue func;
} timer_callback_data_t;

JSValue js_setTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);

#endif // __SET_TIMEOUT_H__