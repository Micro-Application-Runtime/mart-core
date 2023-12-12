#include "runtime/modules/std/std.h"

#include <uv.h>

#include "runtime/modules/std/console.h"
#include "runtime/modules/std/timer.h"
#include "runtime/runtime.h"
#include "runtime/utils/runtime_utils.h"

void add_std(JSContext *ctx)
{
    // 注册 console 到 global 对象中
    js_add_console(ctx);

    JSValue global = JS_GetGlobalObject(ctx);
    // 注册 setTimeout 到 global 对象中
    JS_SetPropertyStr(ctx, global, "setTimeout", JS_NewCFunction(ctx, js_setTimeout, "setTimeout", 2));

    // 注册 clearTimeout 到 global 对象中
    JS_SetPropertyStr(ctx, global, "clearTimeout", JS_NewCFunction(ctx, js_clearTimeout, "clearTimeout", 1));

    // 注册 setInterval 到 global 对象中
    JS_SetPropertyStr(ctx, global, "setInterval", JS_NewCFunction(ctx, js_setInterval, "setInterval", 2));

    // 注册 clearTimeout 到 global 对象中
    JS_SetPropertyStr(ctx, global, "clearInterval", JS_NewCFunction(ctx, js_clearInterval, "clearInterval", 1));

    // 释放 global 对象的引用
    JS_FreeValue(ctx, global);
}

void destory_std(JSContext *ctx)
{
    close_all_timer(ctx);
}
