#ifndef __commands_H_
#define __commands_H_

#include "Cpu.h"
#include "Events.h"
#include "rtos_main_task.h"
#include "mutex.h"
#include <message.h>

#define NUM_USER_TASK 1

typedef struct user_permissions{
	_queue_id queue_id;
	_task_id task_id;
	int has_write_permissions;
	int has_read_permissions;
} USER_PERMISSIONS;

typedef struct getline_queue {
	unsigned char data[100];
	int items_ready;
} GETLINE_QUEUE;


extern GETLINE_QUEUE getline_q;
extern MUTEX_STRUCT getline_mutex;

extern USER_PERMISSIONS user_permissions[NUM_USER_TASK];
extern MUTEX_STRUCT permissions_mutex;

extern _queue_id openR(uint16_t);
extern _queue_id openW(void);
extern int _getline(char *);
extern int _putline(_queue_id, char *);
extern int Close(void);

#endif
