/* UDP Multicast by Ron Shani */
#ifndef __MULTICASTER_H__
#define __MULTICASTER_H__
#include <stddef.h>

typedef struct MultiCaster MultiCaster;
/*
Incommind data handler - should be supplied by user
parameters:
	_caster - MultiCaster pointer
	_data - pointer to data
	_length - incomming data bytes length*/
typedef void (*InDataHandler)(MultiCaster *_caster, void *_data, size_t _length);

/*Creates MultiCaster object
parameters:
	_ip - ip address as string. must be between 224.0.0.0 - 239.255.255.255
	_port - port number
	_bufferSize - buffer size for incomming data
	_inHandler - pointer to incommind data handler*/
MultiCaster *CreateCaster(char *_ip, int _port, size_t _bufferSize, InDataHandler _inHandler);

/*Never-ending loop that listens to incomming data*/
void ListenMulti(MultiCaster *_caster);

/*Send string message
returns number of bytes sent or 0 if error*/
int SendMulti(MultiCaster *_caster, char *_msg);

/*Send any data (blob) message
returns number of bytes sent or 0 if error*/
int SendMultiAnyData(MultiCaster *_caster, void *_msg, size_t _length);

#endif /*__MULTICASTER_H__*/
