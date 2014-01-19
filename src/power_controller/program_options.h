// Copyright (c) 2006-2008 The Elastic-Beam Authors. Beerware License".

#ifndef SRC_POWER_CONTROLLER_PROGRAM_OPTIONS_H_
#define SRC_POWER_CONTROLLER_PROGRAM_OPTIONS_H_

#include <vector>
#include <cstdint>
#include <iostream>  // NOLINT

class ProgramOptions {
  // What a mess to read uint8_t as byte!!
  //
  // I need to use this instead of outlet_list to allow program_options
  // (that uses stream operator >> for conversion) to parse input
  // as an int, not as a char.
  //
  // Keeping this as private, hidden and implicit as possible
  struct NumByte {
    uint8_t value;
    NumByte() : value() {}
    NumByte(const uint8_t &arg) : value(arg) {}  // NOLINT
    NumByte(const NumByte &arg) : value(arg.value) {}  // NOLINT
    operator uint8_t() const { return value; }

    friend std::istream& operator>>(std::istream& in, NumByte& valArg) {
      int i;
      // Do what it is expected to do
      in >> i;
      // Safe
      valArg.value = static_cast<uint8_t>(i);
      return in;
    }
  };
  ///
  typedef std::vector<NumByte> outlet_list;

 public:
  ProgramOptions(int argc, char** argv);

 private:
  void Parse(int argc, char** argv);

 public:
  outlet_list GetPowerOnList() const { return power_on_list_; }
  outlet_list GetPowerOffList() const { return power_off_list_; }
  outlet_list GetToggleList() const { return toggle_list_; }

  // In case an error occurs while parsing command line.
  bool IsEmpty() const { return need_help_; }

  bool NeedPowerOn() const { return !power_on_list_.empty(); }
  bool NeedPowerOff() const { return !power_off_list_.empty(); }
  bool NeedToggle() const { return !toggle_list_.empty(); }

 private:
  bool need_help_ = true;
  outlet_list power_on_list_;
  outlet_list power_off_list_;
  outlet_list toggle_list_;
};


#endif  // SRC_POWER_CONTROLLER_PROGRAM_OPTIONS_H_
