#include "jsutils.h"

runtime_t *GetRuntime(JSContext *ctx)
{
    return (runtime_t *)JS_GetContextOpaque(ctx);
}
