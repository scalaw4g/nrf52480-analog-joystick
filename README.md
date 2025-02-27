Modified the sketches from Glitzville's Magnetic Mini Keyboard Thumb Joystick V2 at https://www.printables.com/model/104949-magnetic-mini-keyboard-thumb-joystick-v2/comments.
- now supports analog BLE HID input using a Seeed XIAO NRF52480, direct connection to host device with Adafruit Bluefruit BLE HID library
- still exploring options for a dongle setup, using a peripheral joystick and central HID controller using the NRF52480 for reduced input latency and lower battery consumption. there seems to be limited support for seeed nrf52480 as a USB input device other than keyboard/mouse devices
