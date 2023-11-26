#ifndef __RUNTIME_H__
#define __RUNTIME_H__

#include <quickjs.h>
#include <quickjs-libc.h>
#include <uv.h>

typedef struct
{
    JSRuntime *qjs_rt;
    JSContext *qjs_ctx;
    uv_loop_t *uv_loop;
} runtime_t;

int runtime_init(runtime_t *rt);

int runtime_run_loop(runtime_t *rt);

int runtime_destory(runtime_t *rt);

#endif // __RUNTIME_H__