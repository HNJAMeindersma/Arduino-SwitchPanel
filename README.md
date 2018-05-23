# Arduino-SwitchPanel
Switch Panel is a controller for relays, IR commands and RF signals. It runs on basicly any Arduino with at least 32 kb flash memory. By default there are 16 relay profiles. This is the same for the IR commands and RF signals, each. However, the user decides which and in what
order the profiles get used. It can be controlled by 3 button control, IR commands or RF signals. And the menu is displayed trough a Liquid Crystal LCD screen with I2C module. All widths and heights should be supported, however the 1602 and 2004 are tested and confirmed to work for sure.

##### Current version: 1.0
- Version 1.0 | Basic usage for relays trough 3 button control with output on I2C LCD screen.
- Version 1.1 | Future version will bring usage for IR and RF control.

##### Supported
- control by 3 button control (OK, NEXT, PREVIOUS)
- output for I2C LCD screen (1602 and 2004 tested)
- output for relays (16 profiles)

##### Unsupported
- control by IR commands (OK, NEXT, PREVIOUS, #)
- output for IR commands (16 profiles)
- control by RF signals (OK, NEXT, PREVIOUS, #)
- output for RF signals (16 profiles)
