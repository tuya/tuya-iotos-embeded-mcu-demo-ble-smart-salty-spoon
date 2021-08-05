
# Tuya IoTOS Embedded Bluetooth LE Salty Spoon

[English](./README.md) | [中文](./README_zh.md)

## Introduction

In this demo, we will show you how to use the MCU SDK to develop a spoon that can enhance salty flavor and connect this spoon to the Tuya IoT Cloud with the Tuya Smart app.

Features:

+ Battery level detection

+ Frequency control

## Get started

### Compile and flash
+ Download Tuya IoTOS Embedded Code.

+ Run `test.uvprojx`.

+ Click **Compile** on the software to download the code.

### File introduction

```
├── SYSTEM
│   ├── main.c
│   └── main.h
└── SDK
    ├── mcu_api.c
    ├── mcu_api.h
    ├── protocol.c
    ├── protocol.h
    ├── system.c
    ├── system.h
    └── bluetooth.h

```

### Demo entry

Entry file: `main.c`

Main function: `main()`

+ Initialize and configure I/Os, USART, and timer of the MCU. All events are polled and determined in `while(1)`.

### Data point (DP)

+ Process DP data: `mcu_dp_value_update()`

    | Function | unsigned char mcu_dp_value_update(unsigned char dpid,unsigned long value) |
    | ------ | ------------------------------------------------------------ |
    | dpid | DP ID |
    | value | DP data |
    | Return | `SUCCESS`: DP data reporting succeeded. `ERROR`: DP data reporting failed. |

### Pin configuration

| ADC | UASRT1 | Frequency |
| :--: | :------: | :-------: |
| P5.5 | P3.3 TXD | P5.4 |
|      | P3.2 RXD |           |

## Reference

[Tuya Project Hub](https://developer.tuya.com/demo)

## Technical support

You can get support from Tuya with the following methods:

- [Tuya IoT Developer Platform](https://developer.tuya.com/en/)
- [Help Center](https://support.tuya.com/en/help)
- [Service & Support](https://service.console.tuya.com)[](https://service.console.tuya.com/)
