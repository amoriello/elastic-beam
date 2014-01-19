#include <node.h>
#include "node_power_ctl.h"

void InitAll(v8::Handle<v8::Object> exports) {
  NodePowerCtl::Init(exports);
}

NODE_MODULE(power_ctl, InitAll)
