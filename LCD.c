#include "tm4c123gh6pm.h"
#include "delay.h"


// LCD data pins and control pins is defined in the file "tm4c123gh6pm.h"
//PB0-PB3 for data(4bit mode) and PA2-PA3 for control 

/*Connetction 
PA2-------E
PA3-------RS
PB0-------DB4
PB1-------DB5
PB2-------DB6
PB3-------DB7
*/
void LCD_Port_Init(void){
	//Init for PortB
	SYSCTL_RCGC2_R |= 0x00000002;     // PortB clock

  GPIO_PORTB_CR_R |= 0x0F;           // allow changes to PB0-PB3
  GPIO_PORTB_AMSEL_R &= 0x00;        //  disable analog mode select function
  GPIO_PORTB_PCTL_R &= 0x00000000;   //  GPIO clear bit PCTL (port Control Register)
  GPIO_PORTB_DIR_R |= 0x0F;          //  PB0-PB3 as OUTPUTS
  GPIO_PORTB_AFSEL_R &= 0x00;        //  no alternate function Alternate function select 
  GPIO_PORTB_DEN_R |= 0x0F;          //  enable digital pins to PB0-PB3

	//Init for PortA2-3
  SYSCTL_RCGC2_R |= 0x00000001;     // port A clock
  GPIO_PORTA_CR_R |= 0x0C;           // allow changes to PA2-3
  GPIO_PORTA_AMSEL_R &= 0x00;        //  disable analog function
  GPIO_PORTA_PCTL_R &= 0x00000000;   // GPIO clear bit PCTL
  GPIO_PORTA_DIR_R |= 0x0C;          // PA2-3 OUTPUTS
  GPIO_PORTA_AFSEL_R &= 0x00;        // no alternate function
  GPIO_PORTA_DEN_R |= 0x0C;          // enable digital pins PA2-3
}
void lcdENPulse(unsigned long time){ //set Enable high for x time
	lcdEN |= 0x04;
	Delay_Us(time);
	lcdEN &=~ 0x04; //return en to 0
}
void LCD_Init(void) {
	Delay_Init(); //init time functions
	lcdEN =0x00; //set enable to 0
	lcdDB =0x00; //set data to 0
	Delay_ms(50); //50 ms wait
	lcdRS=0x00<<2; //RS to 0
	lcdDB =0x3; //waking up instructions
	lcdENPulse(4); //latch enable line 
	Delay_ms(50);
	lcdDB =0x3;
	lcdENPulse(4);
	Delay_ms(2); //designated times to wait from datasheet
	lcdENPulse(4);
	Delay_ms(2);
	lcdDB =0x2;
  lcdENPulse(4);
	Delay_ms(2);
} 


void lcd_Command(unsigned char c){     // function for setting commands

	lcdRS =0x00;                 //line low
	Delay_Us(400);
	lcdDB=(c&0xF0)>>4;  //send first high nible to PB0-3
	lcdENPulse(4);
	Delay_ms(50); //50ms delay
	lcdDB=(c&0x0F);//set low nible
	lcdENPulse(4);
	Delay_ms(50); //50ms delay
}
void LCD_Data (char c){   //function for setting data
	lcdRS =0x08;                   //RS on for sending data
	Delay_Us(400);

	lcdDB=(c&0xF0)>>4; //high nibble
	lcdENPulse(4);

	Delay_Us(2000); //50ms delay

	lcdDB=(c&0x0F); //low nibble
	lcdENPulse(4);

	Delay_ms(50); //50ms delay
	lcdRS =0x00;
}
void lcdGoto(unsigned char address){
	lcd_Command(address); 
	Delay_ms(50);
}
void LCD_Command_Init(void){
	lcd_Command(0x28);
	Delay_ms(50); 
	lcd_Command(0x00); //sets lcd to 4 bit data, 2 lines, 5x8 font
	lcd_Command(0x06);
	Delay_ms(50);
	lcd_Command(0x00); //entry mode: cursor to increment and display not to shift
	lcd_Command(0x0F);
	Delay_ms(50);
	lcd_Command(0x00); //sets display pn, cursor on, blink on
	lcd_Command(0x04);
	Delay_ms(50);
	lcd_Command(0x00); //entry mode: cursor to increment and display not to shift
}
void LCD_Clear_Screen(void){
	lcd_Command(0x01); 
	Delay_ms(50); 
	lcd_Command(0x00); //clear lcd
	lcd_Command(0x02);
	Delay_ms(50); 
	lcd_Command(0x00); //"Home" LCD
}
void LCD_Write_String(char *string) {//function for writing strings 
while(*string) {
	LCD_Data(*string);
	string++;
	}
}