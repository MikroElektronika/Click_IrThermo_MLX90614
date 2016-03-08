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
const _IR_THERMO_ADDR = 0x00;
const _AMB_TEMP      = 0x06;
const _OBJ_TEMP      = 0x07;

float Temp;

/**************************************************************************************************
* Function ReadSensor(char Temp_Source)
* -------------------------------------------------------------------------------------------------
* Overview: Function Reads Ir Termo sensor via I2C interface
* Input: Temp_Source  0x06 for Ambient temperature
*                     0x07 for Target temperature
* Output: Value that's proportional to Temperature in Kelvin degrees
**************************************************************************************************/
float ReadSensor(char Temp_Source){
  unsigned int Temp_var, Temp_var1, Temp_var2;
  
  TWI_Start();                  // issue TWI start signal
  TWI_Write(_IR_THERMO_ADDR | 0x00);   // send byte via TWI  (device address << 1)
  TWI_Write(Temp_Source);       // send byte (data address)
  TWI_Start();                  // issue TWI signal repeated start
  TWI_Write(_IR_THERMO_ADDR | 0x01);   // send byte (device address << 1)
  Temp_var = TWI_Read(0);       // Read the LSB first
  Temp_var1 = TWI_Read(0);      // Read the MSB next
  Temp_var2 = TWI_Read(1);      // Read the PEC (CRC sum for string confirmation calc'ing)
  TWI_Stop();                   // issue TWI stop signal
  
  return ((Temp_var1 << 8) + Temp_var);
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
void Display_Temperature(char Temp_Source, float temperature){
  char text[15];
  
  FloatToStr(temperature, text); // Convert temperature into string
  if (text[1] == '.')            // display it with only two decimals
    text[4] = 0;
  if (text[2] == '.')
    text[5] = 0;
  if (text[3] == '.')
    text[6] = 0;

  strcat(text, "°C");            // add Celsius degree mark
  
  if (Temp_Source == _AMB_TEMP)  // Display result
    Lcd_Out(2, 1, text);
  if (Temp_Source == _OBJ_TEMP)
    Lcd_Out(2, 10, text);
}

//  Main
void main() {
  Lcd_Init();                           // performs Lcd initialization
  Lcd_Cmd(_LCD_CLEAR);                  // clear Lcd
  Lcd_Cmd(_LCD_CURSOR_OFF);             // set cursor off
  Lcd_Out(1, 2, "IrThermo click");
  Lcd_Out(2, 2, "Demonstration");
  Delay_ms(2000);

  TWI_Init(100000);                     // performs TWI initialization
  Delay_ms(100);

  Lcd_Cmd(_LCD_CLEAR);
  Lcd_Out(1, 1, "Amb. t: ");
  Lcd_Out(1, 9, " Obj. t:");
  
  // infinite loop
  while (1){
    Temp = ReadSensor(_AMB_TEMP);          // Read Ambient temperature
    Temp = (Temp * 0.02) - 273.15;         // Convert result in Celsius degrees

    Display_Temperature(_AMB_TEMP, Temp);  // Display result

    Temp = ReadSensor(_OBJ_TEMP);          // Read Ambient temperature
    Temp = (Temp * 0.02) - 273.15;         // Convert result in Celsius degrees

    Display_Temperature(_OBJ_TEMP, Temp);  // Display result

    Delay_ms(500);                         // wait
  }
}