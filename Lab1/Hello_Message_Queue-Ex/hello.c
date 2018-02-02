#ifndef __server_h__
#define __server_h__

#include <mqx.h>
#include <message.h>

/* Number of clients */
#define FP_SYSTEM_QUEUE_BASE   2

/* Queue IDs */
#define SERVER_QUEUE      8
#define CLIENT_QUEUE_BASE 9

/* This structure contains a data field and a message header structure */
typedef struct server_message
{
   MESSAGE_HEADER_STRUCT   HEADER;
   unsigned char DATA[5];
} SERVER_MESSAGE, * SERVER_MESSAGE_PTR;

_pool_id message_pool;

#endif


#include <stdio.h>
#include <bsp.h>

/* Task IDs */
#define HELLO_TASK  5
#define WORLD_TASK  6

extern void hello_task(uint32_t);
extern void world_task(uint32_t);


const TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   /* Task Index,   Function,   Stack,  Priority, Name,     Attributes,          Param, Time Slice */
	{ HELLO_TASK,   hello_task, 700,   9,        "world",  MQX_AUTO_START_TASK,  0,     0 },
	{ WORLD_TASK,   world_task, 700,   8,        "hello",  0, 					 0,     0 },
    { 0 }
};

/*TASK*-----------------------------------------------------
*
* Task Name    : world_task
* Comments     :
*    This task creates hello_task and then prints " World ".
*
*END*-----------------------------------------------------*/

void world_task
   (
      uint32_t index
   )
{
   SERVER_MESSAGE_PTR msg_ptr;
   _queue_id          client_qid;
   bool            result;

   client_qid  = _msgq_open((_queue_number)(CLIENT_QUEUE_BASE + index), 0);

   if (client_qid == 0) {
	 printf("\nCould not open a client message queue\n");
	 _task_block();
   }

   /*allocate a message*/
   msg_ptr = (SERVER_MESSAGE_PTR)_msg_alloc(message_pool);

   if (msg_ptr == NULL) {
	   printf("\nCould not allocate a message\n");
	   _task_block();
   }

   msg_ptr->HEADER.SOURCE_QID = client_qid;
   msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, SERVER_QUEUE);
   msg_ptr->HEADER.SIZE = sizeof(MESSAGE_HEADER_STRUCT) +
		   strlen((char *)msg_ptr->DATA) + 1;

   msg_ptr->DATA[0] = 'W';
   msg_ptr->DATA[1] = 'o';
   msg_ptr->DATA[2] = 'r';
   msg_ptr->DATA[3] = 'l';
   msg_ptr->DATA[4] = 'd';
   msg_ptr->DATA[5] = 0;

   result = _msgq_send(msg_ptr);

   if (result != TRUE) {
	   printf("\nCould not send a message\n");
	   _task_block();
   }

   /* wait for a return message */
   msg_ptr = _msgq_receive(client_qid, 0);

   if (msg_ptr == NULL) {
	   printf("\nCould not receive a message\n");
	   _task_block();
   }

   /* free the message */
   printf("Message Recieved: %s\n", msg_ptr->DATA);
   _msg_free(msg_ptr);

   // end server business

   _task_block();
}

void hello_task
   (
      uint32_t initial_data
   )
{

   printf("\n Hello\n");

   SERVER_MESSAGE_PTR msg_ptr;
  _queue_id          server_qid;
  bool            result;
  _task_id           task_id;

  /* open a message queue */
  server_qid = _msgq_open(SERVER_QUEUE, 0);

  if (server_qid == 0) {
	 printf("\nCould not open the server message queue\n");
	 _task_block();
  }

  /* create a message pool */
  message_pool = _msgpool_create(sizeof(SERVER_MESSAGE), 1, 0, 0);

  if (message_pool == MSGPOOL_NULL_POOL_ID) {
	 printf("\nCount not create a message pool\n");
	 _task_block();
  }

  // create client tasks
  task_id = _task_create(0, WORLD_TASK, (uint32_t)0);

  if (task_id == 0) {
	  printf("\nCould not create a world task\n");
	  _task_block();
  }

 msg_ptr = _msgq_receive(server_qid, 0);

 if (msg_ptr == NULL) {
	printf("\nCould not receive a message\n");
	_task_block();
 }

 printf(" %s\n", msg_ptr->DATA);

 /* return the message */
 msg_ptr->HEADER.TARGET_QID = msg_ptr->HEADER.SOURCE_QID;
 msg_ptr->HEADER.SOURCE_QID = server_qid;

 result = _msgq_send(msg_ptr);

 if (result != TRUE) {
	printf("\nCould not send a message\n");
	_task_block();
 }


   _task_block();
}

/* EOF */
