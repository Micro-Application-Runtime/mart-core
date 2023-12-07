#ifndef __RUNTIME__UTILS__RUNTIME_UTILS_H__
#define __RUNTIME__UTILS__RUNTIME_UTILS_H__

#include <quickjs.h>

#include "runtime/runtime.h"

runtime_t* GetRuntime(JSContext *ctx);

#endif // __RUNTIME__UTILS__RUNTIME_UTILS_H__