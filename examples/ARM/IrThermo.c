/*
 * Project name:
 *     IrThermo 3.3
 * Copyright:
 *     (c) Mikroelektronika, 2011.
 * Revision History:
 *     20120913:
 *     - initial release (JK);
 * Description:
 *     This example demonstrates reading from Ir thermo sensor MLX90614ESF-BAA
 *     Temperature is converted into Celsius degrees and displayed on LCD.
 * Test configuration:
 *     MCU:             LM3S9B95
 *                      http://www.ti.com/lit/ds/symlink/lm3s9b95.pdf
 *     Dev. Board:      EasyMx v7 for STELLARIS(R) ARM(R)
 *                      http://www.mikroe.com/easymx-pro/stellaris/
 *     Oscillator:      XT-PLL, 80.000MHz
 *     Ext. Modules:    Ir Thermo 3.3 click : ac:IrThermo_click
 *                      http://www.mikroe.com/click/irthermo-3.3v/
 *     SW:              mikroC PRO for ARM
 *                      http://www.mikroe.com/mikroc/arm/
 * NOTES:
 *     - Place IrThermo 3.3 click into mikroBUS socket1.
 *     - Turn ON TFT control on SW12
 *     - Turn ON TFT Data on SW11
 *     - Turn ON Touch Panel on SW 14
 */

#include "resources.h"

/* Ir Termo click constants
 ****************************/
const int       _ir_thermo_addr         = 0x5A;
const int       _amb_temp       	= 0x06;
const int       _obj_temp       	= 0x07;
const int       _fahrenheit    		= 0x01;
const int       _celsius      		= 0x02;

/* Globals
 ***********/
float           temp;
char            temp_format;
unsigned int    x_coord, y_coord;
char            counter;
const int       pressed_color 		= 0xE71C;

/* Button declarations
 ***********************/
typedef struct Button
{
    unsigned int x1;
    unsigned int y1;
    unsigned int x2;
    unsigned int y2;

}button_t;

button_t button1 =
{
    170,
    150,
    220,
    185
};

button_t button2 =
{
    240,
    150,
    290,
    185
};


/* Draw Screen
 ***************/
void draw_screen()
{
    TFT_Fill_Screen( CL_WHITE );
    TFT_Set_Pen( CL_Black, 1 );
    TFT_Line( 20, 220, 300, 220 );
    TFT_LIne( 20,  46, 300,  46 );
    TFT_Set_Font( &HandelGothic_BT21x22_Regular, CL_RED, FO_HORIZONTAL );
    TFT_Write_Text( "IrThermo click", 75, 14 );
    TFT_Set_Font( &Verdana12x13_Regular, CL_BLACK, FO_HORIZONTAL );
    TFT_Write_Text( "EasyMX PRO for Stellaris", 19, 223 );
    TFT_Set_Font( &Verdana12x13_Regular, CL_RED, FO_HORIZONTAL );
    TFT_Write_Text( "www.mikroe.com", 200, 223 );
    TFT_Set_Font( &TFT_defaultFont, CL_BLACK, FO_HORIZONTAL );
    TFT_Set_Font( &TFT_FontR, CL_BLACK, FO_HORIZONTAL );
    TFT_Set_Font( &Verdana12x13_Regular, CL_BLACK, FO_HORIZONTAL );
    TFT_Write_Text( "Ambient", 70, 75 );
    TFT_Set_Font( &HandelGothic_BT21x22_Regular, CL_BLACK, FO_HORIZONTAL );
    TFT_Write_Text( "Target", 70, 100 );
    TFT_Set_Font( &TFT_FontR, CL_BLACK, FO_HORIZONTAL );
    TFT_Set_Pen( CL_BLACK, 1 );
    TFT_Rectangle_Round_Edges( button1.x1, button1.y1,
                               button1.x2, button1.y2, 5 );
    TFT_Set_Brush( 1, pressed_color, 0, 0, 0, 0 );
    TFT_Rectangle_Round_Edges( button2.x1, button2.y1,
                               button2.x2, button2.y2, 5 );
    TFT_Set_Font( &Tahoma30x34, CL_BLACK, FO_HORIZONTAL );
    TFT_Write_Text( " °F", button1.x1 + 5, button1.y1 );
    TFT_Write_Text( " °C", button2.x1 + 5, button2.y1 );
}

/* Init function
 *****************/
void init_mcu()
{
    TFT_Init( 320, 240 );
    Delay_ms( 150 );
    TFT_Set_Font( &TFT_FontR,CL_BLACK, FO_HORIZONTAL );
    Delay_ms( 100 );
    GPIO_Digital_Output( &GPIO_PORTA, _GPIO_PINMASK_3 );
    TFT_BLED = 1;
    draw_screen();
    ADC_Set_Input_Channel( _ADC_CHANNEL_10 | _ADC_CHANNEL_11 );
    ADC0_Init_Advanced( _ADC_EXTERNAL_REF );

    // Initialize touch panel
    TP_TFT_Init( 320, 240, 11, 10 );

    // Set touch panel ADC threshold
    TP_TFT_Set_ADC_Threshold( 800 );

     // Set calibration constants
    TP_TFT_Set_Calibration_Consts( 69, 925, 64, 917 );

    // Initialize I2C bus for communication with IrThermo
    I2C0_Init_Advanced( 100000, &_GPIO_MODULE_I2C0_B23 );
}

/*****************************************************************************
* Function : read_sensor( char temp_source )
* ----------------------------------------------------------------------------
* Overview: Function Reads Ir Termo sensor via I2C interface
* Input: Temp_Source  0x06 for Ambient temperature
*                     0x07 for Target temperature
* Output: Value that's proportional to Temperature in Kelvin degrees
*****************************************************************************/
float read_sensor( char temp_source )
{
    unsigned char temp_var[ 2 ];

    // Set Slave address for writing
    I2C0_Master_Slave_Addr_Set( _ir_thermo_addr, _I2C_DIR_MASTER_TRANSMIT );

    // Write proper address for reading
    I2C0_Write( temp_source, _I2C_MASTER_MODE_BURST_SEND_START );

    // Set Slave address for rading
    I2C0_Master_Slave_Addr_Set( _ir_thermo_addr, _I2C_DIR_MASTER_RECEIVE );

    // Issue repeated start signal
    I2C0_MCS |= ( 0x0000000F & _I2C_MASTER_MODE_BURST_RECEIVE_START );

    // Wait while the module is busy
    while( I2C_MCS_BUSY_bit );

    // Read first byte from the slave
    temp_var[ 0 ] = I2C0_MDR;

    // Read last byte from the slave
    I2C0_Read( &temp_var[1], _I2C_MASTER_MODE_BURST_RECEIVE_FINISH );

    return ( ( temp_var[ 1 ] << 8 ) + temp_var[ 0 ] );
}

/*****************************************************************************
* Function : display_temperature( char temp_source, float temperature )
* ----------------------------------------------------------------------------
* Overview: Function Displays temperature on LCD
* Input: Temp_Source  0x06 for Ambient temperature
*                     0x07 for Target temperature
*        temperature  value of temperature that will be written on LCD
* Output: Nothing
*****************************************************************************/
void display_temperature( char temp_source,
                          float temperature )
{
    char text[ 40 ];

    // Convert temperature into string
    FloatToStr( temperature, text );

    // Display it with only two decimals
    if( text[ 1 ] == '.' )
        text[ 4 ] = 0;

    if( text[ 2 ] == '.' )
        text[ 5 ] = 0;

    if( text[ 3 ] == '.' )
        text[ 6 ] = 0;

    if( text[ 4 ] == '.' )
        text[ 7 ] = 0;

    TFT_Set_Pen( CL_WHITE, 1 );
    TFT_Set_Brush( 1, CL_WHITE, 0, 0, 0, 0 );

    if ( temp_source == _amb_temp )
    {
        TFT_Set_Font( &Digital711x21_Regular, CL_BLACK, FO_HORIZONTAL );
        TFT_Rectangle( 180, 75, 220, 95 );
        TFT_Write_Text( text, 180, 75 );
    }

    if ( temp_source == _obj_temp )
    {
        TFT_Set_Font( &Digital723x40, CL_BLACK, FO_HORIZONTAL );
        TFT_Rectangle( 180, 100, 270, 135 );
        TFT_Write_Text( text, 180, 100 );
    }
}

/*****************************************************************************
* Function : read_temperature( char source )
* ----------------------------------------------------------------------------
* Overview: Function Reads Ir Termo sensor and converts result into degrees
* Input: Temp_Source  0x06 for Ambient temperature
*                     0x07 for Target temperature
* Output: Value in Celsius or fahrenheit degrees
*****************************************************************************/
float read_temperature( char source )
{
    float temperatue;

    // Read Ambient temperature
    temperatue = read_sensor( source );

    if ( temp_format == _fahrenheit )
    {
        // Convert result in FAHRENHEIT degrees
        temperatue = ( ( temperatue * 0.02 ) - 273.15 ) * 9.0/5 + 32;

    } else if ( temp_format == _celsius ) {

        // Convert result in Celsius degrees
        temperatue = ( temperatue * 0.02 ) - 273.15;

    } else {

        temperatue = 0;
    }

    return temperatue;
}

/*****************************************************************************
* Main function
*****************************************************************************/
void main()
{
    init_mcu();
    temp_format = 2;
    counter = 200;
  
    while ( 1 )
    {
        // process pressed buttons
        if ( TP_TFT_Press_Detect() )
        {
            if ( TP_TFT_Get_Coordinates( &x_coord, &y_coord ) == 0 )
            {
                // button1
                if ( ( x_coord > button1.x1 ) && ( x_coord < button1.x2 ) &&
                     ( y_coord > button1.y1 ) && ( y_coord < button1.y2 ) )
                {
                    if ( temp_format == 2 )
                    {
                        temp_format = 1;
                        TFT_Set_Pen( CL_BLACK, 1 );
                        TFT_Set_Brush( 1, CL_WHITE, 0, 0, 0, 0 );
                        TFT_Rectangle_Round_Edges( button2.x1, button2.y1,
                                                   button2.x2, button2.y2, 5 );
                        TFT_Set_Brush( 1, pressed_color, 0, 0, 0, 0 );
                        TFT_Rectangle_Round_Edges( button1.x1, button1.y1,
                                                   button1.x2, button1.y2, 5 );
                        TFT_Set_Font( &Tahoma30x34, CL_BLACK, FO_HORIZONTAL );
                        TFT_Write_Text( " °F", button1.x1 + 5, button1.y1 );
                        TFT_Write_Text( " °C", button2.x1 + 5, button2.y1 );
                    }
                } else if ( ( x_coord > button2.x1 ) &&
                            ( x_coord < button2.x2 ) &&
                            ( y_coord > button2.y1 ) &&
                            ( y_coord < button2.y2 ) ) {

                    if ( temp_format == 1 )
                    {
                        temp_format = 2;
                        TFT_Set_Pen( CL_BLACK, 1 );
                        TFT_Set_Brush( 1, CL_WHITE, 0, 0, 0, 0 );
                        TFT_Rectangle_Round_Edges( button1.x1, button1.y1,
                                                   button1.x2, button1.y2, 5 );
                        TFT_Set_Brush( 1, pressed_color, 0, 0, 0, 0 );
                        TFT_Rectangle_Round_Edges( button2.x1, button2.y1,
                                                   button2.x2, button2.y2, 5 );
                        TFT_Set_Font( &Tahoma30x34, CL_BLACK, FO_HORIZONTAL );
                        TFT_Write_Text( " °F", button1.x1 + 5, button1.y1 );
                        TFT_Write_Text( " °C", button2.x1 + 5, button2.y1 );
                    }
                }
            }
        }
    
        if ( counter > 150 )
        {
            temp = read_temperature( _amb_temp ;

            // Display result
            display_temperature( _amb_temp, temp );

            // Read Ambient temperature
            temp = read_temperature( _obj_temp );

            // Display result
            display_temperature( _obj_temp, temp );
    
            counter = 0;

        } else {

            counter ++;
        }
  
        Delay_ms( 2 );
    }
}
