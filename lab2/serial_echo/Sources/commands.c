/*
 * commands.c
 *
 *  Created on: Feb 28, 2018
 *      Author: asinha
 */
#include "Cpu.h"
#include "rtos_main_task.h"
#include "os_tasks.h"
#include "mutex.h"

#include "commands.h"

MUTEX_STRUCT openr_mutex;
MUTEX_STRUCT readline_mutex;

_queue_id openr(uint16_t queue_id) {
	// Init Rx message queue

		if (_mutex_lock(&openr_mutex) != MQX_EOK) {
			printf("Couldn't Access Mutex in OpenR from: Task %d", _task_get_id());
		}

		_queue_id read_queue_id = _msgq_open(READ_QUEUE_HANDLER_ID, 0);

		if (read_queue_id == 0) {
		 printf("\nCould not open the server message queue\n");
		 _task_block();
		}

		OPENR_REQUEST_PTR msg_ptr = (OPENR_REQUEST_PTR) _msg_alloc(RX_MESSAGE_POOL_ID);

	    if (msg_ptr == NULL) {
	 	   printf("\nCould not allocate a message\n");
	 	   _task_block();
	    }

	    msg_ptr->HEADER.SOURCE_QID = read_queue_id;
	    msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, READ_QUEUE_ID);

	    msg_ptr->HEADER.SIZE = sizeof(OPENR_REQUEST);
	    msg_ptr->queue_id = queue_id;
	    msg_ptr->task_id = _task_get_id();
	    msg_ptr->exists = FALSE;

	    _msgq_send(msg_ptr);
	    if (_task_get_error() != MQX_OK) {
	 	   printf("\nCould not send a message\n");
	 	   return;
	    }

	    msg_ptr = _msgq_receive(read_queue_id, 0);
	      if (msg_ptr == NULL) {
	         printf("\nCould not receive a message\n");
	         _task_block();
	      }

	      _queue_id returnvalue = msg_ptr->exists;
	      _msg_free(msg_ptr);

	      _mutex_unlock(&openr_mutex);
	      return returnvalue;
}

int getline(char *c) {

}




