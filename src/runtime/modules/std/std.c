#include "runtime/modules/std/std.h"

#include "runtime/modules/std/console.h"
#include "runtime/modules/std/settimeout.h"

void runtime_add_std(runtime_t *rt)
{
    JSContext *qjs_ctx = rt->qjs_ctx;
    JSValue global = JS_GetGlobalObject(qjs_ctx);

    // 注册 console.log 到 global 对象中
    JSValue console = JS_NewObject(qjs_ctx);
    JS_SetPropertyStr(qjs_ctx, console, "log", JS_NewCFunction(qjs_ctx, js_console_log, "log", 1));
    JS_SetPropertyStr(qjs_ctx, global, "console", console);

    // 注册 setTimeout 到 global 对象中
    JS_SetPropertyStr(qjs_ctx, global, "setTimeout", JS_NewCFunction(qjs_ctx, js_setTimeout, "setTimeout", 2));

    // 注册 clearTimeout 到 global 对象中
    JS_SetPropertyStr(qjs_ctx, global, "clearTimeout", JS_NewCFunction(qjs_ctx, js_clearTimeout, "clearTimeout", 1));

    // 释放 global 对象的引用
    JS_FreeValue(qjs_ctx, global);
}
