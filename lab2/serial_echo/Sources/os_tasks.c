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
#include "commands.h"

#ifdef __cplusplus
extern "C" {
#endif 


USER_TASK_READ readTasks[10];

void newLine() {
	  char ansi_escape[] =  {0x1B, '[', '2', 'K', '\r'};
	  UART_DRV_SendData(myUART_IDX, (uint8_t*)ansi_escape, sizeof(ansi_escape));
}

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
	sprintf(output_message, "\n\rType: \n");
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





	// GetLine Queue
	/*
	_queue_id getline_queue_id = _msgq_open(RX_QUEUE_ID, 0);

	if (rx_queue_id == 0) {
	 printf("\nCould not open the server message queue\n");
	 _task_block();
	}

	RX_MESSAGE_POOL_ID = _msgpool_create(sizeof(SERVER_MESSAGE), RX_QUEUE_SIZE, 0, 0);

	if (RX_MESSAGE_POOL_ID == MSGPOOL_NULL_POOL_ID) {
	 printf("\nCount not create a message pool\n");
	 _task_block();
	}
*/



	int size = 1000;
	char megaBuffer[size];
	int spacePosition = 0;

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

	  if (c == 0x15) { //erase line
		  current_position = 0;
		  newLine();
		  continue;
	  }

	  if (c == 0x17) { // erase word
		  spacePosition = 0;
		  int hasSpace = 0;
		  //find last space
		  for(int i = 0; i < current_position; i++){
			  if(megaBuffer[i] == ' '){
				  spacePosition = i;
				  hasSpace = 1;
			  }
		  }

		  if(hasSpace == 1){

			  //Delete a single char x amount of times
			  for(int i = 0; i < (current_position - spacePosition); i++){
				  printf("delete once\n");
				  //char ansi_escape[] =  {0x1B, '[','1','D'};
				  char ansi_escape[] =  {'\b',' ', '\b'};
				  UART_DRV_SendData(myUART_IDX, (uint8_t*)ansi_escape, sizeof(ansi_escape));
			  }

			  //delete everything to the right of the cursor
			  //char ansi_escape[] = {0x1B, '[', 'K'};
			  //UART_DRV_SendData(myUART_IDX, (uint8_t*)ansi_escape, sizeof(ansi_escape));
		  }
		  //decrement the current position
		  current_position = current_position - (current_position - spacePosition);
		  continue;
	  }

	  if (c == 0x08) { //erase char
		  current_position--;
		  char ansi_escape[] =  {'\b',' ', '\b'};
		  UART_DRV_SendData(myUART_IDX, (uint8_t*)ansi_escape, sizeof(ansi_escape));
		  continue;
	  }

	  //Add char to megaBuffer
	  megaBuffer[current_position] = c;
	  //increment current megaBuffers index
	  current_position++;
	  //Print Mega buffer - hold mega buffer but only print and use for calculations
	  //UART_DRV_SendData(myUART_IDX, (uint8_t*)megaBuffer, current_position);
	  UART_DRV_SendData(myUART_IDX, myRxBuff, sizeof(myRxBuff));
	  //OSA_TimeDelay(10);

    
#ifdef PEX_USE_RTOS   
  }
#endif    
}

/*
** ===================================================================
**     Callback    : readrequest_Task
**     Description : Task function entry.
**     Parameters  :
**       task_init_data - OS task parameter
**     Returns : Nothing
** ===================================================================
*/
void readrequest_Task(os_task_param_t task_init_data)
{
  /* Write your local variable definition here */


	// Init openR message queue
	_queue_id user_read_queue = _msgq_open(READ_QUEUE_ID, 0);

	if (user_read_queue == 0) {
	 printf("\nCould not open the read message queue\n");
	 _task_block();
	}

	READ_MESSAGE_POOL_ID = _msgpool_create(sizeof(OPENR_REQUEST), RX_QUEUE_SIZE, 0, 0);

	if (READ_MESSAGE_POOL_ID == MSGPOOL_NULL_POOL_ID) {
	 printf("\nCount not create a read message pool\n");
	 _task_block();
	}

	int count = 0;
  
#ifdef PEX_USE_RTOS
  while (1) {
#endif

    /* Write your code here ... */
    if (_msgq_get_count(user_read_queue) == 0) {
    	continue;
    }


    OPENR_REQUEST_PTR msg_ptr = _msgq_receive(user_read_queue, 0);
    USER_TASK_READ user_task_read;
    user_task_read.taskId = msg_ptr->task_id;
    user_task_read.userQueueId = msg_ptr->queue_id;

	msg_ptr->HEADER.TARGET_QID = msg_ptr->HEADER.SOURCE_QID;
	msg_ptr->HEADER.SOURCE_QID = user_read_queue;

    for (int i = 0; i < count; i++) {
    	if (readTasks[i].taskId == user_task_read.taskId) {
			msg_ptr->exists = TRUE;
    	}
    }

	if (!msg_ptr->exists) {
		readTasks[count] = user_task_read;
		count++;
	}

    int result = _msgq_send(msg_ptr);

	if (result != TRUE) {
		printf("\nCould not send a message\n");
		_task_block();
	}

    OSA_TimeDelay(10);                 /* Example code (for task release) */
    
    
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
