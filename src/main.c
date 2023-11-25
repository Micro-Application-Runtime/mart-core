#include <stdio.h>
#include <memory.h>

#include <quickjs.h>
#include <quickjs-libc.h>
#include <uv.h>

#include "modules/global/set_timeout.h"

#define BUF_SIZE 10 * 1024 * 1024

int main(int argc, char **argv)
{
    JSRuntime *rt = NULL;
    JSContext *ctx = NULL;
    uv_loop_t loop;

    const char *filename = NULL;
    FILE *file = NULL;
    char *buf = NULL;
    size_t bytes_read = 0;
    int eval_flags = 0;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    filename = argv[1];

    // 初始化QuickJS运行时和上下文
    rt = JS_NewRuntime();
    ctx = JS_NewContext(rt);

    js_std_add_helpers(ctx, 0, NULL);

    // 初始化libuv事件循环
    uv_loop_init(&loop);
    // 设置libuv事件循环作为QuickJS上下文的透明数据
    JS_SetContextOpaque(ctx, &loop);

    // 注册setTimeout函数到QuickJS中
    JSValue global_obj = JS_GetGlobalObject(ctx);
    JS_SetPropertyStr(ctx, global_obj, "setTimeout", JS_NewCFunction(ctx, js_setTimeout, "setTimeout", 1));
    // JS_FreeValue(ctx, global_obj);

    /* Load and execute the JavaScript file */
    file = fopen(filename, "r");

    if (!file)
    {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        goto cleanup;
    }

    eval_flags = JS_EVAL_TYPE_MODULE; // Use JS_EVAL_TYPE_GLOBAL for non-module scripts

    buf = malloc(BUF_SIZE);

    if (!buf)
    {
        fprintf(stderr, "malloc memory failed!\n");
        goto cleanup;
    }

    while ((bytes_read = fread(buf, 1, BUF_SIZE, file)) > 0)
    {
        JSValue ret = JS_Eval(ctx, buf, bytes_read, filename, eval_flags);
        JSValue exception = JS_GetException(ctx);
        if (!JS_IsNull(exception))
        {
            const char *exception_str = JS_ToCString(ctx, exception);
            fprintf(stderr, "Error1 executing JavaScript: %s\n", exception_str);
            JS_FreeCString(ctx, exception_str);
            JS_FreeValue(ctx, exception);
        }
        JS_FreeValue(ctx, exception);
        JS_FreeValue(ctx, ret);
    }

    // 加载和执行JavaScript代码

    uv_run(&loop, UV_RUN_DEFAULT);
    js_std_loop(ctx);
    uv_loop_close(&loop);

    JSValue exception = JS_GetException(ctx);
    if (!JS_IsNull(exception))
    {
        const char *exception_str = JS_ToCString(ctx, exception);
        fprintf(stderr, "Error3 executing JavaScript: %s\n", exception_str);
        JS_FreeCString(ctx, exception_str);
        JS_FreeValue(ctx, exception);
    }

    free(buf);
    buf = NULL;

cleanup:
    fclose(file);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    return 0;
}
