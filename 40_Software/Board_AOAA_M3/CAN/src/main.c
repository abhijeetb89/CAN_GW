/*
===============================================================================
 Name        : CAN.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/


#include <cr_section_macros.h>
#include "can_handler.h"
#include "FreeRTOS.h"
#include "task.h"
#include "board.h"


// TODO: insert other include files here

// TODO: insert other definitions and declarations here
#define 	STACK_TASK_SEND_VELOCITY	configMINIMAL_STACK_SIZE
#define 	STACK_TASK_RCV_CAN_MSGS		configMINIMAL_STACK_SIZE

void task_Can_Send_Velocity(void *parameters);
void task_Can_Rcv_Messages(void *parameters);

int main(void) {

    // TODO: insert code here

	portBASE_TYPE task_Create_Status=NULL;

	 SystemInit();

	trimpot_init();
	can_handler_init();

	task_Create_Status= xTaskCreate(
							task_Can_Send_Velocity,				//task function
							(signed char *)"Send_Velocity",		//task name
							STACK_TASK_SEND_VELOCITY,			//stack size
							NULL,								//parameters to be passed to task
							1,									//priority
							NULL								//task handler
	            		);

	if(!task_Create_Status)
		return 0;

	task_Create_Status= xTaskCreate(
								task_Can_Rcv_Messages,				//task function
								(signed char *)"Rcv_Msgs",		//task name
								STACK_TASK_RCV_CAN_MSGS,			//stack size
								NULL,								//parameters to be passed to task
								2,									//priority
								NULL								//task handler
		            		);

	if(!task_Create_Status)
		return 0;

	vTaskStartScheduler();


// Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        i++ ;
    }
    return 0 ;
}

/*
 * RTOS Task for sending vehicle velocity
 * Value of potentiometer on board is used as speed sensor.
 * Min value of sensor = 0		= 	0 kph
 * Max value of sensor = 4095	= 300 kph
 * The sensor value is being sent in CAN message CAN_MSG_ID_VEHICLE_SPEED with cycle time of 100mS.
 */

void task_Can_Send_Velocity(void *parameters)
{
	uint8_t uiData[2];
	uint16_t uiPotValue;

	for(;;)
	{
		uiPotValue = trimpot_get();
		uiData[0]  = uiPotValue;
		uiData[1]  = uiPotValue >> 8;

		can_handler_send(CAN_MSG_ID_VEHICLE_SPEED, CAN_MSG_DLC_VEHICLE_SPEED, uiData);
		vTaskDelay(CAN_MSG_CYCLE_TIME_VEHICLE_SPEED);
	}

}

void task_Can_Rcv_Messages(void *parameters)
{
	uint16_t id;
	uint8_t  dlc;
	uint8_t  data[8];

	uint16_t rcv_id;;

	for(;;)
	{
		can_handler_receive(&id, &dlc, data);
		rcv_id = id;
		vTaskDelay(500);
	}

}
