#Thx for nonolith (node-usb)

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
            'include_dirs+': [
              '<!@(pkg-config libusb-1.0 --cflags-only-I | sed s/-I//g)'
            ],
            'libraries': [
              '<!@(pkg-config libusb-1.0 --libs)'
            ],
            'defines': [
              #'USE_POLL',
            ]
          }],
          ['OS=="mac"', {
            'include_dirs+': [
              '<!@(pkg-config libusb-1.0 --cflags-only-I | sed s/-I//g)'
            ],
            'libraries': [
              '<!@(pkg-config libusb-1.0 --libs)'
            ],
          }],
          ['OS=="win"', {
            'variables': {
              # Path to extracted libusbx windows binary package from http://libusbx.org/
              'libusb_path': "C:/Program Files/libusb"
            },
            'defines':[
              'WIN32_LEAN_AND_MEAN'
            ],
            'include_dirs+': [
              '<(libusb_path)/include/libusbx-1.0'
            ],
            'msvs_settings': {
              'VCCLCompilerTool': {
                'AdditionalOptions': [ '/EHsc /MD' ],
              },
            },
            "conditions" : [
              ["target_arch=='ia32'", {
                'libraries': [
                   '<(libusb_path)/MS32/static/libusb-1.0.lib'
                ]
              }],
              ["target_arch=='x64'", {
                'libraries': [
                   '<(libusb_path)/MS64/static/libusb-1.0.lib'
                ]
              }]
            ]
          }]
      ]
    }
  ]
}
