
#include <stdio.h>
#include <memory.h>

#include <quickjs.h>
#include <quickjs-libc.h>

// JS_AddIntrinsicBaseObjects(context);
// JS_AddIntrinsicDate(context);
// JS_AddIntrinsicEval(context);
// JS_AddIntrinsicStringNormalize(context);
// JS_AddIntrinsicRegExpCompiler(context);
// JS_AddIntrinsicRegExp(context);
// JS_AddIntrinsicJSON(context);
// JS_AddIntrinsicProxy(context);
// JS_AddIntrinsicMapSet(context);
// JS_AddIntrinsicTypedArrays(context);
// JS_AddIntrinsicPromise(context);
// JS_AddIntrinsicBigInt(context);
// JS_AddIntrinsicBigFloat(context);
// JS_AddIntrinsicBigDecimal(context);
// js_std_init_handlers(runtime);
// js_init_module_std(context, "std");
// js_init_module_os(context, "os");
// js_std_add_helpers(context, 0, NULL);

/* system modules */
// js_std_add_helpers(context, 0, NULL);
// js_init_module_std(context, "std");

// 执行JavaScript文件
// static const char *js_file = "./index.js";

// FILE *fp = fopen(js_file, "r");

// if (fp != NULL)
// {
// printf("read file ...\n");
// char buffer[1024 * 1024];
// memset(buffer, '\0', 1024 * 1024);
// size_t read_size = fread(buffer, 1024 * 1024 - 1, 1, fp);
// printf("read:\n%s\n", buffer);

// }
// else
// {
//     printf("file %s open failed.\n", js_file);
// }

// // 获取 myfunc 函数对象
// const char *function_name = "eventHandler";
// JSValue js_function = JS_GetPropertyStr(context, jsvalue_global, function_name);

// // 检查函数对象是否可调用
// if (JS_IsUndefined(js_function))
// {
//     printf("JSValue '%s' is undefined\n", function_name);
// }
// else if (!JS_IsFunction(context, js_function))
// {
//     printf("JSValue '%s' is not function\n", function_name);
// }
// else
// {
//     // 调用JavaScript函数
//     JSValue ret_val = JS_Call(context, js_function, jsvalue_global, 0, NULL);
// }

JSValue js_event_handler(JSContext *context, int argc, JSValueConst *argv)
{
    size_t len;
    const char *str = JS_ToCStringLen(context, &len, argv[0]);
    printf("%s\n", str);

    JSValue global = JS_GetGlobalObject(context);
    JSValue js_func_test = JS_GetPropertyStr(context, global, "test");
    JS_Call(context, js_func_test, global, 0, NULL);
    JS_FreeCString(context, str);
    JS_FreeValue(context, js_func_test);
    JS_FreeValue(context, global);
}

static int (*os_poll_func)(JSContext *ctx);

int main(int argc, char **argv)
{

    printf("start js engine...\n");

    JSRuntime *runtime = JS_NewRuntime();
    JSContext *context = JS_NewContext(runtime);

    printf("start js engine finished.\n");

    js_std_add_helpers(context, 0, NULL);

    /* system modules */
    js_init_module_std(context, "std");
    js_init_module_os(context, "os");

    // 获取全局对象
    JSValue jsvalue_global = JS_GetGlobalObject(context);

    const char *js_code =
        "import * as os from 'os'\n"
        "console.log('hello world')\n"
        "function test(){console.log('test')}\n"
        "globalThis.test = test\n"
        "os.setTimeout(test, 1000)\n"
        "os.setTimeout(test, 2000)\n"
        "os.setTimeout(test, 3000)\n"
        "os.setInterval(test, 1000)\n";

    JS_Eval(context, js_code, strlen(js_code), "<input>", JS_EVAL_TYPE_MODULE);

    JSValue msg = JS_NewString(context, "event1");
    JSValue args[] = {
        msg};
    JS_EnqueueJob(context, js_event_handler, 1, args);
    JS_FreeValue(context, msg);

    // event loop
    js_std_loop(context);

    JSValue jsvalue_exception = JS_GetException(context);
    if (!JS_IsNull(jsvalue_exception))
    {
        const char *exception_str = JS_ToCString(context, jsvalue_exception);
        printf("[JS Runtime] %s\n", exception_str);
        JS_FreeCString(context, exception_str);
    }
    JS_FreeValue(context, jsvalue_exception);

    // JS_FreeValue(context, js_function);

    JS_FreeValue(context, jsvalue_global);

    printf("exit js engine...\n");

    JS_FreeContext(context);
    JS_FreeRuntime(runtime);

    printf("exit js engine finished.\n");

    return 0;
}