#include "exit.h"

#include "runtime/runtime.h"
#include "runtime/utils/runtime_utils.h"

JSValue js_process_exit(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    int exit_code = 1;

    if (argc >= 1)
    {
        JSValue arg0 = argv[0];

        if (JS_IsNumber(arg0))
        {
            JS_ToInt32(ctx, &exit_code, arg0);
        }

        JS_FreeValue(ctx, arg0);
    }

    runtime_t *rt = GetRuntime(ctx);
    exit_func_t exit_func = rt->exit_func;

    if (exit_func != NULL)
    {
        (*exit_func)(exit_code);
    }

    return JS_UNDEFINED;
}