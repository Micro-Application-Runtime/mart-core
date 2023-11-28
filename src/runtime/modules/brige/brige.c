#include "runtime/modules/brige/brige.h"

JSValue jsjob_send_to_brige(JSContext *ctx, int argc, JSValueConst *argv)
{
    JSValue global = JS_GetGlobalObject(ctx);
    JSValue brige = JS_GetPropertyStr(ctx, global, "brige");

    if (!JS_IsObject(brige))
    {
        return JS_UNDEFINED;
    }

    JSValue on_msg = JS_GetPropertyStr(ctx, brige, "on_msg");

    if (JS_IsFunction(ctx, on_msg))
    {
        JS_Call(ctx, on_msg, brige, argc, argv);
    }

    JS_FreeValue(ctx, on_msg);
    JS_FreeValue(ctx, brige);
    JS_FreeValue(ctx, global);

    return JS_UNDEFINED;
}

JSValue js_on_msg(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    return JS_UNDEFINED;
}

void add_brige(JSContext *ctx)
{
    JSValue global = JS_GetGlobalObject(ctx);

    // 注册 console.log 到 global 对象中
    JSValue brige = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, brige, "on_msg", JS_NewCFunction(ctx, js_on_msg, "on_msg", 1));
    JS_SetPropertyStr(ctx, global, "brige", brige);

    // 释放 global 对象的引用
    JS_FreeValue(ctx, global);
}

void send_to_brige(JSContext *ctx, int argc, JSValueConst *argv)
{
    JS_EnqueueJob(ctx, jsjob_send_to_brige, argc, argv);
}
