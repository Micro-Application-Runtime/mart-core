#include "settimeout.h"

#include <stdlib.h>
#include <uv.h>

#include "runtime/runtime.h"
#include "runtime/utils/jsutils.h"

JSValue jsjob_call_callback(JSContext *ctx, int argc, JSValueConst *argv)
{
    // 调用QuickJS的回调函数
    JSValue func = argv[0];
    JS_Call(ctx, func, JS_UNDEFINED, 0, NULL);
}

void setTimeout_callback(uv_timer_t *handle)
{
    timer_callback_data_t *data = (timer_callback_data_t *)handle->data;
    JSContext *ctx = data->ctx;
    JSValue func = data->func;
    free(data);

    JSValue args[] = {func};

    JS_EnqueueJob(ctx, jsjob_call_callback, 1, args);

    // 释放资源
    JS_FreeValue(ctx, func);

    // 关闭和释放定时器句柄
    uv_close((uv_handle_t *)handle, NULL);
}

JSValue js_setTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    if (argc < 2)
    {
        return JS_ThrowTypeError(ctx, "Invalid arguments");
    }

    // 获取延迟时间和回调函数
    int64_t delay;
    if (!JS_IsFunction(ctx, argv[0]) || JS_ToInt64(ctx, &delay, argv[1]))
    {
        return JS_ThrowTypeError(ctx, "Invalid arguments");
    }

    timer_callback_data_t *data = malloc(sizeof(timer_callback_data_t));
    JSValue func = JS_DupValue(ctx, argv[0]);
    data->ctx = ctx;
    data->func = func;

    // 创建 libuv 定时器句柄，并设置回调函数和数据
    runtime_t *rt = GetRuntime(ctx);
    uv_timer_t *timer = (uv_timer_t *)malloc(sizeof(uv_timer_t));
    uv_timer_init(rt->uv_loop, timer);
    timer->data = data;

    // 启动定时器
    uv_timer_start(timer, setTimeout_callback, delay, 0);
    JSValue ret = JS_NewInt64(ctx, (int64_t)timer);
    return ret;
}

JSValue js_clearTimeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    if (argc < 1)
    {
        return JS_ThrowTypeError(ctx, "Invalid arguments");
    }

    if (!JS_IsNumber(argv[0]))
    {
        return JS_ThrowTypeError(ctx, "Invalid arguments");
    }

    int64_t timer_id = 0;
    JS_ToInt64(ctx, &timer_id, argv[0]);
    uv_timer_t *timer = (uv_timer_t *)timer_id;

    if (timer != NULL)
    {
        timer_callback_data_t *data = timer->data;
        JS_FreeValue(ctx, data->func);
        free(data);
        timer->data = NULL;
        uv_timer_stop(timer);
    }

    return JS_UNDEFINED;
}

void setInterval_callback(uv_timer_t *handle)
{
    timer_callback_data_t *data = (timer_callback_data_t *)handle->data;
    JSContext *ctx = data->ctx;
    JSValue func = data->func;

    JSValue args[] = {func};

    JS_EnqueueJob(ctx, jsjob_call_callback, 1, args);
}

JSValue js_setInterval(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    // TODO: 目前当定时器没有关闭而退出程序的时候会出现js内存泄漏，

    if (argc < 2)
    {
        return JS_ThrowTypeError(ctx, "Invalid arguments");
    }

    // 获取延迟时间和回调函数
    int64_t delay;
    if (!JS_IsFunction(ctx, argv[0]) || JS_ToInt64(ctx, &delay, argv[1]))
    {
        return JS_ThrowTypeError(ctx, "Invalid arguments");
    }

    timer_callback_data_t *data = malloc(sizeof(timer_callback_data_t));
    JSValue func = JS_DupValue(ctx, argv[0]);
    data->ctx = ctx;
    data->func = func;

    // 创建 libuv 定时器句柄，并设置回调函数和数据
    runtime_t *rt = GetRuntime(ctx);
    uv_timer_t *timer = (uv_timer_t *)malloc(sizeof(uv_timer_t));
    uv_timer_init(rt->uv_loop, timer);
    timer->data = data;

    // 启动定时器
    uv_timer_start(timer, setInterval_callback, delay, delay);
    JSValue ret = JS_NewInt64(ctx, (int64_t)timer);
    return ret;
}