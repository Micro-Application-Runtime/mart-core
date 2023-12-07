#include "console.h"

#include <stdlib.h>
#include <memory.h>

#include "config/common.h"
#include "common/utils.h"
#include "common/log/log.h"
#include "runtime/utils/runtime_utils.h"

JSValue js_console_log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)
{
    const char *str = NULL;
    size_t str_len = 0;
    char *buf = GetRuntime(ctx)->log_buf;
    const size_t log_buf_size = GetRuntime(ctx)->log_buf_size;

    int cur = 0;
    size_t n = 0;

    for (int i = 0; i < argc; i++)
    {
        str = JS_ToCStringLen(ctx, &str_len, argv[i]);

        if (!str)
        {
            return JS_EXCEPTION;
        }

        if (str_len >= log_buf_size - cur)
        {
            n = log_buf_size - cur;
        }
        else
        {
            n = str_len;
        }

        memcpy(buf + cur, str, n);

        JS_FreeCString(ctx, str);
        cur += n;
    }

    LOG_I(JS_LOG_TAG, "%.*s", cur, buf);

    return JS_UNDEFINED;
}

static const JSCFunctionListEntry js_console_funcs[] = {
    JS_CFUNC_DEF("log", -1, js_console_log),
};

static const JSCFunctionListEntry js_json_obj[] = {
    JS_OBJECT_DEF("console", js_console_funcs, countof(js_console_funcs), JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE),
};

void js_add_console(JSContext *ctx)
{
    JSValue global = JS_GetGlobalObject(ctx);
    JS_SetPropertyFunctionList(ctx, global, js_json_obj, countof(js_json_obj));
    JS_FreeValue(ctx, global);
}
