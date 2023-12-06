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
        JSValue exception = JS_GetException(ctx);
        if (!JS_IsNull(exception))
        {
            const char *exception_str = JS_ToCString(ctx, exception);
            LOG_E(JS_LOG_TAG, "Exception: %s", exception_str);
            JS_FreeCString(ctx, exception_str);
        }
        JS_FreeValue(ctx, exception);
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
