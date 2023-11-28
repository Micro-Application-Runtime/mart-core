#include "exit.h"

#include "runtime/runtime.h"
#include "runtime/utils/jsutils.h"

JSValue js_process_exit(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    if (argc < 1)
    {
        return JS_ThrowTypeError(ctx, "Invalid arguments");
    }

    JSValue arg0 = argv[0];

    if (!JS_IsNumber(arg0))
    {
        return JS_ThrowTypeError(ctx, "Invalid arguments");
    }

    runtime_t *rt = GetRuntime(ctx);
    exit_func_t exit_func = rt->exit_func;

    if (exit_func != NULL)
    {
        int exit_code = 0;
        JS_ToInt32(ctx, &exit_code, arg0);
        (*exit_func)(exit_code);
    }

    JS_FreeValue(ctx, arg0);

    return JS_UNDEFINED;
}