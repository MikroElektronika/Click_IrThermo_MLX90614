/*
 * Project name:
     IrThermo 3.3
 * Copyright:
     (c) Mikroelektronika, 2011.
 * Revision History:
     20120913:
       - initial release (JK);
 * Description:
     This example demonstrates reading from Ir thermo sensor MLX90614ESF-BAA
     Temperature is converted into Celsius degrees and displayed on LCD.
 * Test configuration:
     MCU:             LM3S9B95
                      http://www.ti.com/lit/ds/symlink/lm3s9b95.pdf
     Dev. Board:      EasyMx v7 for STELLARIS(R) ARM(R)
                      http://www.mikroe.com/easymx-pro/stellaris/
     Oscillator:      XT-PLL, 80.000MHz
     Ext. Modules:    Ir Thermo 3.3 click : ac:IrThermo_click
                      http://www.mikroe.com/click/irthermo-3.3v/
     SW:              mikroC PRO for ARM
                      http://www.mikroe.com/mikroc/arm/
 * NOTES:
     - Place IrThermo 3.3 click into mikroBUS socket1.
     - Turn ON TFT control on SW12
     - Turn ON TFT Data on SW11
     - Turn ON Touch Panel on SW 14
*/
#include "resources.h"

// Ir Termo click constants
const _IR_THERMO_ADDR = 0x5A;
const _AMB_TEMP       = 0x06;
const _OBJ_TEMP       = 0x07;
const _FAHRENHEIT     = 0x01;
const _CELSIUS        = 0x02;

// Globals
float Temp;
char temp_format;
unsigned int Xcoord, Ycoord;
char counter;
const PRESSED_COLOR = 0xE71C;

// Button declarations
typedef struct Button{
  unsigned int x1;
  unsigned int y1;
  unsigned int x2;
  unsigned int y2;
} TButton;

TButton Button1 = {
  170,
  150,
  220,
  185
};

TButton Button2 = {
  240,
  150,
  290,
  185
};

/**************************************************************************************************
* Draw Screen
**************************************************************************************************/
void DrawScr(){
  TFT_Fill_Screen(CL_WHITE);
  TFT_Set_Pen(CL_Black, 1);
  TFT_Line(20, 220, 300, 220);
  TFT_LIne(20,  46, 300,  46);
  TFT_Set_Font(&HandelGothic_BT21x22_Regular, CL_RED, FO_HORIZONTAL);
  TFT_Write_Text("IrThermo click", 75, 14);
  TFT_Set_Font(&Verdana12x13_Regular, CL_BLACK, FO_HORIZONTAL);
  TFT_Write_Text("EasyMX PRO for Stellaris", 19, 223);
  TFT_Set_Font(&Verdana12x13_Regular, CL_RED, FO_HORIZONTAL);
  TFT_Write_Text("www.mikroe.com", 200, 223);
  TFT_Set_Font(&TFT_defaultFont, CL_BLACK, FO_HORIZONTAL);
  TFT_Set_Font(&TFT_FontR, CL_BLACK, FO_HORIZONTAL);
  
  TFT_Set_Font(&Verdana12x13_Regular, CL_BLACK, FO_HORIZONTAL);
  TFT_Write_Text("Ambient", 70, 75);
  TFT_Set_Font(&HandelGothic_BT21x22_Regular, CL_BLACK, FO_HORIZONTAL);
  TFT_Write_Text("Target", 70, 100);

  TFT_Set_Font(&TFT_FontR, CL_BLACK, FO_HORIZONTAL);
  
  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Rectangle_Round_Edges(Button1.x1, Button1.y1, Button1.x2, Button1.y2, 5);
  TFT_Set_Brush(1, PRESSED_COLOR, 0, 0, 0, 0);
  TFT_Rectangle_Round_Edges(Button2.x1, Button2.y1, Button2.x2, Button2.y2, 5);
  TFT_Set_Font(&Tahoma30x34, CL_BLACK, FO_HORIZONTAL);
  TFT_Write_Text(" °F", Button1.x1 + 5, Button1.y1);
  TFT_Write_Text(" °C", Button2.x1 + 5, Button2.y1);
}

/**************************************************************************************************
* Init function
**************************************************************************************************/

void InitMCU(){
  // Init TFT
  TFT_Init(320, 240);
  Delay_ms(150);
  TFT_Set_Font(&TFT_FontR,CL_BLACK, FO_HORIZONTAL);
  Delay_ms(100);
  
  GPIO_Digital_Output(&GPIO_PORTA, _GPIO_PINMASK_3);
  TFT_BLED = 1;
  DrawScr();
  
  ADC_Set_Input_Channel(_ADC_CHANNEL_10 | _ADC_CHANNEL_11);
  ADC0_Init_Advanced(_ADC_EXTERNAL_REF);

  TP_TFT_Init(320, 240, 11, 10);                       // Initialize touch panel
  TP_TFT_Set_ADC_Threshold(800);                       // Set touch panel ADC threshold
  TP_TFT_Set_Calibration_Consts(69, 925, 64, 917);     // Set calibration constants
  
  I2C0_Init_Advanced(100000, &_GPIO_MODULE_I2C0_B23);   // Initialize I2C bus for communication with IrThermo
}

/**************************************************************************************************
* Function ReadSensor(char Temp_Source)
* -------------------------------------------------------------------------------------------------
* Overview: Function Reads Ir Termo sensor via I2C interface
* Input: Temp_Source  0x06 for Ambient temperature
*                     0x07 for Target temperature
* Output: Value that's proportional to Temperature in Kelvin degrees
**************************************************************************************************/
float ReadSensor(char Temp_Source){
  unsigned char Temp_var[2];
  
  I2C0_Master_Slave_Addr_Set(_IR_THERMO_ADDR, _I2C_DIR_MASTER_TRANSMIT);  // Set Slave address for writing
  I2C0_Write(Temp_Source, _I2C_MASTER_MODE_BURST_SEND_START);             // Write proper address for reading
  I2C0_Master_Slave_Addr_Set(_IR_THERMO_ADDR, _I2C_DIR_MASTER_RECEIVE);   // Set Slave address for rading
  
  I2C0_MCS |= (0x0000000F & _I2C_MASTER_MODE_BURST_RECEIVE_START);        // Issue repeated start signal
  while(I2C_MCS_BUSY_bit);                                                // wait while the module is busy
  Temp_var[0] = I2C0_MDR;                                                 // Read first byte from the slave
  
  I2C0_Read(&Temp_var[1], _I2C_MASTER_MODE_BURST_RECEIVE_FINISH);         // Read last byte from the slave
  
  return ((Temp_var[1] << 8) + Temp_var[0]);
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
  char text[40];
  
  FloatToStr(temperature, text); // Convert temperature into string
  if (text[1] == '.')            // display it with only two decimals
    text[4] = 0;
  if (text[2] == '.')
    text[5] = 0;
  if (text[3] == '.')
    text[6] = 0;
  if (text[4] == '.')
    text[7] = 0;
  
  TFT_Set_Pen(CL_WHITE, 1);
  TFT_Set_Brush(1, CL_WHITE, 0, 0, 0, 0);
  
  if (Temp_Source == _AMB_TEMP){  // Display result
    TFT_Set_Font(&Digital711x21_Regular, CL_BLACK, FO_HORIZONTAL);
    TFT_Rectangle(180, 75, 220, 95);
    TFT_Write_Text(text, 180, 75);
  }
  if (Temp_Source == _OBJ_TEMP){
    TFT_Set_Font(&Digital723x40, CL_BLACK, FO_HORIZONTAL);
    TFT_Rectangle(180, 100, 270, 135);
    TFT_Write_Text(text, 180, 100);
  }
}

/**************************************************************************************************
* Function Read_Temperature(char Source)
* -------------------------------------------------------------------------------------------------
* Overview: Function Reads Ir Termo sensor and converts result into degrees
* Input: Temp_Source  0x06 for Ambient temperature
*                     0x07 for Target temperature
* Output: Value in Celsius or fahrenheit degrees
**************************************************************************************************/
float Read_Temperature(char Source){
float Temperatue;

  Temperatue = ReadSensor(Source);          // Read Ambient temperature
  if (temp_format == _FAHRENHEIT)
    Temperatue = ((Temperatue * 0.02) - 273.15) * 9.0/5 + 32; // Convert result in FAHRENHEIT degrees
  else if (temp_format == _CELSIUS)
    Temperatue = (Temperatue * 0.02) - 273.15;       // Convert result in Celsius degrees
  else
    Temperatue = 0;

  return Temperatue;
}

/**************************************************************************************************
* Main function
**************************************************************************************************/
void main() {
  InitMCU();
  temp_format = 2;
  counter = 200;
  
  // infinite loop
  while (1){
    // process pressed buttons
    if (TP_TFT_Press_Detect()) {
      if (TP_TFT_Get_Coordinates(&Xcoord, &Ycoord) == 0){
        // button1
        if ((Xcoord > Button1.x1) && (Xcoord < Button1.x2) && (Ycoord > Button1.y1) && (Ycoord < Button1.y2)){
          if (temp_format == 2){
            temp_format = 1;
            TFT_Set_Pen(CL_BLACK, 1);
            TFT_Set_Brush(1, CL_WHITE, 0, 0, 0, 0);
            TFT_Rectangle_Round_Edges(Button2.x1, Button2.y1, Button2.x2, Button2.y2, 5);
            TFT_Set_Brush(1, PRESSED_COLOR, 0, 0, 0, 0);
            TFT_Rectangle_Round_Edges(Button1.x1, Button1.y1, Button1.x2, Button1.y2, 5);
            TFT_Set_Font(&Tahoma30x34, CL_BLACK, FO_HORIZONTAL);
            TFT_Write_Text(" °F", Button1.x1 + 5, Button1.y1);
            TFT_Write_Text(" °C", Button2.x1 + 5, Button2.y1);
          }
        }
        // button2
        else if ((Xcoord > Button2.x1) && (Xcoord < Button2.x2) && (Ycoord > Button2.y1) && (Ycoord < Button2.y2)){
          if (temp_format == 1){
            temp_format = 2;
            TFT_Set_Pen(CL_BLACK, 1);
            TFT_Set_Brush(1, CL_WHITE, 0, 0, 0, 0);
            TFT_Rectangle_Round_Edges(Button1.x1, Button1.y1, Button1.x2, Button1.y2, 5);
            TFT_Set_Brush(1, PRESSED_COLOR, 0, 0, 0, 0);
            TFT_Rectangle_Round_Edges(Button2.x1, Button2.y1, Button2.x2, Button2.y2, 5);
            TFT_Set_Font(&Tahoma30x34, CL_BLACK, FO_HORIZONTAL);
            TFT_Write_Text(" °F", Button1.x1 + 5, Button1.y1);
            TFT_Write_Text(" °C", Button2.x1 + 5, Button2.y1);
          }
        }
      }
    }
    
    if (counter > 150){
      Temp = Read_Temperature(_AMB_TEMP);
      Display_Temperature(_AMB_TEMP, Temp);  // Display result

      Temp = Read_Temperature(_OBJ_TEMP);    // Read Ambient temperature
      Display_Temperature(_OBJ_TEMP, Temp);  // Display result
    
      counter = 0;
    }
    else
      counter ++;
  
    Delay_ms(2);                             // small delay
  }
}