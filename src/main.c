#include <stdio.h>
#include <memory.h>

#include <quickjs.h>
#include <quickjs-libc.h>
#include <uv.h>

#include "runtime/runtime.h"
// #include "common/log.h"

static int exit_code = 0;
static int is_exit = 0;

void exit_func(int exit_code_)
{
    exit_code = exit_code_;
    is_exit = 1;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *file_path = argv[1];

    runtime_t rt;
    runtime_init(&rt);
    rt.exit_func = exit_func;
    runtime_load_js_file(&rt, file_path);

    while (!is_exit)
    {
        // TODO
        runtime_run_loop(&rt);
    }

    runtime_destory(&rt);

    return exit_code;
}
