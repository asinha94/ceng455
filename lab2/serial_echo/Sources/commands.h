#ifndef __commands_H_
#define __commands_H_


_queue_id openR(uint16_t);
_queue_id openW(void);
int _getline(char *);
int _putline(_queue_id, char *);
int Close(void);

#endif
