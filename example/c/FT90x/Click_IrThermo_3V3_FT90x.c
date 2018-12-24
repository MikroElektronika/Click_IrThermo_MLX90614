/*
Example for IrThermo_3V3 Click

    Date          : jan 2018.
    Author        : MikroE Team

Test configuration FT90x :
    
    MCU                : FT900
    Dev. Board         : EasyFT90x v7 
    FT90x Compiler ver : v2.2.1.0

---
Description :

The application is composed of three sections :

- System Initialization -  Initialization I2C and UART modules
- Application Initialization - Initialization IrThermo 3V3 Driver
- Application Task - Reading Ambient Temperature and displaying the value periodically



*/

#include "Click_IrThermo_3V3_types.h"
#include "Click_IrThermo_3V3_config.h"



char text[10];
float measuredTemperature;

void systemInit()
{

    mikrobus_i2cInit( _MIKROBUS1, &_IRTHRM3V3_I2C_CFG[0] );
    mikrobus_logInit( _LOG_USBUART_A, 9600 );

    Delay_ms( 100 );
}

void applicationInit()
{
    irthrm3v3_i2cDriverInit( (T_IRTHRM3V3_P)&_MIKROBUS1_GPIO, (T_IRTHRM3V3_P)&_MIKROBUS1_I2C, 0x5A );
}

void applicationTask()
{
   
    measuredTemperature = irthrm3v3_getTambient();
    FloatToStr(measuredTemperature,text);
    mikrobus_logWrite("Ambient Temperature: ",_LOG_TEXT);
    mikrobus_logWrite(text,_LOG_LINE);
    Delay_ms(1000);
}

void main()
{
    systemInit();
    applicationInit();

    while (1)
    {
            applicationTask();
    }
}