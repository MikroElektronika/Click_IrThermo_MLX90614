/*
    __irthrm3v3_driver.c

-----------------------------------------------------------------------------

  This file is part of mikroSDK.

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

#include "__irthrm3v3_driver.h"
#include "__irthrm3v3_hal.c"

/* ------------------------------------------------------------------- MACROS */



/* ---------------------------------------------------------------- VARIABLES */

#ifdef   __IRTHRM3V3_DRV_I2C__
static uint8_t _slaveAddress;
#endif


 const uint8_t _IRTHRM3V3_RAW_DATA_CH1      = 0x04;
 const uint8_t _IRTHRM3V3_RAW_DATA_CH2      = 0x05;
 const uint8_t _IRTHRM3V3_TA                = 0x06;
 const uint8_t _IRTHRM3V3_TOBJ1             = 0x07;
 const uint8_t _IRTHRM3V3_TOBJ2             = 0x08;


/* -------------------------------------------- PRIVATE FUNCTION DECLARATIONS */



/* --------------------------------------------- PRIVATE FUNCTION DEFINITIONS */



/* --------------------------------------------------------- PUBLIC FUNCTIONS */

#ifdef   __IRTHRM3V3_DRV_SPI__

void irthrm3v3_spiDriverInit(T_IRTHRM3V3_P gpioObj, T_IRTHRM3V3_P spiObj)
{
    hal_spiMap( (T_HAL_P)spiObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
    // ... configure CHIP
}

#endif
#ifdef   __IRTHRM3V3_DRV_I2C__

void irthrm3v3_i2cDriverInit(T_IRTHRM3V3_P gpioObj, T_IRTHRM3V3_P i2cObj, uint8_t slave)
{
    _slaveAddress = slave;
    hal_i2cMap( (T_HAL_P)i2cObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
    // ... configure CHIP
}

#endif
#ifdef   __IRTHRM3V3_DRV_UART__

void irthrm3v3_uartDriverInit(T_IRTHRM3V3_P gpioObj, T_IRTHRM3V3_P uartObj)
{
    hal_uartMap( (T_HAL_P)uartObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
    // ... configure CHIP
}

#endif

// GPIO Only Drivers - remove in other cases
void irthrm3v3_gpioDriverInit(T_IRTHRM3V3_P gpioObj)
{
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
}

/* ----------------------------------------------------------- IMPLEMENTATION */

float irthrm3v3_getTambient()
{
    uint8_t temp[2];
    uint16_t value;
    float temperature;
    temp[0] = 0x06;

    hal_i2cStart();
    hal_i2cWrite(_slaveAddress,temp,1, END_MODE_RESTART);
    hal_i2cRead(_slaveAddress,temp,2,END_MODE_STOP);

    
    value = ((uint16_t)temp[1] << 8) | (temp[0]);
    temperature = (float)(value * 0.02) - 273.15;

    return temperature;

}


float irthrm3v3_getTobject()
{
    uint8_t temp[2];
    uint16_t value;
    float temperature;
    temp[0] = 0x07;

    hal_i2cStart();
    hal_i2cWrite(_slaveAddress,temp,1, END_MODE_RESTART);
    hal_i2cRead(_slaveAddress,temp,2,END_MODE_STOP);

    
    value = ((uint16_t)temp[1] << 8) | (temp[0]);
    temperature = (float)(value * 0.02) - 273.15;

    return temperature;

}

uint16_t irthrm3v3_readReg(uint8_t address)
{
    uint8_t temp[2];
    uint16_t value;
    temp[0] = address;

    hal_i2cStart();
    hal_i2cWrite(_slaveAddress,temp,1, END_MODE_RESTART);
    hal_i2cRead(_slaveAddress,temp,2,END_MODE_STOP);

    
    value = ((uint16_t)temp[1] << 8) | (temp[0]);

    return value;

}



/* -------------------------------------------------------------------------- */
/*
  __irthrm3v3_driver.c

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the MikroElektonika.

4. Neither the name of the MikroElektonika nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY MIKROELEKTRONIKA ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MIKROELEKTRONIKA BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------------- */