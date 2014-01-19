// Copyright (c) 2006-2008 The Elastic-Beam Authors. Beerware License".

#include <node.h>
#include "power_controller/node_power_ctl.h"

v8::Persistent<v8::Function> NodePowerCtl::constructor;

NodePowerCtl::NodePowerCtl()
  : power_strip_(products::kSisPmFlashNew),
    power_ctl_(power_strip_) {
  power_ctl_.Init();
}


NodePowerCtl::~NodePowerCtl() {
}


bool NodePowerCtl::InputIsValid(const v8::Arguments& args,
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


void NodePowerCtl::Init(v8::Handle<v8::Object> exports) {
  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
  tpl->SetClassName(v8::String::NewSymbol("PowerCtl"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("turnOn"),
      v8::FunctionTemplate::New(TurnOn)->GetFunction());

  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("turnOff"),
      v8::FunctionTemplate::New(TurnOff)->GetFunction());

  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("toggle"),
      v8::FunctionTemplate::New(Toggle)->GetFunction());

  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("isOn"),
      v8::FunctionTemplate::New(IsOn)->GetFunction());

  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("isOff"),
      v8::FunctionTemplate::New(IsOff)->GetFunction());

  constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());

  exports->Set(v8::String::NewSymbol("PowerCtl"), constructor);
}


v8::Handle<v8::Value> NodePowerCtl::New(const v8::Arguments& args) {
  v8::HandleScope scope;

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
v8::Handle<v8::Value> NodePowerCtl::ForwardCall(const v8::Arguments& args,
                                               PwrAction action) {
  auto p_pwr_ctl = ObjectWrap::Unwrap<NodePowerCtl>(args.This());

  std::string err_msg;
  if (!p_pwr_ctl->InputIsValid(args, &err_msg)) {
    ThrowInputError(err_msg);
    return v8::Undefined();
  }

  int value = args[0]->NumberValue();
  int result = action(&(p_pwr_ctl->power_ctl_), value);

  return v8::Number::New(result);
}



v8::Handle<v8::Value> NodePowerCtl::TurnOn(const v8::Arguments& args) {
  auto action = [](PowerCtl* p_pwr_ctl, int outlet) {
    p_pwr_ctl->TurnOn(outlet);
    return 0;
  };

  return NodePowerCtl::ForwardCall(args, action);
}


v8::Handle<v8::Value> NodePowerCtl::TurnOff(const v8::Arguments& args) {
  auto action = [](PowerCtl* p_pwr_ctl, int outlet) {
    p_pwr_ctl->TurnOff(outlet);
    return 0;
  };

  return NodePowerCtl::ForwardCall(args, action);
}


v8::Handle<v8::Value> NodePowerCtl::Toggle(const v8::Arguments& args) {
  auto action = [](PowerCtl* p_pwr_ctl, int outlet) {
    p_pwr_ctl->Toggle(outlet);
    return 0;
  };

  return NodePowerCtl::ForwardCall(args, action);
}


v8::Handle<v8::Value> NodePowerCtl::IsOn(const v8::Arguments& args) {
  auto action = [](PowerCtl* p_pwr_ctl, int outlet) {
    return p_pwr_ctl->IsOn(outlet);
  };

  return NodePowerCtl::ForwardCall(args, action);
}


v8::Handle<v8::Value> NodePowerCtl::IsOff(const v8::Arguments& args) {
  auto action = [](PowerCtl* p_pwr_ctl, int outlet) {
    return p_pwr_ctl->IsOff(outlet);
  };

  return NodePowerCtl::ForwardCall(args, action);
}

