// Copyright (c) 2006-2008 The Elastic-Beam Authors. Beerware License".

#ifndef SRC_POWER_CONTROLLER_NODE_POWER_CTL_H_
#define SRC_POWER_CONTROLLER_NODE_POWER_CTL_H_

#include <string>

#include <node.h>  // NOLINT

#include "power_controller/products.h"
#include "power_controller/power_ctl.h"


class NodePowerCtl : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  static v8::Handle<v8::Value> New(const v8::Arguments& args);

 private:
  NodePowerCtl();
  ~NodePowerCtl();

  static v8::Handle<v8::Value> TurnOn(const v8::Arguments& args);
  static v8::Handle<v8::Value> TurnOff(const v8::Arguments& args);
  static v8::Handle<v8::Value> Toggle(const v8::Arguments& args);
  static v8::Handle<v8::Value> IsOn(const v8::Arguments& args);
  static v8::Handle<v8::Value> IsOff(const v8::Arguments& args);

  static void ThrowInputError(const std::string& msg) {
    v8::ThrowException(v8::Exception::TypeError(
                                   v8::String::New(msg.c_str())));
  }

  template <typename PwrAction>
  static v8::Handle<v8::Value> ForwardCall(const v8::Arguments& args,
                                           PwrAction action);

 private:
  bool InputIsValid(const v8::Arguments& args,
                    std::string* p_err_msg) const;

 private:
  static v8::Persistent<v8::Function> constructor;

 private:
  const products::PowerStrip& power_strip_;
  PowerCtl power_ctl_;
};


#endif  // SRC_POWER_CONTROLLER_NODE_POWER_CTL_H_

