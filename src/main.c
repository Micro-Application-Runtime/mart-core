
#include <stdio.h>

#include <quickjs.h>
#include <quickjs-libc.h>

int main(int argc, char **argv)
{
    printf("start js engine...\n");

    JSRuntime *runtime = JS_NewRuntime();
    JSContext *context = JS_NewContext(runtime);

    printf("start js engine finished.\n");

    // 获取全局对象
    JSValue global_obj = JS_GetGlobalObject(context);

    js_std_add_helpers(context, -1, NULL);

    // 执行JavaScript文件
    const char *js_file = "index.js";

    // eval_file(context, "index.js", JS_EVAL_TYPE_MODULE);

    FILE* fp = fopen(js_file, "r");

    // // JSValue ret = JS_Eval(context, NULL, 0, js_file, JS_EVAL_TYPE_MODULE);

    // 获取 myfunc 函数对象
    const char *function_name = "eventHandler";
    JSValue js_function = JS_GetPropertyStr(context, global_obj, function_name);

    // 检查函数对象是否可调用
    if (JS_IsUndefined(js_function))
    {
        printf("JSValue '%s' is undefined\n", function_name);
    }
    else if (!JS_IsFunction(context, js_function))
    {
        printf("JSValue '%s' is not function\n", function_name);
    }
    else
    {
        // 调用JavaScript函数
        JSValue ret_val = JS_Call(context, js_function, global_obj, 0, NULL);
    }

    JS_FreeValue(context, js_function);

    JS_FreeValue(context, global_obj);

    printf("exit js engine...\n");

    // JS_FreeValue(context, ret);
    JS_FreeContext(context);
    JS_FreeRuntime(runtime);

    printf("exit js engine finished.\n");

    return 0;
}