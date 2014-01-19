{
  'targets': [
    {
      'target_name': 'power_ctl',
      'sources': [
        './src/power_controller/power_ctl.cc',
        './src/power_controller/node_power_ctl.cc',
        './src/power_controller/addon.cc',
      ],
      'cflags_cc': [
        '-std=c++0x'
      ],
      'defines': [
        '_FILE_OFFSET_BITS=64',
        '_LARGEFILE_SOURCE',
      ],
      'include_dirs+': [
        'src/'
      ],
      'conditions' : [
          ['OS=="linux"', {
            'variables': {
              'libusb_path': "./src/power_controller/third_party/libusb/libusb-1.0.9"
            },
            'include_dirs+': [
              '<(libusb_path)/include/libusb-1.0/'
            ],
            'libraries': [
              '../src/power_controller/third_party/libusb/libusb-1.0.9/lib/libusb-1.0.so'
            ],
          }]
      ]
    }
  ]
}
