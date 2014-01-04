// Copyright (c) 2006-2008 The Elastic-Beam Authors. Beerware License".

#include "power_controller/program_options.h"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

//-----------------------------------------------------------------------------
ProgramOptions::ProgramOptions(int argc, char** argv) {
  try {
    Parse(argc, argv);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
};


void ProgramOptions::Parse(int argc, char** argv) {
  // Declare the supported options.
  po::options_description generic("Generic options");
  generic.add_options()
    ("version,v", "print version string")
    ("help,h", "produce help message")
    ;

  po::options_description power_options("Power options");
  power_options.add_options()
    ("activate,a", po::value<outlet_list>(), "power on specified outlet list")
    ("desactivate,d", po::value<outlet_list>(), "power off specified outlet list")
    ("toggle,t", po::value<outlet_list>(), "toggle specified outlet list")
    ;

  po::options_description cmd_line_opts("Allowed options");
  cmd_line_opts.add(generic).add(power_options);

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, cmd_line_opts), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << cmd_line_opts << "\n";
    return;
  }

  if (vm.count("version")) {
    // @todo (Amoriello) : Un versionning system + cmake
    std::cout << "v1.0" << std::endl;
    return;
  }

  if (vm.count("activate")) {
    power_on_list_ = vm["activate"].as<outlet_list>();
    need_help_ = false;
  }

  if (vm.count("desactivate")) {
    power_off_list_ = vm["desactivate"].as<outlet_list>();
    need_help_ = false;
  }

  if (vm.count("toggle")) {
    toggle_list_ = vm["toggle"].as<outlet_list>();
    need_help_ = false;
  }

  if (need_help_) {
    std::cout << cmd_line_opts << "\n";
    return;
  }
}
