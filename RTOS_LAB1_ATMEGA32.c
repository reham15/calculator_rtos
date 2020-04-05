/*
 * calculator_rtos
 *
 * Created: 5/4/2020 
 *  Author: reham nader
 */ 

#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
//#include "FreeRTOSConfig.h"
#include"lcd_4bit.h"
#define F_CPU 8000000
#include <util/delay.h>

//task definitions
void task_welcome25ms(void*PTR);
void task2250ms(void*PTR);
void taskled25ms(void*PTR);

//function definitions
void led(void);
void lcd_display_welcome(void);
void lcd_display2(void);
//global variable

int x=0;
int counter=0;

int main(void)
{ lcd_init();
	DDRB=0XFF;
	DDRA=0XFF;
	 OCR0=255;
	
	xTaskCreate(task_welcome25ms,NULL,100,NULL,3,NULL);
	xTaskCreate(task2250ms,NULL,100,NULL,2,NULL);
	xTaskCreate(taskled25ms,NULL,100,NULL,1,NULL);
	
        vTaskStartScheduler();
	
	return 0;
    
}
//tasks********************************************
void task_welcome25ms(void*PTR)
{ 
	portTickType xLastWakeTime;
	const portTickType xPeriod = 25;

	xLastWakeTime = xTaskGetTickCount();
	while(counter<3)
	{
		vTaskDelayUntil( &xLastWakeTime, xPeriod );
		lcd_display_welcome();
		
	}
	
	vTaskDelete(NULL);
}
//------------------------------------------------------------------------
void task2250ms(void*PTR)
{ 
	portTickType xLastWakeTime;
	const portTickType xPeriod = 250;
int y=0;
	xLastWakeTime = xTaskGetTickCount();
	 y=xLastWakeTime +10000 ;
	while(xLastWakeTime <y)
	{   
		vTaskDelayUntil( &xLastWakeTime, xPeriod );
		lcd_display2();
		
	}
	x=1; //set flag to start blink led
	vTaskDelete(NULL);
}
//-----------------------------------------------------------------------
void taskled25ms(void*PTR)
{
portTickType xLastWakeTime;
	const portTickType xPeriod = 25;

	xLastWakeTime = xTaskGetTickCount();
	TCCR0 |=(1<<WGM00)|(1<<WGM01)|(1<<COM00)|(1<<COM01); //initialization for pwm
	TCNT0=(unsigned char )0;
	TCCR0|=(1<<CS00);
	while(1)
	{   
		vTaskDelayUntil( &xLastWakeTime, xPeriod );
		led();
		
	}
	vTaskDelete(NULL);	
}
//-----------------------------------------------------------
//functions
//****************************************************************
void led(void)
{  if(x==1) //screen blinked 10sec
    {static int  led_count=0;
	static state_led=0;
	switch(state_led)
{case 0: //rising
if(led_count==10)
state_led=1;
else
{led_count++;
OCR0=OCR0+25;}
    break;
	
case 1: // high
if(led_count==20)
{state_led=2;
OCR0=255;}
else
{led_count++;
PORTB |= 0x08;}
  break;
  
  case 2://falling
if(led_count==30)
state_led=3;
else
{led_count++;
OCR0=OCR0-25;}
  break;
  
  case 3: //low
if(led_count==40)
state_led=0;
else
{led_count++;
	PORTB &= 0xF7;}
  break;
  	
}}
else
{
	//do no thing "led is off
}

}
//--------------------------------------------------------






void lcd_display_welcome (void)
{
	 static int flag1=0;
	 
	static int x1=0; //position of welcome
	static int x2=0; //space position
//lcd_clrScreen();
lcd_disp_string_xy("   ",0,x2);
	lcd_disp_string_xy("welcome",0,x1);
	
	
     if(x1==0)
	    {flag1=0;
			counter++; //count cycle of welcome
		 }
	  else if(x1==9)
	   {
		   flag1=1;
		   
	   }
	    if(flag1==0)
        {		x1++;	 	
			x2=x1-1;
		}
    else if(flag1==1)
    {		x1--;	 	
			x2=x1+7;
		}
    
}
//------------------------------------------------------------------------------		
void lcd_display2 (void)
{
	 static int count2=0; //count num of tick of periodic task
	 if(counter>2) // check 3 cycle of welcome
	     {
             if(count2<2)
             { lcd_clrScreen();
              lcd_dispString("press any key");
	            count2++;
	               }
            else if(count2=2)
             {
	          lcd_clrScreen();
	           count2=0;
                         }
		 }						  
     else if(counter<2)
       {
          //no task 	
                     }}
	
                    
	
//---------------------------------------------------------
