// Copyright (c) 2006-2008 The Elastic-Beam Authors. Beerware License.


#ifndef SRC_SCOPE_EXIT_H_
#define SRC_SCOPE_EXIT_H_

#include <functional>

class OnScopeExit {
  typedef std::function<void()> Handler;

public:
  explicit OnScopeExit(Handler&& handler)
    : handler_(handler) { }

  ~OnScopeExit() {
    handler_();
  }

private:
  Handler handler_;
};

#endif  // SRC_SCOPE_EXIT_H_
