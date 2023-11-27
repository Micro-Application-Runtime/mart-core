#include "settimeout.h"

#include <stdlib.h>
#include <uv.h>

typedef struct
{
    JSContext *ctx;
    JSValue func;
} timer_callback_data_t;

JSValue call_callback(JSContext *ctx, int argc, JSValueConst *argv)
{
    // 调用QuickJS的回调函数
    JSValue func = argv[0];
    JS_Call(ctx, func, JS_UNDEFINED, 0, NULL);

    // JSValue exception = JS_GetException(ctx);

    // if (!JS_IsNull(exception))
    // {
    //     const char *exception_str = JS_ToCString(ctx, exception);
    //     fprintf(stderr, "Error2 executing JavaScript: %s\n", exception_str);
    //     JS_FreeCString(ctx, exception_str);
    // }

    // JS_FreeValue(ctx, exception);
}

void setTimeout_callback(uv_timer_t *handle)
{
    timer_callback_data_t *data = (timer_callback_data_t *)handle->data;
    JSContext *ctx = data->ctx;
    JSValue func = data->func;
    JSValue args[] = {func};

    JS_EnqueueJob(ctx, call_callback, 1, args);

    // 释放资源
    JS_FreeValue(ctx, func);

    // 关闭和释放定时器句柄
    uv_close((uv_handle_t *)handle, NULL);
}

JSValue js_setTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    int64_t delay;
    JSValue func;
    timer_callback_data_t *data;

    if (argc < 2)
    {
        return JS_ThrowTypeError(ctx, "Invalid arguments");
    }

    // 获取延迟时间和回调函数
    if (!JS_IsFunction(ctx, argv[0]) || JS_ToInt64(ctx, &delay, argv[1]))
    {
        return JS_ThrowTypeError(ctx, "Invalid arguments");
    }

    data = malloc(sizeof(timer_callback_data_t));
    func = JS_DupValue(ctx, argv[0]);
    data->ctx = ctx;
    data->func = func;

    // 创建 libuv 定时器句柄，并设置回调函数和数据
    uv_loop_t *loop = (uv_loop_t *)JS_GetContextOpaque(ctx);
    uv_timer_t *timer = (uv_timer_t *)malloc(sizeof(uv_timer_t));
    uv_timer_init(loop, timer);
    timer->data = data;

    // 启动定时器
    uv_timer_start(timer, setTimeout_callback, delay, 0);

    // JS_FreeValue(ctx, func);

    return JS_NewInt64(ctx, (int64_t)timer);
}