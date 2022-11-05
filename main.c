/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "lpc21xx.h"
/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"

BaseType_t task_1;
BaseType_t task_2;
BaseType_t task_3;
BaseType_t task_4;
BaseType_t task_5;
BaseType_t task_6;

void task_1_code(void *pvParameters);
void task_2_code(void *pvParameters);
void task_3_code(void *pvParameters);
void task_4_code(void *pvParameters);
void task_5_code(void *pvParameters);
void task_6_code(void *pvParameters);

/*-----------------------------------------------------------*/
/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )

/* Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/
/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
QueueHandle_t Queue;
int task_1_in=0 , task_1_out=0 , task_1_total=0 ;
int task_2_in=0 , task_2_out=0 , task_2_total=0 ;
int task_3_in=0 , task_3_out=0 , task_3_total=0 ;
int task_4_in=0 , task_4_out=0 , task_4_total=0 ;
int task_5_in=0 , task_5_out=0 , task_5_total=0 ;
int task_6_in=0 , task_6_out=0 , task_6_total=0 ;
int system_time=0 ;
float	CPU_Load=0 ;
int main( void )
{
	prvSetupHardware();
  Queue = xQueueCreate(200, sizeof(char *));

    /* Create Tasks here */
	task_1 = xTaskPeriodicCreate(task_1_code,"Button_1",configMINIMAL_STACK_SIZE, ( void * ) NULL,1, NULL ,50);
  task_2 = xTaskPeriodicCreate(task_2_code,"Button_2",configMINIMAL_STACK_SIZE, ( void * ) NULL,1, NULL ,50);
  task_3 = xTaskPeriodicCreate(task_3_code,"Periodic_Transmitter",configMINIMAL_STACK_SIZE, ( void * ) NULL,1, NULL ,100);
	task_4 = xTaskPeriodicCreate(task_4_code,"Uart",configMINIMAL_STACK_SIZE, ( void * ) NULL,1, NULL ,20);
	task_5 = xTaskPeriodicCreate(task_5_code,"Load_1",configMINIMAL_STACK_SIZE, ( void * ) NULL,1, NULL ,10);
	task_6 = xTaskPeriodicCreate(task_6_code,"Load_2",configMINIMAL_STACK_SIZE, ( void * ) NULL,1, NULL ,100);
	/* Now all the tasks have been started - start the scheduler.
*/

	vTaskStartScheduler();

	
	for( ;; );
}
/*-----------------------------------------------------------*/

/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware( void )
{

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


 void vApplicationIdleHook( void )
 {
  GPIO_write(PORT_0,PIN9,PIN_IS_HIGH);
  GPIO_write(PORT_0,PIN9,PIN_IS_LOW);

 }

void task_1_code(void *pvParameters)
{
	pinState_t NewValue = 0 ;
  pinState_t OldValue = 0;
 char *RisingEdge  = "Button 1 Rising \n";
 char *FallingEdge = "Button 1 Falling\n";
	TickType_t xLastWakeTime1 = xTaskGetTickCount(); 
	vTaskSetApplicationTaskTag(NULL,(void *)1);
	for (;;)
	{
		NewValue = GPIO_read(PORT_0, PIN7);
		if ((NewValue == 1) && (OldValue == 0))
		{xQueueSend(Queue, (void *)&RisingEdge, (TickType_t)5);}
		else if (NewValue == 0 && OldValue == 1)
		{xQueueSend(Queue, (void *)&FallingEdge, (TickType_t)5);}
	  OldValue = NewValue ;
		vTaskDelayUntil(&xLastWakeTime1, 50);
	}
}
void task_2_code(void *pvParameters)
{
	pinState_t NewValue = 0 ;
  pinState_t OldValue = 0;
 char *RisingEdge  = "Button 2 Rising \n";
 char *FallingEdge = "Button 2 Falling\n";
	TickType_t xLastWakeTime2 = xTaskGetTickCount(); 
	vTaskSetApplicationTaskTag(NULL,(void *)2);
	for (;;)
	{
		NewValue = GPIO_read(PORT_0, PIN8);
		if ((NewValue == 1) && (OldValue == 0))
		{xQueueSend(Queue, (void *)&RisingEdge, (TickType_t)5);}
		else if (NewValue == 0 && OldValue == 1)
		{xQueueSend(Queue, (void *)&FallingEdge, (TickType_t)5);}
	  OldValue = NewValue ;
		vTaskDelayUntil(&xLastWakeTime2, 50);
	}
}
void task_3_code(void *pvParameters)
{

	char *periodicMessage = "100 msec Message\n";
	TickType_t xLastWakeTime3 = xTaskGetTickCount();
	vTaskSetApplicationTaskTag(NULL,(void *)3);
	for (;;)
	{xQueueSend(Queue, (void *)&periodicMessage, (TickType_t)0);
	/*	 vTaskGetRunTimeStats( bufferRuntime );
  xSerialPutChar('\n');
 vSerialPutString((const signed char *)bufferRuntime, 200);*/
	 vTaskDelayUntil(&xLastWakeTime3, 100);
	}
}
void task_4_code(void *pvParameters)
{
	char *Queue2 =NULL;
		TickType_t xLastWakeTime4 =  xTaskGetTickCount(); 
	vTaskSetApplicationTaskTag(NULL, (void *)4);
	for (;;)
	{
		if (xQueueReceive(Queue, &Queue2, (TickType_t)0) == pdTRUE)
		{vSerialPutString((const signed char *)Queue2, 20);}
		vTaskDelayUntil(&xLastWakeTime4, 20);
	}
}
void task_5_code(void *pvParameters)
 {
 TickType_t xLastWakeTime5 = xTaskGetTickCount();
vTaskSetApplicationTaskTag(NULL,(void *)5);
 while(1)
 {
 int x=0;
 while(x < 37200)   //5ms
 {x++;}
 vTaskDelayUntil( &xLastWakeTime5, 10 );
 } }
 
void task_6_code(void *pvParameters)
 {
 TickType_t xLastWakeTime6 = xTaskGetTickCount();
vTaskSetApplicationTaskTag(NULL,(void *) 6);
GPIO_read(PORT_0, PIN5);
 while(1)
 {
 int x=0;
 while(x < 89280)   //12ms
 {x++;}

  vTaskDelayUntil( &xLastWakeTime6, 100 );
 } 
 }