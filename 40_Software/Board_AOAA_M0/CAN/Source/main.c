/*
 * main.c
 *
 *  Created on: 23.05.2015
 *      Author: Abhijeet
 */

#include "FreeRTOS.h"
#include "task.h"
#include "mcu_regs.h"
#include "type.h"
#include "stdio.h"
#include "timer32.h"
#include "gpio.h"
#include "i2c.h"
#include "can_handler.h"
#include "can_database.h"

#include "rgb.h"
#include "btn.h"
#include "light.h"
#include "lm75a.h"
#include "board.h"

#include "canpt.h"

void led_toggle(void *parameters);
void can_send_Ambient_Brightness(void *parameters);
void can_send_Interior_Temperature(void *parameters);

int main(void) {

	GPIOInit();
	led_init();			//initialize LED
	I2CInit(I2CMASTER, 0);

	light_init();		//initialize light intensity sensor
	light_enable();
	light_setWidth(LIGHT_WIDTH_16BITS);
	light_setRange(LIGHT_RANGE_16000);
	lm75a_init();		//initialize temperature sensor

	//can_handler_init();	//initialize CAN
	canpt_init();

	//create task for blinking LED
	xTaskCreate(
		led_toggle, 						//function
		(const char *) "LED_Toggle",		//name of the task
		configMINIMAL_STACK_SIZE,			//stack size
		NULL,								//parameters
		0,									//priority
		NULL								//task handle
	);

	//create task for blinking LED
	xTaskCreate(
		can_send_Ambient_Brightness, 			//function
		(const char *) "can_send_brightness",  //name of the task
		configMINIMAL_STACK_SIZE,			  //stack size
		NULL,								 //parameters
		1,									//priority
		NULL							   //task handle
	);

	//create task for blinking LED
	xTaskCreate(
		can_send_Interior_Temperature, 				 //function
		(const char *) "can_send_temperature",		//name of the task
		configMINIMAL_STACK_SIZE,				   //stack size
		NULL,								      //parameters
		1,									     //priority
		NULL								    //task handle
	);

	vTaskStartScheduler();	//start scheduler

	while(1){

	}

}

void led_toggle(void *parameters) {

	for(;;)
	{
		led_set(1);
		vTaskDelay(1000);
		led_set(0);
		vTaskDelay(1000);
	}

}

void can_send_Interior_Temperature(void *parameters)
{
	uint32_t *uiTemperaure=NULL;

	for(;;)
	{
		*uiTemperaure = lm75a_readTemp();	//read the value of temperature
		can_handler_send(CAN_MSG_ID_INTERIOR_TEMPERATURE, CAN_MSG_DLC_INTERIOR_TEMPERATURE, uiTemperaure);
		vTaskDelay(1000);	//wait for 1Sec so that message will be sent at the rate of 1Sec

	}

}

void can_send_Ambient_Brightness(void *parameters)
{
	uint32_t *uiLux=NULL;

	for(;;)
	{
		*uiLux =  light_read();	//read the value of light intensity
		can_handler_send(CAN_MSG_ID_AMBIENT_BRIGHTNESS, CAN_MSG_DLC_AMBIENT_BRIGHTNESS, uiLux);
		vTaskDelay(1000);	//wait for 1Sec so that message will be sent at the rate of 1Sec

	}

}

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
#if mainCHECK_INTERRUPT_STACK == 1
extern unsigned long _pvHeapStart[];

	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */

	/* Manually check the last few bytes of the interrupt stack to check they
	have not been overwritten.  Note - the task stacks are automatically
	checked for overflow if configCHECK_FOR_STACK_OVERFLOW is set to 1 or 2
	in FreeRTOSConifg.h, but the interrupt stack is not. */
	configASSERT( memcmp( ( void * ) _pvHeapStart, ucExpectedInterruptStackValues, sizeof( ucExpectedInterruptStackValues ) ) == 0U );
#endif /* mainCHECK_INTERRUPT_STACK */
}
/*-----------------------------------------------------------*/
