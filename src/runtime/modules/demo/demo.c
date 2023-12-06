#include "runtime/modules/std/std.h"

#include "runtime/utils/quickjs_utils.h"

static const char *js_files[] = {
    "demo.js"};

void add_demo(JSContext *ctx)
{
    JSValue global = JS_GetGlobalObject(ctx);

    // 注册 console.log 到 global 对象中
    JSValue demo = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, global, "demo", demo);
    // 释放 global 对象的引用
    JS_FreeValue(ctx, global);

    for (size_t i = 0; i < sizeof(js_files) / sizeof(const char *); i++)
    {
        quickjs_load_js_file(ctx, js_files[i]);
    }
}
