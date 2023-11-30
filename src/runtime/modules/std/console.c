#include "console.h"
#include "common/utils.h"

JSValue js_console_log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    int i;
    const char *str;
    size_t len;

    for (i = 0; i < argc; i++)
    {
        if (i != 0)
            putchar(' ');
        str = JS_ToCStringLen(ctx, &len, argv[i]);
        if (!str)
            return JS_EXCEPTION;
        fwrite(str, 1, len, stdout);
        JS_FreeCString(ctx, str);
    }
    putchar('\n');
    return JS_UNDEFINED; 
}

static const JSCFunctionListEntry js_console_funcs[] = {
    JS_CFUNC_DEF("log", -1, js_console_log),
};

static const JSCFunctionListEntry js_json_obj[] = {
    JS_OBJECT_DEF("console", js_console_funcs, countof(js_console_funcs), JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE),
};

void js_add_console(JSContext* ctx)
{
    JSValue global = JS_GetGlobalObject(ctx);
    JS_SetPropertyFunctionList(ctx, global, js_json_obj, countof(js_json_obj));
    JS_FreeValue(ctx, global);
}
