/* ###################################################################
**     Filename    : os_tasks.c
**     Project     : serial_echo
**     Processor   : MK64FN1M0VLL12
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-02-25, 18:10, # CodeGen: 1
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         serial_task - void serial_task(os_task_param_t task_init_data);
**
** ###################################################################*/
/*!
** @file os_tasks.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup os_tasks_module os_tasks module documentation
**  @{
*/         
/* MODULE os_tasks */

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "os_tasks.h"

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Callback    : serial_task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/

void serial_task(os_task_param_t task_init_data)
{

	// Print Message to console
	char output_message[13];
	sprintf(output_message, "\n\rType Test: ");
	UART_DRV_SendDataBlocking(myUART_IDX, (uint8_t*)output_message, sizeof(output_message), 1000);


	// Init Rx message queue
	_queue_id rx_queue_id = _msgq_open(RX_QUEUE_ID, 0);

	if (rx_queue_id == 0) {
	 printf("\nCould not open the server message queue\n");
	 _task_block();
	}

	RX_MESSAGE_POOL_ID = _msgpool_create(sizeof(SERVER_MESSAGE), RX_QUEUE_SIZE, 0, 0);

	if (RX_MESSAGE_POOL_ID == MSGPOOL_NULL_POOL_ID) {
	 printf("\nCount not create a message pool\n");
	 _task_block();
	}

	int size = 1000;
	char megaBuffer[size];
	char clearbuffer[] = "                                                              ";
	int current_position = 0;
	int last_space = 0;

#ifdef PEX_USE_RTOS
  while (1) {
#endif

	  if (_msgq_get_count(rx_queue_id) == 0) {
	  		  continue;
	  }


	  //Get data from the rx queue
	  SERVER_MESSAGE_PTR msg_ptr = _msgq_receive(rx_queue_id, 0);
	  char c =  msg_ptr->DATA;
	  _msg_free(msg_ptr);

	  if (c == 0x17) { //erase line
		  current_position = 0;
		  megaBuffer[current_position] = 0;

	  }else if (c == 0x20) { //space
		  last_space = current_position + 1;

	  }else if (c == 0x15) { // erase word
		  current_position = last_space;
		  megaBuffer[current_position] = 0;

	  }else if (c == 0x08) { //erase char
		  megaBuffer[current_position] = 0x20;
		  current_position--;

	  }else{

	  //increment current megaBuffers index
	  current_position++;

	  //Add char to megaBuffer
	  megaBuffer[current_position] = c;
	  //timeDelay
	  OSA_TimeDelay(10);
	  }

	  //Print Mega buffer mega
	  UART_DRV_SendDataBlocking(myUART_IDX, (uint8_t*)megaBuffer, sizeof(megaBuffer), 1000);

    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/* END os_tasks */



#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
