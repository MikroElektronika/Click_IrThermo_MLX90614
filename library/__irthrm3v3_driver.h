/*
    __irthrm3v3_driver.h

-----------------------------------------------------------------------------

  This file is part of mikroSDK.
  
  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

/**
@file   __irthrm3v3_driver.h
@brief    IrThermo_3V3 Driver
@mainpage IrThermo_3V3 Click
@{

@image html sch.jpg

@}

@defgroup   IRTHRM3V3
@brief      IrThermo_3V3 Click Driver
@{

| Global Library Prefix | **IRTHRM3V3** |
|:---------------------:|:-----------------:|
| Version               | **1.0.0**    |
| Date                  | **jan 2018.**      |
| Developer             | **MikroE Team**     |

*/
/* -------------------------------------------------------------------------- */

#include "stdint.h"

#ifndef _IRTHRM3V3_H_
#define _IRTHRM3V3_H_

/** 
 * @macro T_IRTHRM3V3_P
 * @brief Driver Abstract type 
 */
#define T_IRTHRM3V3_P    const uint8_t*

/** @defgroup IRTHRM3V3_COMPILE Compilation Config */              /** @{ */

//  #define   __IRTHRM3V3_DRV_SPI__                            /**<     @macro __IRTHRM3V3_DRV_SPI__  @brief SPI driver selector */
   #define   __IRTHRM3V3_DRV_I2C__                            /**<     @macro __IRTHRM3V3_DRV_I2C__  @brief I2C driver selector */                                          
// #define   __IRTHRM3V3_DRV_UART__                           /**<     @macro __IRTHRM3V3_DRV_UART__ @brief UART driver selector */ 

                                                                       /** @} */
/** @defgroup IRTHRM3V3_VAR Variables */                           /** @{ */

 extern const uint8_t _HEARTRATE_INT_STATUS      ;

extern const uint8_t _IRTHRM3V3_RAW_DATA_CH1;
extern const uint8_t _IRTHRM3V3_RAW_DATA_CH2;
extern const uint8_t _IRTHRM3V3_TA          ;
extern const uint8_t _IRTHRM3V3_TOBJ1       ;
extern const uint8_t _IRTHRM3V3_TOBJ2       ;
                                                                       /** @} */
/** @defgroup IRTHRM3V3_TYPES Types */                             /** @{ */



                                                                       /** @} */
#ifdef __cplusplus
extern "C"{
#endif



/** @defgroup IRTHRM3V3_INIT Driver Initialization */              /** @{ */

#ifdef   __IRTHRM3V3_DRV_SPI__
void irthrm3v3_spiDriverInit(T_IRTHRM3V3_P gpioObj, T_IRTHRM3V3_P spiObj);
#endif
#ifdef   __IRTHRM3V3_DRV_I2C__
void irthrm3v3_i2cDriverInit(T_IRTHRM3V3_P gpioObj, T_IRTHRM3V3_P i2cObj, uint8_t slave);
#endif
#ifdef   __IRTHRM3V3_DRV_UART__
void irthrm3v3_uartDriverInit(T_IRTHRM3V3_P gpioObj, T_IRTHRM3V3_P uartObj);
#endif

// GPIO Only Drivers - remove in other cases
void irthrm3v3_gpioDriverInit(T_IRTHRM3V3_P gpioObj);
                                                                       /** @} */
/** @defgroup IRTHRM3V3_FUNC Driver Functions */                   /** @{ */


/**
 * @brief Reads Temperature ambient from sensor
 *
 * Returns float temperature value in Celsius
 */
float irthrm3v3_getTambient();
/**
 * @brief Reads Temperature object1 from sensor
 *
 * Returns float temperature value in Celsius
 */
float irthrm3v3_getTobject();
/**
 * @brief Generic function for reading values from RAM
 *
 * @param[in] address - address of register
 * 
 * Retuns 16bit value of register
 */

uint16_t irthrm3v3_readReg(uint8_t address);






                                                                       /** @} */
#ifdef __cplusplus
} // extern "C"
#endif
#endif

/**
    @example Click_IrThermo_3V3_STM.c
    @example Click_IrThermo_3V3_TIVA.c
    @example Click_IrThermo_3V3_CEC.c
    @example Click_IrThermo_3V3_KINETIS.c
    @example Click_IrThermo_3V3_MSP.c
    @example Click_IrThermo_3V3_PIC.c
    @example Click_IrThermo_3V3_PIC32.c
    @example Click_IrThermo_3V3_DSPIC.c
    @example Click_IrThermo_3V3_AVR.c
    @example Click_IrThermo_3V3_FT90x.c
    @example Click_IrThermo_3V3_STM.mbas
    @example Click_IrThermo_3V3_TIVA.mbas
    @example Click_IrThermo_3V3_CEC.mbas
    @example Click_IrThermo_3V3_KINETIS.mbas
    @example Click_IrThermo_3V3_MSP.mbas
    @example Click_IrThermo_3V3_PIC.mbas
    @example Click_IrThermo_3V3_PIC32.mbas
    @example Click_IrThermo_3V3_DSPIC.mbas
    @example Click_IrThermo_3V3_AVR.mbas
    @example Click_IrThermo_3V3_FT90x.mbas
    @example Click_IrThermo_3V3_STM.mpas
    @example Click_IrThermo_3V3_TIVA.mpas
    @example Click_IrThermo_3V3_CEC.mpas
    @example Click_IrThermo_3V3_KINETIS.mpas
    @example Click_IrThermo_3V3_MSP.mpas
    @example Click_IrThermo_3V3_PIC.mpas
    @example Click_IrThermo_3V3_PIC32.mpas
    @example Click_IrThermo_3V3_DSPIC.mpas
    @example Click_IrThermo_3V3_AVR.mpas
    @example Click_IrThermo_3V3_FT90x.mpas
*/                                                                     /** @} */
/* -------------------------------------------------------------------------- */
/*
  __irthrm3v3_driver.h

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