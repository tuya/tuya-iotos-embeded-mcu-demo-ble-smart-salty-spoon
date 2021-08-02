
This MCU_SDK is the MCU code automatically generated according to the product functions defined on the Tuya 
development platform. Modifications and supplements on this basis can quickly complete the MCU program.


Development steps:

1: Need to configure according to the actual situation of the product (reset the bluetooth button and bluetooth 
status indicator processing method, whether to support MCU upgrade, etc.), please modify this configuration in protocol.h;

2: To transplant this MCU_SDK, please check the transplantation steps in the protocol.c file, and complete the transplantation correctly. 
After transplantation, please complete the code of the data delivery processing and data reporting part to complete all bluetooth functions.


File overview:
This MCU_SDK includes nine files:
(1) protocol.h and protocol.c need to be modified by you.The protocol.h and protocol.c files contain detailed 
instructions for modification. Please read them carefully. 
(2) bluetooth.h file is the total header file, if you need to call bluetooth internal functions, please #include "bluetooth.h". 
(3) system.c and system.h are bluetooth function implementation codes without user modification. 
(4) All the functions that users need to call are implemented in mcu_api.c and mcu_api.h, and users do not need to 
modify them.	
(5) mcu_ota_handler.h and mcu_ota_handler.c is an OTA-related implementation example.You need to modify. Ota is closely related to the
chip and requires users to debug and modify according to their own chip platform. These two documents are for reference only.
