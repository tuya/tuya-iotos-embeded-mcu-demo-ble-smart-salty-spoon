# Tuya IoTOS Embedded Mcu Demo  Ble Salty Spoon

[English](./README.md) | [中文](./README_zh.md)

## Introduction  

This Demo uses the Tuya smart cloud platform, Tuya smart APP and IoTOS Embedded MCU SDK to realize a salty spoon.

The implemented features include:

+ voltage detection
+ frequency control


## Quick start  

### Compile & Burn
+ Download  Tuya IoTOS Embeded Code
+ Execute the test.uvprojx file
+ Click Compile in the software and complete the download


### File introduction 

```
├── SYSTEM
│   ├── main.c
│   └── main.h
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

Entry file：main.c

Important functions：main()

+ Initialize and configure MCU IO port, USART, timer, etc. All events are polled and judged in while(1)。




### DataPoint related

+ DP point processing: mcu_dp_value_update()

| function name | unsigned char mcu_dp_value_update(unsigned char dpid,unsigned long value) |
| ------------- | ------------------------------------------------------------ |
| dpid          | DP ID number                                                 |
| value         | DP data                                                      |
| Return        | SUCCESS: Success ERROR: Failure                              |



### I/O 列表 

| ADC  |  UASRT1  | Frequency |
| :--: | :------: | :-------: |
| P5.5 | P3.3 TXD |   P5.4    |
|      | P3.2 RXD |           |



## Related Documents

  Tuya Demo Center: https://developer.tuya.com/demo



## Technical Support

  You can get support for Tuya by using the following methods:

- Developer Center: https://developer.tuya.com
- Help Center: https://support.tuya.com/help
- Technical Support Work Order Center: [https://service.console.tuya.com](https://service.console.tuya.com/) 

