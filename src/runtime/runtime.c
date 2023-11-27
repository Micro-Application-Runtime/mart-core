#include "runtime.h"

int runtime_init(runtime_t *rt)
{
    if (rt == NULL)
    {
        return -1;
    }

    // 初始化QuickJS运行时和上下文
    rt->qjs_rt = JS_NewRuntime();
    rt->qjs_ctx = JS_NewContext(rt->qjs_rt);
    rt->uv_loop = malloc(sizeof(uv_loop_t));

    // 初始化libuv事件循环
    uv_loop_init(rt->uv_loop);

    // 设置runtime_t作为QuickJS上下文的透明数据
    JS_SetContextOpaque(rt->qjs_ctx, rt);

    return 0;
}

int runtime_run_loop(runtime_t *rt)
{
    uv_run(rt->uv_loop, UV_RUN_ONCE);
    JSContext *ctx1;
    int err = JS_ExecutePendingJob(JS_GetRuntime(rt->qjs_ctx), &ctx1);
    if (err <= 0)
    {
        if (err < 0)
        {
            // show exception
        }
    }
    return 0;
}

int runtime_destory(runtime_t *rt)
{
    uv_loop_close(rt->uv_loop);
    free(rt->uv_loop);
    JS_FreeContext(rt->qjs_ctx);
    JS_FreeRuntime(rt->qjs_rt);
    return 0;
}
