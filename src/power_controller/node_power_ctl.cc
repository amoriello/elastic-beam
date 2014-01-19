#include <node.h>
#include "power_controller/node_power_ctl.h"

using namespace v8;

Persistent<Function> NodePowerCtl::constructor;

NodePowerCtl::NodePowerCtl()
  : power_strip_(products::kSisPmFlashNew),
    power_ctl_(power_strip_) {
  power_ctl_.Init();
}


NodePowerCtl::~NodePowerCtl() {
}


bool NodePowerCtl::InputIsValid(const Arguments& args,
                                       std::string* p_err_msg) const {
  if (args.Length() != 1) {
    *p_err_msg = "Wrong number of arguments";
    return false;
  }

  if (args[0]->IsUndefined()) {
    *p_err_msg = "Argument is undefined";
    return false;
  }

  if (!args[0]->IsNumber()) {
    *p_err_msg = "Wrong argument type";
    return false;
  }

  if (!power_strip_.IsValidOutlet(args[0]->NumberValue())) {
    *p_err_msg = "Invalid input range";
    return false;
  }

  return true;
}


void NodePowerCtl::Init(Handle<Object> exports) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("PowerCtl"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->PrototypeTemplate()->Set(String::NewSymbol("turnOn"),
      FunctionTemplate::New(TurnOn)->GetFunction());

  tpl->PrototypeTemplate()->Set(String::NewSymbol("turnOff"),
      FunctionTemplate::New(TurnOff)->GetFunction());

  tpl->PrototypeTemplate()->Set(String::NewSymbol("toggle"),
      FunctionTemplate::New(Toggle)->GetFunction());

  tpl->PrototypeTemplate()->Set(String::NewSymbol("isOn"),
      FunctionTemplate::New(IsOn)->GetFunction());

  tpl->PrototypeTemplate()->Set(String::NewSymbol("isOff"),
      FunctionTemplate::New(IsOff)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());

  exports->Set(String::NewSymbol("PowerCtl"), constructor);
}


Handle<Value> NodePowerCtl::New(const Arguments& args) {
  HandleScope scope;

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new PowerCtl(...)`
    NodePowerCtl* obj = new NodePowerCtl();
    obj->Wrap(args.This());
    return args.This();
  } else {
    // Invoked as plain function `NodePowerCtl()`, turn into construct call.
    return scope.Close(constructor->NewInstance());
  }
}


template <typename PwrAction>
Handle<Value> NodePowerCtl::ForwardCall(const Arguments& args,
                                               PwrAction action) {
  auto p_pwr_ctl = ObjectWrap::Unwrap<NodePowerCtl>(args.This());

  std::string err_msg;
  if (!p_pwr_ctl->InputIsValid(args, &err_msg)) {
    ThrowInputError(err_msg);
    return Undefined();
  }

  int value = args[0]->NumberValue();
  int result = action(&(p_pwr_ctl->power_ctl_), value);

  return Number::New(result);
}



Handle<Value> NodePowerCtl::TurnOn(const Arguments& args) {
  auto action = [](PowerCtl* p_pwr_ctl, int outlet) {
    p_pwr_ctl->TurnOn(outlet);
    return 0;
  };

  return NodePowerCtl::ForwardCall(args, action);
}


Handle<Value> NodePowerCtl::TurnOff(const Arguments& args) {
  auto action = [](PowerCtl* p_pwr_ctl, int outlet) {
    p_pwr_ctl->TurnOff(outlet);
    return 0;
  };

  return NodePowerCtl::ForwardCall(args, action);
}


Handle<Value> NodePowerCtl::Toggle(const Arguments& args) {
  auto action = [](PowerCtl* p_pwr_ctl, int outlet) {
    p_pwr_ctl->Toggle(outlet);
    return 0;
  };

  return NodePowerCtl::ForwardCall(args, action);
}


Handle<Value> NodePowerCtl::IsOn(const Arguments& args) {
  auto action = [](PowerCtl* p_pwr_ctl, int outlet) {
    return p_pwr_ctl->IsOn(outlet);
  };

  return NodePowerCtl::ForwardCall(args, action);
}


Handle<Value> NodePowerCtl::IsOff(const Arguments& args) {
  auto action = [](PowerCtl* p_pwr_ctl, int outlet) {
    return p_pwr_ctl->IsOff(outlet);
  };

  return NodePowerCtl::ForwardCall(args, action);
}

