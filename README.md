# lcd1602 Library
This library provides support for the LCD1602 and LCD2004 devices connected to an i2c master device via a PCF8574 serial-to-parallel converter chip.

This library currently supports the following platforms:
* Linux i2c subsystem
* esp-idf

# Usage

## API

The API for this library can be found in the `include/lcd1602.h` header file.

## Portability

Portability among various host platforms (e.g. Linux i2c device interface vs. the esp-idf i2c driver interface) is accomplished via a platform-specific `lcd1602_lowlevel_config` structure which is defined at compile-time for the project based on build environment and/or toolchain hints. An example configuration for `lcd1602_lowlevel_config` for Linux is:

```bash
lcd1602_lowlevel_config config;
config.device = "/dev/i2c-0";
```

An example configuration for `lcd1602_lowlevel_config` for esp-idf is:

```bash
lcd1602_lowlevel_config config;
config.port = I2C_NUM_0;
config.pin_sda = GPIO_NUM_21;
config.pin_scl = GPIO_NUM_22;
```

Note that the members of the `lcd1602_lowlevel_config` change (at compile-time) based on the target platform.

# Example Applications

Example applications are provided for each of the supported platforms.

# License
All files delivered with this library are copyright 2024 Zorxx Software and released under the MIT license. See the `LICENSE` file for details.