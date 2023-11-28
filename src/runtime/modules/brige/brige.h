#ifndef __MODULES_BRIGE_BRIGE_H__
#define __MODULES_BRIGE_BRIGE_H__

#include <quickjs.h>

void add_brige(JSContext *ctx);

void send_to_brige(JSContext *ctx, int argc, JSValueConst *argv);

#endif // __MODULES_BRIGE_BRIGE_H__