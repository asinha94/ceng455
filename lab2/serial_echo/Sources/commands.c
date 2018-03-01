/*
 * commands.c
 *
 *  Created on: Feb 28, 2018
 *      Author: asinha
 */
#include "Cpu.h"
#include "rtos_main_task.h"
#include "mutex.h"

#include "commands.h"



_queue_id OpenR(uint16_t queue_id) {
	if (_mutex_lock(&permissions_mutex) != MQX_EOK) {
		printf("\r\n[%d] Couldn't Access Mutex in OpenR: %x", _task_get_id(), _task_get_error());
	}

	int access_granted = TRUE;

	for (int i = 0; i < NUM_USER_TASK; i++) {
		if (user_permissions[i].task_id != queue_id) {
			continue;
		}

		if (user_permissions[i].has_read_permissions) {
			access_granted = FALSE;
		} else {
			user_permissions[i].has_read_permissions = TRUE;
		}

		break;
	}

	_mutex_unlock(&permissions_mutex);
	return access_granted;
}

int _getline(char *c) {

	if (_mutex_lock(&permissions_mutex) != MQX_EOK) {
		printf("\r\n[%d] Couldn't Access Mutex in getline", _task_get_id());
	}

	for (int i = 0; i < NUM_USER_TASK; i++) {
		if (user_permissions[i].task_id != _task_get_id()) {
			continue;
		}

		while (TRUE) {
			_mutex_lock(&getline_mutex);
		    if (getline_q.items_ready) {
		    	strcpy(c, getline_q.data);
		    }
			_mutex_unlock(&getline_mutex);
			_mutex_unlock(&permissions_mutex);
			return TRUE;
		}
	}

	_mutex_unlock(&permissions_mutex);
	return FALSE;
}

_queue_id OpenW() {

	if (_mutex_lock(&permissions_mutex) != MQX_EOK) {
		printf("\r\n[%d] Couldn't Access Mutex in OpenW", _task_get_id());
	}

	int access_granted = 1;

	for (int i = 0; i < NUM_USER_TASK; i++) {
		if (user_permissions[i].task_id != _task_get_id()) {
			continue;
		}

		if (user_permissions[i].has_write_permissions) {
			access_granted = 0;
		} else {
			user_permissions[i].has_write_permissions = TRUE;
		}

		break;
	}

	_mutex_unlock(&permissions_mutex);
	return access_granted;
}

int _putline(_queue_id qid, char * string) {
	if (_mutex_lock(&permissions_mutex) != MQX_EOK) {
		printf("\r\n[%d] Couldn't Access Mutex in putline", _task_get_id());
	}

	for (int i = 0; i < NUM_USER_TASK; i++) {
		if (user_permissions[i].task_id != _task_get_id()) {
			continue;
		}

		_mutex_lock(&getline_mutex);
		strcpy(getline_q.data, string);
		getline_q.items_ready = TRUE;
		_mutex_unlock(&getline_mutex);
		_mutex_unlock(&permissions_mutex);
		return TRUE;
	}

	_mutex_unlock(&permissions_mutex);
	return FALSE;
}

int Close(void) {
	if (_mutex_lock(&permissions_mutex) != MQX_EOK) {
		printf("\r\n[%d] Couldn't Access Mutex in Close", _task_get_id());
	}

	int access_ungranted = TRUE;

	for (int i = 0; i < NUM_USER_TASK; i++) {
		if (user_permissions[i].task_id != _task_get_id()) {
			continue;
		}

		if (!user_permissions[i].has_read_permissions && !user_permissions[i].has_write_permissions) {
			access_ungranted = FALSE;
		}

		user_permissions[i].has_read_permissions = FALSE;
		user_permissions[i].has_write_permissions = FALSE;
	}

	_mutex_unlock(&permissions_mutex);
	return access_ungranted;
}




