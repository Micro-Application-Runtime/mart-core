#include "runtime/modules/std/std.h"

#include "runtime/modules/std/console.h"
#include "runtime/modules/std/timer.h"

void add_std(JSContext *ctx)
{
    JS_AddIntrinsicDate(ctx);

    JSValue global = JS_GetGlobalObject(ctx);

    // 注册 console.log 到 global 对象中
    JSValue console = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, console, "log", JS_NewCFunction(ctx, js_console_log, "log", 1));
    JS_SetPropertyStr(ctx, global, "console", console);

    // 注册 setTimeout 到 global 对象中
    JS_SetPropertyStr(ctx, global, "setTimeout", JS_NewCFunction(ctx, js_setTimeout, "setTimeout", 2));

    // 注册 clearTimeout 到 global 对象中
    JS_SetPropertyStr(ctx, global, "clearTimeout", JS_NewCFunction(ctx, js_clearTimeout, "clearTimeout", 1));

    // 注册 setInterval 到 global 对象中
    JS_SetPropertyStr(ctx, global, "setInterval", JS_NewCFunction(ctx, js_setInterval, "setInterval", 2));

    // 释放 global 对象的引用
    JS_FreeValue(ctx, global);
}
