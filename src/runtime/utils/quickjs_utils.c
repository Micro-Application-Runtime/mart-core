#include "runtime/utils/quickjs_utils.h"

#include <stdlib.h>

#include "config/common.h"
#include "common/log/log.h"

#define RUMTIME_TAG "runtime"
#define JS_LOG_TAG "JavaScript"

int quickjs_load_js_file(JSContext *ctx, const char *file_path)
{
    if (file_path == NULL)
    {
        LOG_E(RUMTIME_TAG, "Load js error: file_path is NULL");
        return -1;
    }

    FILE *file = fopen(file_path, "r");

    if (!file)
    {
        LOG_E(RUMTIME_TAG, "Failed to open file: %s", file_path);
        return -1;
    }

    char *buf = malloc(JS_FILE_BUF_SIZE);

    if (!buf)
    {
        LOG_F(RUMTIME_TAG, "Malloc memory failed!");
        return -1;
    }

    size_t bytes_read = fread(buf, 1, JS_FILE_BUF_SIZE, file);
    fclose(file);
    LOG_I(RUMTIME_TAG, "Load JS file: %s", file_path);

    if (bytes_read > 0)
    {
        JSValue ret = JS_Eval(ctx, buf, bytes_read, file_path, JS_EVAL_TYPE_MODULE);
        quickjs_dump_exception(ctx);
        JS_FreeValue(ctx, ret);
    }
    else
    {
        LOG_W(RUMTIME_TAG, "JS file is empty");
    }

    free(buf);
    buf = NULL;

    return 0;
}

void quickjs_dump_exception(JSContext *ctx)
{
    // show exception
    JSValue exception = JS_GetException(ctx);

    if (JS_IsError(ctx, exception) && !JS_IsNull(exception))
    {
        JSValue lineNumber = JS_GetPropertyStr(ctx, exception, "lineNumber");

        if(JS_IsUndefined(lineNumber)) {
            LOG_E("JavaScript", "JS_IsUndefined");
        }

        if(JS_IsNull(lineNumber)) {
            LOG_E("JavaScript", "JS_IsNull");
        }

        if (JS_IsNumber(lineNumber))
        {
            int num = 0;
            if (JS_ToInt32(ctx, &num, lineNumber))
            {
                LOG_E("JavaScript", "JavaScript lineNumber: %d", num);
            };
        }

        if (JS_IsString(lineNumber))
        {
            const char *lineNumber_str = JS_ToCString(ctx, lineNumber);
            LOG_E("JavaScript", "JavaScript lineNumber: %s", lineNumber_str);
            JS_FreeCString(ctx, lineNumber_str);
        }
        

        const char *exception_str = JS_ToCString(ctx, exception);
        JSValue stack = JS_GetPropertyStr(ctx, exception, "stack");
        const char *stack_str = JS_ToCString(ctx, stack);
        LOG_E("JavaScript",
              "\n\n%s\n%s",
              exception_str,
              stack_str);
        JS_FreeCString(ctx, exception_str);
        JS_FreeCString(ctx, stack_str);
        JS_FreeValue(ctx, stack);
    }

    JS_FreeValue(ctx, exception);
}
