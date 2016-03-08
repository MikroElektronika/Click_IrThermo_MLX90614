/*
 * Project name:
     IrThermo 3.3V
 * Copyright:
     (c) Mikroelektronika, 2011.
 * Revision History:
     20120913:
       - initial release (JK);
 * Description:
     This example demonstrates reading from Ir thermo sensor MLX90614ESF-BAA
     Temperature is converted into Celsius degrees and displayed on LCD.
 * Test configuration:
     MCU:             ATMEGA32
                      http://www.atmel.com/Images/doc2503.pdf
     Dev.Board:       EasyAVR v7
                      http://www.mikroe.com/easyavr/
     Oscillator:      External Clock 08.0000 MHz
     Ext. Modules:    Ir Thermo click : ac:IrThermo_click
                      http://www.mikroe.com/click/irthermo-3.3v/
                      Character Lcd 2x16 : ac:LCD
                      http://www.mikroe.com/store/components/
     SW:              mikroC PRO for AVR
                      http://www.mikroe.com/mikroc/avr/
 * NOTES:
     - Place IrThermo click into mikroBUS socket1.
     - Put jumper J5 in 3.3V position.
     - Turn off PORTC LEDs (SW10.7). (board specific)
     - Turn on LCD backlight switch SW3.1. (board specific)
     - Turn on I2C3 lines on SW6. (board specific)
 */

// LCD module connections
sbit LCD_RS at PORTA2_bit;
sbit LCD_EN at PORTD6_bit;
sbit LCD_D4 at PORTC4_bit;
sbit LCD_D5 at PORTC5_bit;
sbit LCD_D6 at PORTC6_bit;
sbit LCD_D7 at PORTC7_bit;

sbit LCD_RS_Direction at DDA2_bit;
sbit LCD_EN_Direction at DDD6_bit;
sbit LCD_D4_Direction at DDC4_bit;
sbit LCD_D5_Direction at DDC5_bit;
sbit LCD_D6_Direction at DDC6_bit;
sbit LCD_D7_Direction at DDC7_bit;
// End LCD module connections

// Ir Termo click constants
const int _IR_THERMO_ADDR       = 0x00;
const int _AMB_TEMP             = 0x06;
const int _OBJ_TEMP             = 0x07;

float temp;

/**************************************************************************************************
* Function ReadSensor(char Temp_Source)
* -------------------------------------------------------------------------------------------------
* Overview: Function Reads Ir Termo sensor via I2C interface
* Input: Temp_Source  0x06 for Ambient temperature
*                     0x07 for Target temperature
* Output: Value that's proportional to Temperature in Kelvin degrees
**************************************************************************************************/
float read_sensor( char temp_source )
{
    unsigned int temp_var, temp_var1, temp_var2;
  
    // Issue TWI start signal
    TWI_Start();

    // Send byte via TWI  (device address << 1)
    TWI_Write( _IR_THERMO_ADDR | 0x00 );

    // Send byte (data address)
    TWI_Write( temp_source );

    // Issue TWI signal repeated start
    TWI_Start();

    // Send byte (device address << 1)
    TWI_Write( _IR_THERMO_ADDR | 0x01 );

    // Read the LSB first
    temp_var = TWI_Read( 0 );

    // Read the MSB next
    temp_var1 = TWI_Read( 0 );

    // Read the PEC (CRC sum for string confirmation calculating)
    temp_var2 = TWI_Read( 1 );

    // Issue TWI stop signal
    TWI_Stop();
  
    return ( ( temp_var1 << 8 ) + temp_var );
}

/**************************************************************************************************
* Function Display_Temperature(char Temp_Source, float temperature)
* -------------------------------------------------------------------------------------------------
* Overview: Function Displays temperature on LCD
* Input: Temp_Source  0x06 for Ambient temperature
*                     0x07 for Target temperature
*        temperature  value of temperature that will be written on LCD
* Output: Nothing
**************************************************************************************************/
void display_temperature( char temp_source,
                          float temperature )
{
    char text[ 15 ];
  
    FloatToStr( temperature, text );

    // Display it with only two decimals
    if( text[ 1 ] == '.' )
        text[4] = 0;

    if( text[ 2 ] == '.' )
        text[ 5 ] = 0;

    if( text[ 3 ] == '.' )
        text[ 6 ] = 0;

    strcat( text, "°C" );
  
    // Display result
    if( temp_source == _AMB_TEMP )
        Lcd_Out( 2, 1, text );

    if( temp_source == _OBJ_TEMP )
        Lcd_Out( 2, 10, text );
}

//  Main
void main()
{
    Lcd_Init();
    Lcd_Cmd( _LCD_CLEAR );
    Lcd_Cmd( _LCD_CURSOR_OFF );
    Lcd_Out( 1, 2, "IrThermo click" );
    Lcd_Out( 2, 2, "Demonstration" );
    Delay_ms( 2000 );
    TWI_Init( 100000 );
    Delay_ms( 100 );
    Lcd_Cmd( _LCD_CLEAR );
    Lcd_Out( 1, 1, "Amb. t: " );
    Lcd_Out( 1, 9, " Obj. t:" );
  
    while ( 1 )
    {
        // Read Ambient temperature
        temp = read_sensor( _AMB_TEMP );

        // Convert result in Celsius degrees
        temp = ( temp * 0.02 ) - 273.15;

        // Display result
        display_temperature( _AMB_TEMP, temp );

        // Read Ambient temperature
        temp = read_sensor( _OBJ_TEMP );

        // Convert result in Celsius degrees
        temp = ( temp * 0.02 ) - 273.15;

        // Display result
        display_temperature( _OBJ_TEMP, temp );
        Delay_ms( 500 );
    }
}
