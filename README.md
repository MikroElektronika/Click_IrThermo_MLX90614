![MikroE](http://www.mikroe.com/img/designs/beta/logo_small.png)

---

# IrThermo_3V3 Click

- **CIC Prefix**  : IRTHRM3V3
- **Author**      : MikroE Team
- **Verison**     : 1.0.0
- **Date**        : Jan 2018.

---

### Software Support

We provide a library for the IrThermo_3V3 Click on our [http://libstock.mikroe.com/projects/view/495/irthermo-3-3v-click-example) 
page, as well as a demo application (example), developed using MikroElektronika 
[compilers](http://shop.mikroe.com/compilers). The demo can run on all the main 
MikroElektronika [development boards](http://shop.mikroe.com/development-boards).

**Library Description**


Key functions :

- ``` float irthrm3v3_getTambient() ``` - Returns  ambient temperature value in Celsius
- ```float irthrm3v3_getTobject() ``` - Returns object temperature value in Celsius


**Examples Description**

- System Initialization -  Initialization I2C and UART modules
- Application Initialization - Initialization IrThermo 3V3 Driver
- Application Task - Reading Ambient Temperature and displaying the value periodically


```.c
void applicationTask()
{
    measuredTemperature = irthrm3v3_getTambient();
    FloatToStr(measuredTemperature,text);
    mikrobus_logWrite("Ambient Temperature: ",_LOG_TEXT);
    mikrobus_logWrite(text,_LOG_LINE);
    Delay_ms(1000);
}
```

The full application code, and ready to use projects can be found on our 
[LibStock](http://libstock.mikroe.com/projects/view/495/irthermo-3-3v-click-example) page.

Other mikroE Libraries used in the example:

- I2C
- UART
- Conversions

**Additional notes and informations**

Depending on the development board you are using, you may need 
[USB UART click](http://shop.mikroe.com/usb-uart-click), 
[USB UART 2 Click](http://shop.mikroe.com/usb-uart-2-click) or 
[RS232 Click](http://shop.mikroe.com/rs232-click) to connect to your PC, for 
development systems with no UART to USB interface available on the board. The 
terminal available in all Mikroelektronika 
[compilers](http://shop.mikroe.com/compilers), or any other terminal application 
of your choice, can be used to read the message.

---
### Architectures Supported

#### mikroC

| STM | KIN | CEC | MSP | TIVA | PIC | PIC32 | DSPIC | AVR | FT90x |
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| x | x | x | x | x | x | x | x | x | x |

#### mikroBasic

| STM | KIN | CEC | MSP | TIVA | PIC | PIC32 | DSPIC | AVR | FT90x |
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| x | x | x | x | x | x | x | x | x | x |

#### mikroPascal

| STM | KIN | CEC | MSP | TIVA | PIC | PIC32 | DSPIC | AVR | FT90x |
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| x | x | x | x | x | x | x | x | x | x |

---
---
