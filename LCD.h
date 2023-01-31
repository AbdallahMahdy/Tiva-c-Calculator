#ifndef LCD_H_
#define LCD_H_

void LCD_Port_Init(void);
void lcdENPulse(unsigned long);
void LCD_Init(void);
void lcd_Command(unsigned char);
void LCD_Command_Init(void);
void LCD_Clear_Screen(void);
void LCD_Data(char);
void LCD_Write_String( char*);
void lcdGoto(unsigned char );
	
#endif
