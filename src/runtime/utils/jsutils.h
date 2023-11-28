#ifndef __RUNTIME_UTILS_JSUTILS_H__
#define __RUNTIME_UTILS_JSUTILS_H__

#include <quickjs.h>

#include "runtime/runtime.h"

runtime_t* GetRuntime(JSContext *ctx);

#endif // __RUNTIME_UTILS_JSUTILS_H__