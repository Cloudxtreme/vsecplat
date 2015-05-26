#ifndef __VSECPLAT_CONFIG_H__
#define __VSECPLAT_CONFIG_H__
#include "nm_type.h"
#include "nm_dev.h"

struct mgt_cfg{
	char name[NM_NAME_LEN];
	char ipaddr[NM_ADDR_STR_LEN];
};

struct serv_cfg{
	char ipaddr[NM_ADDR_STR_LEN];
	int port;	
};

struct inport_desc{
	char name[NM_NAME_LEN];
	struct nm_dev *dev;
};

struct outport_desc{
	char name[NM_NAME_LEN];
	unsigned char dst_mac[NM_MAC_LEN];
	struct nm_dev *dev;
};

struct vsecplat_config{
	struct mgt_cfg *mgt_cfg;
	struct serv_cfg *serv_cfg;
	int inport_num;
	struct inport_desc *inport_desc_array;
	int outport_num;
	struct outport_desc *outport_desc_array;
};

extern struct vsecplat_config *global_vsecplat_config;
int parse_vsecplat_config(void);
#endif
