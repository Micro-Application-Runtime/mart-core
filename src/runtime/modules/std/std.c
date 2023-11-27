#include "std.h"

#include "settimeout.h"
#include "console_log.h"

void runtime_add_std(runtime_t *rt)
{
    JSContext *qjs_ctx = rt->qjs_ctx;
    JSValue global = JS_GetGlobalObject(qjs_ctx);
    JSValue console = JS_NewObject(qjs_ctx);

    // 注册log函数到console对象中
    JS_SetPropertyStr(qjs_ctx, console, "log", JS_NewCFunction(qjs_ctx, js_console_log, "log", 1));
    // 注册console对象到global中
    JS_SetPropertyStr(qjs_ctx, global, "console", console);
    JS_FreeValue(qjs_ctx, console);

    // 注册setTimeout函数到global中
    JS_SetPropertyStr(qjs_ctx, global, "setTimeout", JS_NewCFunction(qjs_ctx, js_setTimeout, "setTimeout", 1));

    JS_FreeValue(qjs_ctx, global);
}
