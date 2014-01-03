#ifndef SRC_POWER_CONTROLLER_PROGRAM_OPTIONS_
#define SRC_POWER_CONTROLLER_PROGRAM_OPTIONS_

#include <vector>
#include <cstdint>
#include <iostream>

class ProgramOptions {
  // What a mess to read uint8_t as byte!!
  struct NumByte {
    uint8_t value;
    NumByte() : value() {}
    NumByte(const uint8_t &arg) : value(arg) {}
    NumByte(const NumByte &arg) : value(arg.value) {}
    operator uint8_t() const { return value; }

    friend std::istream& operator>>(std::istream& in, NumByte& valArg) {
      int i;
      in >> i;
      // Safe
      valArg.value = static_cast<uint8_t>(i);
      return in;
    }

    friend std::ostream& operator<<(std::ostream& out, NumByte& valArg) {
      out << static_cast<int>(valArg.value);
      return out;
    }
  };

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


#endif  // SRC_POWER_CONTROLLER_PROGRAM_OPTIONS_#endif