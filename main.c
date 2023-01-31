/*
This project is a Calculator implemented using tm4c123gh6pm MicroController and LCD 16*2 and a keypad 
it is done by Eng:Abdullah Alomran..
All the conections are included in each hardware file Note"Commented".
*/
// libraries decleration
#include <stdio.h>   
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "keypad.h" 
#include "LCD.h"
#include "delay.h"

#define PASS 2021

//Global Variables 
float First_op=0,Second_op=0,Third_op=0,Fourth_op=0,Answer=0; //variables for the two operands and the Answer.
int Pass_Check=0; //if 1 the password has been Entered correctly if 0 it has not.
int Count=0; //Count to check that there is an opereator already entered
int cursor=0; //stores the current cursor address of LCD
int S_flag=0 ,dotflag=0; //ahift flag to extend the operations of the keypad 
int operation[20]; // string that stores the operators for each operationition in the operation (each point of the string) in values from 1 (sum) to 4 (division)
int PW=0; //password variable used for comparison of the input password with PASSWORD 
char d; //variable to get the characters for the LCD
char pass_verify[20], pass_in[20], store_pass[20]; //arrays used for the password
char key, in,key_new; //characters to save the data from keypad
char final[20]; //used for printing the final Answerwer.
char first_op_final[20], seconed_op_final[20],third_op_final[20],fourth_op_final[20]; //Final string values
int x=0,y=0;

//Function prototypes
void Initialize(void); //System initialisation functions
float Calculate(char); //main calculations function
char Rec_Number(char); //return numbers for calculation puroperatione
int password(char); // password function
char Rec_Char(char);//function to recognize the input characters 

//main function
int main(void){
	
	Initialize(); //Initialise LCD & Keypad
	
	LCD_Write_String("ENTER PASSWORD"); //Show Msg "Enter password"
	Delay_ms(4000); 
	LCD_Clear_Screen(); //Clear screen
	
while(Pass_Check==0){ //While the correct password hasn't been entered
	
		key=Keypad_Read(); //input
		Delay_ms(1000); //switch debounce
		key_new=Keypad_Read(); //compare the input 
	
		 if (key !=' ' && (key!=key_new)) { //if the last two inputs are different to each other and it is not the default o/p of the keypad function 
				
			 if(key!='*'){ //and if input different to equal
				 
					password(key); //returns password inputs as int
					LCD_Data('*');//print the keypad input to the LCD as *		 
    }
			 else{ //If equals is pressed
				 
			in='='; //then the input must be '='
			sprintf(pass_in,"%d",password(key)); //convert the float to char using special function sprintf
			LCD_Clear_Screen(); //Wipe the screen of all numbers
				 
			if(PW!=PASS){ //If the input password doesn't match 
			LCD_Write_String("WRONG PASSWORD"); 
				Delay_ms(2500);
				LCD_Clear_Screen();
				PW=0; 
 }
	  }
		 }
		 if(PW==PASS){ //If the input password matches the actual one 
			 Pass_Check=1;
			 break;  //break from the while loop 
		 }  
}

while(Pass_Check==1){ //If we have broken out of the previous while loop
	
		key=Keypad_Read(); //input
		Delay_ms(1000); //switch debounce
		key_new=Keypad_Read(); //input compare 
	
		 if (key!=' ' && (key!=key_new)) { //if the last two inputs are different to each other and it is not the default o/p of the keypad function
			 
			 cursor++; //increase the cursor address
			 if(cursor>16){ //if we have exceed the first row 
			 lcdGoto(cursor+151); //move the cursor to the first position of the second row
			 }
			 
			 if(key!='*'){ //if the keypadinput is different to equal sign 	
				 if(Rec_Char(key)!=' '){
					LCD_Data(Rec_Char(key));//print the keypad input to the LCD
				 }
				 Calculate(key); //perform the calculation made from the keypad inputs
				 
															} 
			 
		else{ //if we pressed * 
			in='='; //otherwise set input to equals sign	
			sprintf(final,"%0.2f",Calculate(key));
			cursor=strlen(final); //look for the new position of the cursor after calculation is performed
			LCD_Clear_Screen();	
			if(Answer!=0){
				LCD_Write_String(final); //print computation answerwer to LCD
	  }
			else{LCD_Clear_Screen(); } 
	 }
	}
 }
}

//used functions in the program 
char Rec_Char(char x){ //takes a reading from the keypad in a,return the corresponding value 
    if(x == '0') {return x;}
    if(x == '1') {return x;} 
    if(x == '2') {return x;}
    if(x == '3') {return x;}
    if(x == '4') {return x;}
    if(x == '5') {return x;}
    if(x == '6') {return x;}
    if(x == '7') {return x;}
    if(x == '8') {return x;}
    if(x == '9') {return x;}
		if(x=='*')	{return '=';}
		if(S_flag==0)
		{
			if (x=='A') {return '+';}
			if (x=='B') {return '-';}
			
			if (x=='C') {if(dotflag==0)
										{return '.';}
									}
		}
		if(S_flag==1)
		{
			if (x=='A') {return 'x';}
			if (x=='B') {return '/';}
			if (x=='C') {return 'E';}
		}
		return ' ';
		
}


char Rec_Number(char a){ //takes a reading from the keypad in a return it to function
    if(a == '0') {return a;}
    if(a == '1') {return a;}
    if(a == '2') {return a;} 
    if(a == '3') {return a;}
    if(a == '4') {return a;}
    if(a == '5') {return a;}
    if(a == '6') {return a;}
    if(a == '7') {return a;}
    if(a == '8') {return a;}
    if(a == '9') {return a;}
		if(a == '.') {return a;} 
		return ' ';
}



float Calculate(char in) {	
	if (Count==0 ){                                             //if none operator has been writen means that it is the first operand
		if(sizeof(first_op_final)==0) 		//reset the dot point flag 
			{
			dotflag=0;
			}
		char w = Rec_Number(in); //save the first number 
			if(w!=' '){
		strcat(first_op_final,&w); //concatenate the number to get the final first number value 
			}
	}
	 if (Count==1){ //if the count =1 we are working with 2nd operand
		 if(sizeof(seconed_op_final)==0){
			dotflag=0; 			//reset the dot flag 
			}
		char w = Rec_Number(in); 
			if(w!=' '){
		strcat(seconed_op_final,&w); //concatenate the number to get the 2nd number final value 
			}
	}
	 if (Count==2){
		 if(sizeof(third_op_final)==0){
			dotflag=0; 			//reset the dot flag 
			}
		char w = Rec_Number(in); 
			if(w!=' '){
		strcat(third_op_final,&w); //concatenate the number to get the 3rd number final value 
			}
	 }
	 if (Count==3){
		 if(sizeof(fourth_op_final)==0){
			dotflag=0; 			//reset the dot flag 
			}
		char w = Rec_Number(in); 
			if(w!=' '){
		strcat(fourth_op_final,&w); //concatenate the number to get the 3rd number final value 
			}
	 }
	 if(S_flag==0){//check the shift flag
    if (in == 'A') {operation[Count]=1;Count++;}//addition // every time you give an operator, the Count is incremented 
    if (in == 'B') {operation[Count]=2;Count++;} //subtraction //the operation array stores a value indicates the entered operator
    if (in == 'C') // if a dot is pressed
			{
			if(dotflag==0){
			Calculate('.'); dotflag=1;} 
			}
		if (in=='D')   {S_flag=1;} //fires the shift flag
		if(in=='#'){ //rubout 
         if(Count==0 && sizeof(first_op_final)>0){  //check if it is in the first operand
					 first_op_final[strlen(first_op_final)-1] = '\0';	//clear the last charcter from string
						cursor=cursor-1;
					 LCD_Data(' ');//clear the screen
					 cursor=cursor-1;
					}
				 else if(Count==1 && sizeof(seconed_op_final)==0){ //check if it is the operation sign
					 Count=Count-1;
					 operation[Count]=0;
					 cursor=cursor-1;
					 LCD_Data(' ');
					 cursor=cursor-1;
				 }
				 else if(Count==1 && sizeof(seconed_op_final)>0)//if  it is the 2nd operand 
				 {
					 
					 seconed_op_final[strlen(seconed_op_final)-1] = '\0';
					 cursor=cursor-1;
					 LCD_Data(' ');
					 cursor=cursor-1;
				 }
				 else if(Count==2 && sizeof(third_op_final)==0){ //check if it is the operation sign
					 Count=Count-1;
					 operation[Count]=0;
					 cursor=cursor-1;
					 LCD_Data(' ');
					 cursor=cursor-1;
				 }
				 else if(Count==2 && sizeof(third_op_final)>0)//if  it is the 2nd operand 
				 {
					 
					 third_op_final[strlen(third_op_final)-1] = '\0';
					 cursor=cursor-1;
					 LCD_Data(' ');
					 cursor=cursor-1;
				 }
				 else if(Count==3 && sizeof(fourth_op_final)==0){ //check if it is the operation sign
					 Count=Count-1;
					 operation[Count]=0;
					 cursor=cursor-1;
					 LCD_Data(' ');
					 cursor=cursor-1;
				 }
				 else if(Count==3 && sizeof(fourth_op_final)>0)//if  it is the 2nd operand 
				 {
					 
					 fourth_op_final[strlen(fourth_op_final)-1] = '\0';
					 cursor=cursor-1;
					 LCD_Data(' ');
					 cursor=cursor-1;
				 }
			 }
		
	}
						
			if(S_flag==1){ //if shift flag is fired 
			if(in=='A'){  //multiplication
					S_flag=0; //reset the flag 
					operation[Count]=3;//set the operation to multiplication
					Count++; //an operation is selected
			}				
				if(in=='B'){ //division  
				S_flag=0; 
					operation[Count]=4;
					Count++;
			}
				if(in=='C'){ // E operation 1st number times 10 to the power of 2nd number
					operation[Count]=5;
					Count++;
			}			
				if(in=='D'){ //If shift is pressed twice 
					
					S_flag=0;
					
			}	
				if(in=='#'){ // reset all the variables & clear screen
				LCD_Clear_Screen();
				memset(first_op_final,0,sizeof(first_op_final));
				memset(seconed_op_final,0,sizeof(seconed_op_final));
				memset(third_op_final,0,sizeof(third_op_final));		
				memset(fourth_op_final,0,sizeof(fourth_op_final));
				First_op=0; Second_op=0; Third_op=0; Fourth_op=0; Count=0;operation[0]=0;Answer=0;	
		}
				
			}
		

    if(in=='*' | in=='=') { //If equal sign is pressed
			if(operation[0]!=0 && operation[1]==0){  //if there is only one operation
				First_op=atof(first_op_final); //convert string value to float with special function atof
				Second_op=atof(seconed_op_final); 
				memset(first_op_final,0,sizeof(first_op_final)); 	//clear the strings for the next operations
				memset(seconed_op_final,0,sizeof(seconed_op_final)); 
				if(operation[0]==1) //perform calculations depending on the selected operation
					{Answer=First_op+Second_op;} 
				if(operation[0]==2)
					{Answer=First_op-Second_op;}	
				if(operation[0]==3)
					{Answer=First_op*Second_op;} 
				if(operation[0]==4)
					{Answer=First_op/Second_op;}
				if(operation[0]==5)
					{Answer=First_op*(pow(10,Second_op));}			
					sprintf(first_op_final,"%0.2f",Answer); //store the Answer to first variable so we can use it again
					//reset paramenters 
				Second_op=0;First_op=Answer;Count=0;operation[0]=0;operation[1]=0;		
			}		
				if(operation[1]!=0 && operation[2]==0){ //same for the rest
				
				First_op=atof(first_op_final); Second_op=atof(seconed_op_final);Third_op=atof(third_op_final);
				
				memset(first_op_final,0,sizeof(first_op_final));
				memset(seconed_op_final,0,sizeof(seconed_op_final));
				memset(third_op_final,0,sizeof(third_op_final));
				
				if(operation[0]==1)   
					{y=First_op+Second_op;} 
				if(operation[0]==2)
					{y=First_op-Second_op;}	
				if(operation[0]==3)
					{y=First_op*Second_op;} 
				if(operation[0]==4){y=First_op/Second_op;}
				if(operation[0]==5)
					{y=First_op*(pow(10,Second_op));}	
				if(operation[1]==1)
					{Answer=y+Third_op;} 
				if(operation[1]==2)
					{Answer=y-Third_op;}	
				if(operation[1]==3)
					{Answer=y*Third_op;} 
				if(operation[1]==4)
					{Answer=y/Third_op;}
				if(operation[1]==5)
					{Answer=y*(pow(10,Third_op));}	
				
			 sprintf(first_op_final,"%0.2f",Answer);
				
			 Second_op=0;Third_op=0;First_op=Answer;Count=0;operation[0]=0;operation[1]=0;	
			}
				
			if(operation[2]!=0 && operation[3]==0){
				
				First_op=atof(first_op_final); Second_op=atof(seconed_op_final);Third_op=atof(third_op_final);Fourth_op=atof(fourth_op_final);
				
				memset(first_op_final,0,sizeof(first_op_final));
				memset(seconed_op_final,0,sizeof(seconed_op_final));
				memset(third_op_final,0,sizeof(third_op_final));
				memset(fourth_op_final,0,sizeof(fourth_op_final));
								
		 if(operation[0]==1)
			{y=First_op+Second_op;} 
		 if(operation[0]==2)
			{y=First_op-Second_op;}	
		 if(operation[0]==3)
			{y=First_op*Second_op;} 
		 if(operation[0]==4)
			{y=First_op/Second_op;}	
		 if(operation[0]==5)
			{y=First_op*(pow(10,Second_op));}	
		 if(operation[1]==1)
			{x=y+Third_op;} 
		 if(operation[1]==2)
			{x=y-Third_op;}	
		 if(operation[1]==3)
			{x=y*Third_op;} 
		 if(operation[1]==4)
			{x=y/Third_op;}
		 if(operation[1]==5)
			{x=y*(pow(10,Third_op));}	
		 if(operation[2]==1)
			{Answer=x+Fourth_op;} 
		 if(operation[2]==2)
			{Answer=x-Fourth_op;}	
		 if(operation[2]==3)
			{Answer=x*Fourth_op;}
		 if(operation[2]==4)
			{Answer=x/Fourth_op;}
		 if(operation[2]==5)
			{Answer=x*(pow(10,Fourth_op));}	
				
				 sprintf(first_op_final,"%0.2f",Answer);
				
				 Second_op=0;Third_op=0;Fourth_op=0;First_op=Answer;Count=0;operation[0]=0;operation[1]=0;operation[2]=0;	
			}
			return Answer;
		}
		Answer=0;
		 return Answer; 
}		


int password(char in) { //concatenates the inputs of keypad into a string, and it is converted to int before it's returned
	
    if(in == '0') 
				{
					strcat(pass_in,"0"); 
					strcpy(pass_verify,pass_in);
				} 
    if(in == '1') 
				{
					strcat(pass_in,"1");
				strcpy(pass_verify,pass_in);
				} 
    if(in == '2') 
				{
					strcat(pass_in,"2");
					strcpy(pass_verify,pass_in);
				} 
    if(in == '3') 
				{
					strcat(pass_in,"3");
					strcpy(pass_verify,pass_in);
				} 
    if(in == '4')
				{
					strcat(pass_in,"4");
					strcpy(pass_verify,pass_in);
				} 
    if(in == '5') 
				{
					strcat(pass_in,"5");
					strcpy(pass_verify,pass_in);
				}
    if(in == '6') 
				{
					strcat(pass_in,"6");
					strcpy(pass_verify,pass_in);
				}
    if(in == '7')
				{
					strcat(pass_in,"7");
					strcpy(pass_verify,pass_in);
				}
    if(in == '8') 
				{
					strcat(pass_in,"8");
					strcpy(pass_verify,pass_in);
				}
    if(in == '9') 
				{
					strcat(pass_in,"9");
					strcpy(pass_verify,pass_in);
				}
		if(in == '.') 
				{
					strcat(pass_in,".");
					strcpy(pass_verify,pass_in);
				}
		
if (in == '*' | in=='=') 
	{
		PW=atoi(pass_verify);
	  memset(pass_verify,0,sizeof(pass_verify));
		memset(pass_in,0,sizeof(pass_in)); //flush all the arrays, which when an equal sign is pressed 
	}
	return PW;
}

void Initialize(void)
{ //initialize the system
	LCD_Port_Init();
	 LCD_Init(); 
	LCD_Command_Init();
	 Keypad_Init();
}
