# Tuya IoTOS Embedded Mcu Demo  Ble Salty Spoon

[English](./README.md) | [中文](./README_zh.md)

## 简介 

本Demo通过涂鸦智能云平台、涂鸦智能APP、IoTOS Embeded MCU SDK实现一款能产生咸味的勺子。

已实现功能包括：

+ 电量检测

+ 频率控制

  



## 快速上手 

### 编译与烧录
+ 下载Tuya IoTOS Embeded Code

+ 执行test.uvprojx文件

+ 点击软件中的编译，并完成下载


### 文件介绍 

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



### Demo入口

入口文件：main.c

重要函数：main()

+ 对mcu的IO口，USART，定时器等进行初始化配置，所有事件在while(1)中轮询判断。



### DP点相关

+ 上报dp点处理: mcu_dp_value_update()

| 函数名 | unsigned char mcu_dp_value_update(unsigned char dpid,unsigned long value) |
| ------ | ------------------------------------------------------------ |
| dpid   | DP的ID号                                                     |
| value  | DP数据                                                       |
| Return | SUCCESS: 成功  ERROR: 失败                                   |



### I/O 列表 

| ADC  |  UASRT1  | Frequency |
| :--: | :------: | :-------: |
| P5.5 | P3.3 TXD |   P5.4    |
|      | P3.2 RXD |           |

## 相关文档

涂鸦Demo中心：https://developer.tuya.com/demo



## 技术支持

您可以通过以下方法获得涂鸦的支持:

- 开发者中心：https://developer.tuya.com
- 帮助中心: https://support.tuya.com/help
- 技术支持工单中心: [https://service.console.tuya.com](https://service.console.tuya.com/) 