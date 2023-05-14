#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "MultiCaster.h"

typedef struct ip_mreq{
    struct in_addr imr_multiaddr;
    struct in_addr imr_interface;
    struct in_addr imr_sourceaddr;
	}ip_mreq;

struct MultiCaster{
	int m_socket;
	int m_port;
	struct sockaddr_in m_address;
	struct sockaddr_in m_outAddress;
	ip_mreq m_req;
	char *m_buffer;
	size_t m_bufferSize;
	InDataHandler m_inHandler;
	};
	
MultiCaster *CreateCaster(char *_ip, int _port, size_t _bufferSize, InDataHandler _inHandler)
{
	MultiCaster *caster;
	unsigned int optval = 1;
	caster = (MultiCaster*)malloc(sizeof(MultiCaster));
	caster->m_buffer = (char*)malloc(sizeof(char)*_bufferSize);
	caster->m_bufferSize = _bufferSize;
	caster->m_port = _port;
	caster->m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (caster->m_socket < 0) {
        perror("socket");
        return NULL;
    }
    if (setsockopt(caster->m_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0){
       perror("Reusing ADDR failed");
       return NULL;
    }
    memset(&(caster->m_address), 0, sizeof(caster->m_address));
    caster->m_address.sin_family = AF_INET;
    caster->m_address.sin_addr.s_addr = htonl(INADDR_ANY);
    caster->m_address.sin_port = htons(_port);
	caster->m_inHandler = _inHandler;
    if (bind(caster->m_socket, (struct sockaddr*) &(caster->m_address), sizeof(caster->m_address)) < 0) {
        perror("bind");
        return NULL;
    }
    caster->m_req.imr_multiaddr.s_addr = inet_addr(_ip);
    caster->m_req.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(caster->m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &(caster->m_req), sizeof(caster->m_req)) < 0){
        perror("setsockopt");
        return NULL;
    }
    
    memset(&(caster->m_outAddress), 0, sizeof(caster->m_outAddress));
    caster->m_outAddress.sin_family = AF_INET;
    caster->m_outAddress.sin_addr.s_addr = inet_addr(_ip);
    caster->m_outAddress.sin_port = htons(_port);
    return caster;
}

void ListenMulti(MultiCaster *_caster)
{
	unsigned int addrlen = sizeof(_caster->m_address), nbytes;
    while (1) {
        nbytes = recvfrom(_caster->m_socket,_caster->m_buffer,_caster->m_bufferSize, 0,(struct sockaddr *) &(_caster->m_address),&addrlen);
        if (nbytes < 0) {
            perror("recvfrom");
            return;
        }
        _caster->m_inHandler(_caster,(void*)_caster->m_buffer, nbytes);
    }
}

int SendMulti(MultiCaster *_caster, char *_msg)
{
	int nbytes;
    nbytes = sendto(_caster->m_socket,_msg,strlen(_msg),0,(struct sockaddr*) &(_caster->m_outAddress),sizeof((_caster->m_outAddress)));
    if (nbytes < 0) {
    	perror("sendto");
        return 0;
    }
    return nbytes;
}

int SendMultiAnyData(MultiCaster *_caster, void *_msg, size_t _length)
{
	int nbytes;
    nbytes = sendto(_caster->m_socket,(unsigned char*)_msg,_length,0,(struct sockaddr*) &(_caster->m_outAddress),sizeof((_caster->m_outAddress)));
    if (nbytes < 0) {
    	perror("sendto");
        return 0;
    }
    return nbytes;
}
