#ifndef __MODULES__BRIDGE__BRIDGE_H__
#define __MODULES__BRIDGE__BRIDGE_H__

#include <quickjs.h>

void add_bridge(JSContext *ctx);

void send_to_bridge(JSContext *ctx, int argc, JSValueConst *argv);

#endif // __MODULES__BRIDGE__BRIDGE_H__