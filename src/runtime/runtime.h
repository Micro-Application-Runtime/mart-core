#ifndef __RUNTIME_H__
#define __RUNTIME_H__

#include <quickjs.h>
#include <quickjs-libc.h>
#include <uv.h>

typedef void (*exit_func_t)(int);

typedef struct
{
    JSRuntime *qjs_rt;
    JSContext *qjs_ctx;
    uv_loop_t *uv_loop;
    exit_func_t exit_func;
} runtime_t;

int runtime_init(runtime_t *rt);

int runtime_set_exit_func(runtime_t *rt, exit_func_t exit_func);

int runtime_load_js_file(runtime_t *rt, const char *file_path);

int runtime_call_in_loop(runtime_t *rt, JSJobFunc func, int argc, JSValueConst *argv);

int runtime_send_to_brige(runtime_t *rt, int argc, JSValueConst *argv);

int runtime_run_loop(runtime_t *rt);

int runtime_destory(runtime_t *rt);

#endif // __RUNTIME_H__