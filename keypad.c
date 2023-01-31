#include "tm4c123gh6pm.h"



/*
Connection
PD0-----C0
PD1-----C1
PD2-----C2
PD3-----C3

PE0-----R0
PE1-----R1
PE2-----R2
PE3-----R3
*/


void Keypad_Init(void){
	//Initalise port D (output,columns) 0-3
	SYSCTL_RCGC2_R |= 0x00000008;                
  GPIO_PORTD_LOCK_R = 0x4C4F434B;   
  GPIO_PORTD_CR_R |= 0x0F;                
  GPIO_PORTD_AMSEL_R &= 0x00;        
  GPIO_PORTD_PCTL_R &= 0x00000000;   
  GPIO_PORTD_DIR_R |= 0x0F;         
  GPIO_PORTD_AFSEL_R &= 0x00;        
  GPIO_PORTD_DEN_R |= 0x0F;             
	//Initalise  port E (input,rows) 0-3
  SYSCTL_RCGC2_R |= 0x00000010;                   
  GPIO_PORTE_LOCK_R = 0x4C4F434B;   
  GPIO_PORTE_CR_R |= 0x0F;                 
  GPIO_PORTE_AMSEL_R &= 0x00;        
  GPIO_PORTE_PCTL_R &= 0x00000000;     
  GPIO_PORTE_DIR_R &= ~0x0F;          	
  GPIO_PORTE_AFSEL_R &= 0x00;        
  GPIO_PORTE_PDR_R |= 0x0F;           
  GPIO_PORTE_DEN_R |= 0x0F;               
}  
char Keypad_Read(){
	const  char keymap[4][4] = {
        { '1', '2', '3', 'A'},
        { '4', '5', '6', 'B'},
        { '7', '8', '9', 'C'},
        { '*', '0', '#', 'D'},
    };    
	 GPIO_PORTD_DATA_R=0x02;
	 if(GPIO_PORTE_DATA_R==0x01)
		{return keymap[0][1];} //Read 1st row
	 else if(GPIO_PORTE_DATA_R==0x02)
		{return keymap[1][1];} //Read 2nd row
	 else if(GPIO_PORTE_DATA_R==0x04)
		{return keymap[2][1];} //Read 3rd row
	 else if (GPIO_PORTE_DATA_R==0x08)
		{return keymap[3][1];} //Read 4th row
		GPIO_PORTD_DATA_R=0x01;
	  if(GPIO_PORTE_DATA_R==0x01)
		{return keymap[0][0];} 
		else if(GPIO_PORTE_DATA_R==0x02)
		{return keymap[1][0];}
    else if(GPIO_PORTE_DATA_R==0x04)
		{return keymap[2][0];} 
		else if (GPIO_PORTE_DATA_R==0x08)
		{return keymap[3][0];}
	 GPIO_PORTD_DATA_R=0x04;
	 if(GPIO_PORTE_DATA_R==0x01)
	 {return keymap[0][2];}	
	 else if(GPIO_PORTE_DATA_R==0x02)
	 {return keymap[1][2];}
	 else if(GPIO_PORTE_DATA_R==0x04)
	 {return keymap[2][2];} 
	 else if(GPIO_PORTE_DATA_R==0x08) 
	{return keymap[3][2];} 
	
   GPIO_PORTD_DATA_R=0x08;
	 if(GPIO_PORTE_DATA_R==0x01)
	 {return keymap[0][3];} 
	 else if(GPIO_PORTE_DATA_R==0x02)
	  {return keymap[1][3];}
	 else if(GPIO_PORTE_DATA_R==0x04)
		{return keymap[2][3];} 
	 else if(GPIO_PORTE_DATA_R==0x08)
			{return keymap[3][3];}
	 else{return ' ';}	// return a default value if there is nothing pressed		 
}
