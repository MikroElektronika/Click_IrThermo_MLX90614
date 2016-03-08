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
     MCU:             PIC18F45K22
                      http://ww1.microchip.com/downloads/en/DeviceDoc/41412D.pdf
     dev.board:       EasyPIC v7
                      http://www.mikroe.com/eng/products/view/757/easypic-v7-development-system/
     Oscillator:      HS-PLL 32.0000 MHz, 8.0000 MHz Crystal
     Ext. Modules:    Ir Thermo click : ac:IrThermo_click
                      http://www.mikroe.com/click/irthermo-3.3v/
                      character lcd 2x16 : ac:LCD
                      http://www.mikroe.com/eng/products/view/277/various-components/
     SW:              mikroC PRO for PIC
                      http://www.mikroe.com/mikroc/pic/
 * NOTES:
     - Place IrThermo click into mikroBUS socket1.
     - Put jumper J5 in 3.3V position.
     - Turn off PORTC LEDs (SW3.3). (board specific)
     - Turn on LCD backlight switch SW4.6. (board specific)
*/

// LCD module connections
sbit LCD_RS           at  LATB4_bit;
sbit LCD_EN           at  LATB5_bit;
sbit LCD_D4           at  LATB0_bit;
sbit LCD_D5           at  LATB1_bit;
sbit LCD_D6           at  LATB2_bit;
sbit LCD_D7           at  LATB3_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;
// End LCD module connections

// Ir Termo click constants
const int _ir_thermo_addr       = 0x5A;
const int _amb_temp             = 0x06;
const int _obj_temp             = 0x07;

// Globals
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
    unsigned int temp_var;

    // Issue I2C start signal
    I2C1_Start();

    // Send byte via I2C  (device address << 1)
    I2C1_Wr( _ir_thermo_addr << 1 );

    // Send byte (data address)
    I2C1_Wr( temp_source );

    // Issue I2C signal repeated start
    I2C1_Repeated_Start();

    // Send byte (device address << 1)
    I2C1_Wr( _ir_thermo_addr << 1 );

    // Read the data (NO acknowledge)
    temp_var = I2C1_Rd( 0 );
    temp_var = ( I2C1_Rd( 0 ) << 8 ) + temp_var;

    // Issue I2C stop signal
    I2C1_Stop();
  
    return temp_var;
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
        text[ 4 ] = 0;

    if( text[ 2 ] == '.' )
        text[ 5 ] = 0;

    if( text[ 3 ] == '.' )
        text[ 6 ] = 0;

    strcat( text, "°C" );
  
    if ( temp_source == _amb_temp )
        Lcd_Out( 2, 1, text );

    if ( temp_source == _obj_temp )
        Lcd_Out( 2, 10, text );
}

void main()
{
    // Configure PORTB pins as digital
    ANSELB = 0;

    // Configure PORTC pins as digital
    ANSELC = 0;

    Lcd_Init();
    Lcd_Cmd( _LCD_CLEAR );
    Lcd_Cmd( _LCD_CURSOR_OFF );
    Lcd_Out( 1, 1, "Initialising I2C" );
    I2C1_Init( 50000 );
    Delay_ms( 1000 );
    Lcd_Cmd( _LCD_CLEAR );
    Lcd_Out( 1, 1, "Init OK" );
    Delay_ms( 1000 );
    Lcd_Cmd( _LCD_CLEAR );
    Lcd_Out( 1, 1, "Amb. t: " );
    Lcd_Out( 1, 9, " Obj. t:" );

    while (1)
    {
        // Read Ambient temperature
        temp = read_sensor( _amb_temp );

        // Convert result in Celsius degrees
        temp = ( temp * 0.02 ) - 273.15;

        // Display result
        display_temperature( _amb_temp, temp );

        // Read Ambient temperature
        temp = read_sensor( _obj_temp );

        // Convert result in Celsius degrees
        temp = ( temp * 0.02 ) - 273.15;

        // Display result
        display_temperature( _obj_temp, temp );
        Delay_ms( 500 );
    }
}
