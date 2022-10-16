#ifndef __DISPLAY_
#define __DISPLAY_

#include <sleep.h>
#include "port_out.h"
#include "peripherals.h"
#include "moduleParameters.h"

//Makros für Delay: (nicht kalibriert, bei ms leichte Rundungsfehler)
#define _SLEEP_US(sleep_time) sleep(SB_SPARTANMC_FREQUENCY*sleep_time/3000000-6) //bis 13ms bei 60MHz
#define _SLEEP_MS(sleep_time) sleep_sleep((SB_SPARTANMC_FREQUENCY/1000-3)/3,sleep_time) //inner=Berechnung 1ms (gerundet), outer=Anzahl an Iterationen

//Display Reset Pin
#define DISP_RESET_PIN PORT_OUT.data


void Set_Column_Address_25664(unsigned char a, unsigned char b);
void Set_Row_Address_25664(unsigned char a, unsigned char b);
void Set_Write_RAM_25664();
void Set_Remap_Format_25664(unsigned char d);
void Set_Start_Line_25664(unsigned char d);
void Set_Display_Offset_25664(unsigned char d);
void Set_Display_Mode_25664(unsigned char d);
void Set_Partial_Display_25664(unsigned char a, unsigned char b, unsigned char c);
void Set_Function_Selection_25664(unsigned char d);
void Set_Display_On_Off_25664(unsigned char d);
void Set_Phase_Length_25664(unsigned char d);
void Set_Display_Clock_25664(unsigned char d);
void Set_Display_Enhancement_A_25664(unsigned char a, unsigned char b);
void Set_GPIO_25664(unsigned char d);
void Set_Precharge_Period_25664(unsigned char d);
void Set_Precharge_Voltage_25664(unsigned char d);
void Set_VCOMH_25664(unsigned char d);
void Set_Contrast_Current_25664(unsigned char d);
void Set_Master_Current_25664(unsigned char d);
void Set_Multiplex_Ratio_25664(unsigned char d);
void Set_Display_Enhancement_B_25664(unsigned char d);
void Set_Command_Lock_25664(unsigned char d);
//void Draw_Rectangle_25664(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e);
void Fill_RAM_25664(unsigned char Data);
void Fill_Block_25664(unsigned char Data, unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Checkerboard_25664();
void Grayscale_25664();
void Show_Font57_25664(unsigned char a,  char b, unsigned char c, unsigned char d);
void Show_String_25664(unsigned char a, char *Data_Pointer, unsigned char b, unsigned char c);
void Show_Pattern_25664(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Set_Gray_Scale_Table_25664();
void Set_Linear_Gray_Scale_Table_25664();
void OLED_Init_25664();

#endif //define __DISPLAY_ 
