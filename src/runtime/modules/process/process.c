#include "runtime/modules/process/process.h"

#include "runtime/modules/process/exit.h"

void runtime_add_process(runtime_t *rt)
{
    JSContext *qjs_ctx = rt->qjs_ctx;
    JSValue global = JS_GetGlobalObject(qjs_ctx);

    JSValue process = JS_NewObject(qjs_ctx);
    JS_SetPropertyStr(qjs_ctx, process, "exit", JS_NewCFunction(qjs_ctx, js_process_exit, "exit", 1));
    JS_SetPropertyStr(qjs_ctx, global, "process", process);

    // 释放 global 对象的引用
    JS_FreeValue(qjs_ctx, global);
}
