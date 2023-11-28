#include "runtime/modules/process/process.h"

#include "runtime/modules/process/exit.h"

void add_process(JSContext *ctx)
{
    JSValue global = JS_GetGlobalObject(ctx);

    JSValue process = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, process, "exit", JS_NewCFunction(ctx, js_process_exit, "exit", 1));
    JS_SetPropertyStr(ctx, global, "process", process);

    // 释放 global 对象的引用
    JS_FreeValue(ctx, global);
}
