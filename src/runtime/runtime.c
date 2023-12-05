#include "runtime/runtime.h"

#include <memory.h>

#include "config/common.h"
#include "runtime/modules/std/std.h"
#include "runtime/modules/process/process.h"
#include "runtime/modules/bridge/bridge.h"

int runtime_init(runtime_t *rt)
{
    if (rt == NULL)
    {
        return -1;
    }

    rt->exit_func = NULL;

    // 初始化QuickJS运行时和上下文
    rt->qjs_rt = JS_NewRuntime();
    rt->qjs_ctx = JS_NewContext(rt->qjs_rt);
    rt->uv_loop = malloc(sizeof(uv_loop_t));

    // 初始化libuv事件循环
    uv_loop_init(rt->uv_loop);

    // 设置runtime_t作为 QuickJS 上下文的透明数据
    JS_SetContextOpaque(rt->qjs_ctx, rt);

    // 注册模块到 QuickJS
    add_std(rt->qjs_ctx);
    add_process(rt->qjs_ctx);
    add_bridge(rt->qjs_ctx);

#if DISABLE_EVAL
    {
        // 禁止在JS中使用 eval 函数
        JSValue global = JS_GetGlobalObject(rt->qjs_ctx);
        JS_SetPropertyStr(rt->qjs_ctx, global, "eval", JS_UNDEFINED);
        // 释放 global 对象的引用
        JS_FreeValue(rt->qjs_ctx, global);
    }
#endif
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
        return -1;
    }

    FILE *file = fopen(file_path, "r");

    if (!file)
    {
        fprintf(stderr, "Failed to open file: %s\n", file_path);
        return -1;
    }

    char *buf = malloc(JS_FILE_BUF_SIZE);

    if (!buf)
    {
        fprintf(stderr, "malloc memory failed!\n");
        return -1;
    }

    size_t bytes_read = fread(buf, 1, JS_FILE_BUF_SIZE, file);
    fclose(file);

    if (bytes_read > 0)
    {
        JSValue ret = JS_Eval(rt->qjs_ctx, buf, bytes_read, file_path, JS_EVAL_TYPE_MODULE);
        JSValue exception = JS_GetException(rt->qjs_ctx);
        if (!JS_IsNull(exception))
        {
            const char *exception_str = JS_ToCString(rt->qjs_ctx, exception);
            fprintf(stderr, "Error executing JavaScript: %s\n", exception_str);
            JS_FreeCString(rt->qjs_ctx, exception_str);
        }
        JS_FreeValue(rt->qjs_ctx, exception);
        JS_FreeValue(rt->qjs_ctx, ret);
    }

    free(buf);
    buf = NULL;

    return 0;
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
    if (rt == NULL)
    {
        return -1;
    }

    // TODO: libuv中的数据应该被释放
    uv_stop(rt->uv_loop);
    uv_loop_close(rt->uv_loop);
    free(rt->uv_loop);
    JS_FreeContext(rt->qjs_ctx);
    JS_FreeRuntime(rt->qjs_rt);

    return 0;
}
