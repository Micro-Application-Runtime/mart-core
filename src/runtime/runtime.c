#include "runtime/runtime.h"

#include <memory.h>

#include "common/log/log.h"
#include "config/common.h"
#include "runtime/modules/std/std.h"
#include "runtime/modules/process/process.h"
#include "runtime/modules/bridge/bridge.h"
#include "runtime/modules/demo/demo.h"
#include "runtime/utils/quickjs_utils.h"

int runtime_init(runtime_t *rt)
{
    LOG_I(RUNTIME_LOG_TAG, "Init ...");

    if (rt == NULL)
    {
        LOG_F(RUNTIME_LOG_TAG, "Init error: rt is NULL");
        return -1;
    }

    rt->exit_func = NULL;

    // 初始化QuickJS运行时和上下文
    rt->qjs_rt = JS_NewRuntime();
    rt->qjs_ctx = JS_NewContext(rt->qjs_rt);
    rt->uv_loop = malloc(sizeof(uv_loop_t));
    rt->log_buf = malloc(JS_LOG_BUF_SIZE);
    rt->log_buf_size = JS_LOG_BUF_SIZE;

    // 初始化libuv事件循环
    uv_loop_init(rt->uv_loop);

    // 设置runtime_t作为 QuickJS 上下文的透明数据
    JS_SetContextOpaque(rt->qjs_ctx, rt);

    // 注册模块到 QuickJS
    add_std(rt->qjs_ctx);
    add_process(rt->qjs_ctx);
    add_bridge(rt->qjs_ctx);
    add_demo(rt->qjs_ctx);

#if DISABLE_EVAL
    {
        LOG_V(RUNTIME_LOG_TAG, "Disable eval func in JS runtime");
        // 禁止在JS中使用 eval 函数
        JSValue global = JS_GetGlobalObject(rt->qjs_ctx);
        JS_SetPropertyStr(rt->qjs_ctx, global, "eval", JS_UNDEFINED);
        // 释放 global 对象的引用
        JS_FreeValue(rt->qjs_ctx, global);
    }
#endif
    LOG_I(RUNTIME_LOG_TAG, "Init finished.");
    return 0;
}

int runtime_set_exit_func(runtime_t *rt, exit_func_t exit_func)
{
    if (rt == NULL)
    {
        return -1;
    }

    rt->exit_func = exit_func;
}

int runtime_load_js_file(runtime_t *rt, const char *file_path)
{
    if (rt == NULL)
    {
        LOG_E(RUNTIME_LOG_TAG, "Load js error: rt is NULL");
        return -1;
    }

    return quickjs_load_js_file(rt->qjs_ctx, file_path);
}

int runtime_call_in_loop(runtime_t *rt, JSJobFunc func, int argc, JSValueConst *argv)
{
    if (rt == NULL)
    {
        return -1;
    }

    if (func == NULL)
    {
        return -1;
    }

    JS_EnqueueJob(rt->qjs_ctx, func, argc, argv);

    return 0;
}

int runtime_send_to_bridge(runtime_t *rt, int argc, JSValueConst *argv)
{
    if (rt == NULL)
    {
        return -1;
    }

    send_to_bridge(rt->qjs_ctx, argc, argv);
}

int runtime_run_loop(runtime_t *rt)
{
    if (rt == NULL)
    {
        return -1;
    }

    // 运行 libuv 事件循环，直到没有待处理的回调函数
    uv_run(rt->uv_loop, UV_RUN_NOWAIT);

    // 运行 quickjs 事件循环
    JSContext *ctx1;
    int err = JS_ExecutePendingJob(rt->qjs_rt, &ctx1);

    if (err <= 0)
    {
        if (err < 0)
        {
            // show exception
            JSValue exception = JS_GetException(rt->qjs_ctx);

            if (!JS_IsNull(exception))
            {
                const char *exception_str = JS_ToCString(rt->qjs_ctx, exception);
                fprintf(stderr, "JavaScript Error: %s\n", exception_str);
                JS_FreeCString(rt->qjs_ctx, exception_str);
            }

            JS_FreeValue(rt->qjs_ctx, exception);
        }
    }

    return 0;
}

int runtime_destory(runtime_t *rt)
{
    LOG_I(RUNTIME_LOG_TAG, "Destory ...");

    if (rt == NULL)
    {
        return -1;
    }

    // TODO: libuv中的数据应该被释放
    uv_stop(rt->uv_loop);
    uv_loop_close(rt->uv_loop);
    free(rt->uv_loop);
    free(rt->log_buf);
    JS_FreeContext(rt->qjs_ctx);
    JS_FreeRuntime(rt->qjs_rt);

    LOG_I(RUNTIME_LOG_TAG, "Destory finish.");

    return 0;
}
