#include "runtime/modules/std/timer.h"

#include <stdlib.h>
#include <uv.h>

#include "runtime/runtime.h"
#include "runtime/utils/runtime_utils.h"

typedef struct
{
    JSContext *ctx;
    JSValue func;
} timer_data_t;

static timer_data_t *getTimerData(uv_handle_t *handle)
{
    return (timer_data_t *)handle->data;
}

static void timerCloseCallback(uv_handle_t *handle)
{
    timer_data_t *data = getTimerData(handle);
    JSContext *ctx = data->ctx;
    JS_FreeValue(ctx, data->func);
    free(data);
    handle->data = NULL;
}

static void timerWalkCallback(uv_handle_t *handle, void *arg)
{
    if (handle->type == UV_TIMER && handle->data)
    {
        timerCloseCallback(handle);
    }
}

static JSValue jsjob_TimerCallback(JSContext *ctx, int argc, JSValueConst *argv)
{
    // 调用QuickJS的回调函数
    JSValue func = argv[0];
    JS_Call(ctx, func, JS_UNDEFINED, 0, NULL);
}

static void setTimeoutCallback(uv_timer_t *handle)
{
    timer_data_t *data = handle->data;
    JSContext *ctx = data->ctx;
    JSValue func = data->func;

    JSValue args[] = {func};

    JS_EnqueueJob(ctx, jsjob_TimerCallback, 1, args);

    // 关闭和释放定时器句柄
    uv_close((uv_handle_t *)handle, &timerCloseCallback);
}

static void setIntervalCallback(uv_timer_t *handle)
{
    timer_data_t *data = handle->data;
    JSContext *ctx = data->ctx;
    JSValue func = data->func;

    JSValue args[] = {func};

    JS_EnqueueJob(ctx, jsjob_TimerCallback, 1, args);
}

// JavaScript Function Begin
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

    timer_data_t *data = malloc(sizeof(timer_data_t));
    JSValue func = JS_DupValue(ctx, argv[0]);
    data->ctx = ctx;
    data->func = func;

    // 创建 libuv 定时器句柄，并设置回调函数和数据
    runtime_t *rt = GetRuntime(ctx);
    uv_timer_t *timer = (uv_timer_t *)malloc(sizeof(uv_timer_t));
    uv_timer_init(rt->uv_loop, timer);
    timer->data = data;

    // 启动定时器
    uv_timer_start(timer, setTimeoutCallback, delay, 0);
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

    uv_timer_stop(timer);
    uv_close((uv_handle_t *)timer, &timerCloseCallback);

    return JS_UNDEFINED;
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

    timer_data_t *data = malloc(sizeof(timer_data_t));
    JSValue func = JS_DupValue(ctx, argv[0]);
    data->ctx = ctx;
    data->func = func;

    // 创建 libuv 定时器句柄，并设置回调函数和数据
    runtime_t *rt = GetRuntime(ctx);
    uv_timer_t *timer = (uv_timer_t *)malloc(sizeof(uv_timer_t));
    uv_timer_init(rt->uv_loop, timer);
    timer->data = data;

    // 启动定时器
    uv_timer_start(timer, setIntervalCallback, delay, delay);
    JSValue ret = JS_NewInt64(ctx, (int64_t)timer);
    return ret;
}

JSValue js_clearInterval(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
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

    uv_timer_stop(timer);
    uv_close((uv_handle_t *)timer, &timerCloseCallback);

    return JS_UNDEFINED;
}

// JavaScript Function End

void close_all_timer(JSContext *ctx)
{
    runtime_t *rt = GetRuntime(ctx);
    // 查询所有定时器并释放资源
    uv_walk(rt->uv_loop, &timerWalkCallback, ctx);
}
