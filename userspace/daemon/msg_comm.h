#ifndef __MSG_COMM_H__
#define __MSG_COMM_H__

#include "thread.h"

struct conn_desc{
	int sock;
	char *recv_buf;
	char *send_buf;
	int recv_len;
	int send_len;
};

enum{
	NM_GET_RULES=1, /* vm get the policy rules */
	NM_ADD_RULES,   /* manage center add the policy rules */
	NM_DEL_RULES,	/* manage center del the policy rules */
	NM_REPORT_COUNT,		/* vm report the packet records */
};

struct msg_head{
	int len;
	unsigned char msg_type;
	char data[0];
};

int init_sock(char *ipaddr, int port);
int vsecplat_timer_func(struct thread *thread);

#endif
