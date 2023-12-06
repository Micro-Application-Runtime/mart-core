#include <stdio.h>
#include <memory.h>

#include <quickjs.h>
#include <quickjs-libc.h>
#include <uv.h>

#include "runtime/runtime.h"
#include "common/log/log.h"

static int exit_code = 0;
static int is_exit = 0;

void exit_func(int exit_code_)
{
    exit_code = exit_code_;
    is_exit = 1;
}

int main(int argc, char **argv)
{
    LOG_I("main", "start");

    // 临时方案从参数读取js文件路径
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *file_path = argv[1];

    runtime_t rt;
    runtime_init(&rt);
    runtime_set_exit_func(&rt, exit_func);
    
    runtime_load_js_file(&rt, file_path);

    JSValue msg = JS_NewString(rt.qjs_ctx, "msg is received");
    JSValue args[] = {msg};
    runtime_send_to_bridge(&rt, 1, args);
    JS_FreeValue(rt.qjs_ctx, msg);

    while (!is_exit)
    {
        // TODO
        // read msg from ipc
        runtime_run_loop(&rt);
    }

    runtime_destory(&rt);

    return exit_code;
}
