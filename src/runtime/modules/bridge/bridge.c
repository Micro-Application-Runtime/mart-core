#include "runtime/modules/bridge/bridge.h"

JSValue jsjob_send_to_bridge(JSContext *ctx, int argc, JSValueConst *argv)
{
    JSValue global = JS_GetGlobalObject(ctx);
    JSValue bridge = JS_GetPropertyStr(ctx, global, "bridge");

    if (!JS_IsObject(bridge))
    {
        return JS_UNDEFINED;
    }

    JSValue on_msg = JS_GetPropertyStr(ctx, bridge, "on_msg");

    if (JS_IsFunction(ctx, on_msg))
    {
        JS_Call(ctx, on_msg, bridge, argc, argv);
    }

    JS_FreeValue(ctx, on_msg);
    JS_FreeValue(ctx, bridge);
    JS_FreeValue(ctx, global);

    return JS_UNDEFINED;
}

JSValue js_on_msg(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    return JS_UNDEFINED;
}

void add_bridge(JSContext *ctx)
{
    JSValue global = JS_GetGlobalObject(ctx);

    // 注册 console.log 到 global 对象中
    JSValue bridge = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, bridge, "on_msg", JS_NewCFunction(ctx, js_on_msg, "on_msg", 1));
    JS_SetPropertyStr(ctx, global, "bridge", bridge);

    // 释放 global 对象的引用
    JS_FreeValue(ctx, global);
}

void send_to_bridge(JSContext *ctx, int argc, JSValueConst *argv)
{
    JS_EnqueueJob(ctx, jsjob_send_to_bridge, argc, argv);
}
