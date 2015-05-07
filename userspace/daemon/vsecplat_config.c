#include "rte_json.h"
#include "vsecplat_config.h"

#define VSECPLATFORM_CFG_FILE "./config.json"
struct vsecplat_config *global_vsecplat_config;

static int str_to_mac(const char *bufp, char *ptr)
{
	int i, j;
	unsigned char val;
	unsigned char c;

	i = 0;
	do {
		j = val = 0;

		/* We might get a semicolon here - not required. */
		if (i && (*bufp == ':')) {
			bufp++;
		}

		do {
			c = *bufp;
			if (((unsigned char)(c - '0')) <= 9) {
				c -= '0';
			} else if (((unsigned char)((c|0x20) - 'a')) <= 5) {
				c = (c|0x20) - ('a'-10);
			} else if (j && (c == ':' || c == 0)) {
				break;
			} else {
				return -1;
			}
			++bufp;
			val <<= 4;
			val += c;
		} while (++j < 2);
		*ptr++ = val;
	} while (++i < NM_MAC_LEN);

	return *bufp; /* Error if we don't end at end of string. */
}

int parse_vsecplat_config(void)
{
	int fd=0;
	int len=0;
	struct stat stat_buf;
	char *file_buf=NULL;
	struct rte_json *json=NULL, *item=NULL, *entry=NULL;
	struct rte_json *tmp=NULL;
	int idx=0;

	memset(&stat_buf, 0, sizeof(struct stat));
	stat(VSECPLATFORM_CFG_FILE, &stat_buf);
	len = stat_buf.st_size;
	
	file_buf = malloc(len);
	if(NULL==file_buf){
		return -1;
	}
	memset(file_buf, 0, len);
	fd = open(VSECPLATFORM_CFG_FILE, O_RDONLY);
	if(fd<0){
		free(file_buf);
		return -1;
	}
	read(fd, file_buf, len);
	close(fd);

	global_vsecplat_config = malloc(sizeof(struct vsecplat_config));		
	if(NULL==global_vsecplat_config){
		free(file_buf);
		return -1;
	}
	memset(global_vsecplat_config, 0, sizeof(struct vsecplat_config));
	json = rte_parse_json(file_buf);
	if(NULL==json){
		free(file_buf);
		free(global_vsecplat_config);
		return -1;
	}

	item = rte_object_get_item(json, "mgt_cfg");	
	if(NULL!=item){
		struct mgt_cfg *mgt_cfg = malloc(sizeof(struct mgt_cfg));			
		if(NULL==mgt_cfg){
			// TODO
			goto out;
		}
		memset(mgt_cfg, 0, sizeof(struct mgt_cfg));
	#if 0
		tmp = rte_object_get_item(item, "name");
		if(NULL==tmp){
			// TODO
		}
		strncpy(mgt_cfg->name, tmp->u.val_str, NM_NAME_LEN);
	#endif
		tmp = rte_object_get_item(item, "ipaddr");	
		if(NULL==tmp){
			// TODO
			goto out;
		}
		strncpy(mgt_cfg->ipaddr, tmp->u.val_str, NM_ADDR_STR_LEN);
		tmp = rte_object_get_item(item, "mac");
		if(NULL==tmp){
			// TODO
			goto out;
		}
		// strncpy(mgt_cfg->mac, tmp->u.val_str, NM_ADDR_STR_LEN);
		str_to_mac(tmp->u.val_str, mgt_cfg->mac);
		global_vsecplat_config->mgt_cfg = mgt_cfg;
	}

	item = rte_object_get_item(json, "serv_cfg");
	if(NULL!=item){
		struct serv_cfg *serv_cfg = malloc(sizeof(struct serv_cfg));	
		if(NULL==serv_cfg){
			// TODO
			goto out;
		}
		memset(serv_cfg, 0, sizeof(struct serv_cfg));
		tmp = rte_object_get_item(item, "ipaddr");
		if(NULL==tmp){
			// TODO
			goto out;
		}
		strncpy(serv_cfg->ipaddr, tmp->u.val_str, NM_ADDR_STR_LEN);
		tmp = rte_object_get_item(item, "port");
		if(NULL==tmp){
			// TODO
			goto out;
		}
		serv_cfg->port = tmp->u.val_int;
		global_vsecplat_config->serv_cfg = serv_cfg;
	}

	item = rte_object_get_item(json, "inport_list");
	if(NULL!=item){
		global_vsecplat_config->inport_num = rte_array_get_size(item);
		global_vsecplat_config->inport_list = malloc(global_vsecplat_config->inport_num * sizeof(struct inport_list));
		if(NULL==global_vsecplat_config->inport_list){
			//TODO
			goto out;
		}
		memset(global_vsecplat_config->inport_list, 0, sizeof(struct inport_list)*global_vsecplat_config->inport_num);
		for(idx=0;idx<global_vsecplat_config->inport_num;idx++){
			entry = rte_array_get_item(item, idx);
			if(NULL==entry){
				// TODO
				goto out;
			}
			tmp = rte_object_get_item(entry, "mac");
			if(NULL==tmp){
				// TODO
				goto out;
			}
			// strncpy(global_vsecplat_config->inport_list[idx].mac, tmp->u.val_str, NM_ADDR_STR_LEN);
			str_to_mac(tmp->u.val_str, global_vsecplat_config->inport_list[idx].mac);	
		}
	}

	item = rte_object_get_item(json, "outport_list");
	if(NULL!=item){
		global_vsecplat_config->outport_num = rte_array_get_size(item);
		global_vsecplat_config->outport_list = malloc(global_vsecplat_config->outport_num * sizeof(struct outport_list));		
		if(NULL==global_vsecplat_config->outport_list){
			// TODO
			goto out;
		}
		memset(global_vsecplat_config->outport_list, 0, sizeof(struct outport_list)*global_vsecplat_config->outport_num);
		for(idx=0;idx<global_vsecplat_config->outport_num;idx++){
			entry = rte_array_get_item(item, idx);
			if(NULL==entry){
				// TODO
				goto out;
			}		
			tmp = rte_object_get_item(entry, "mac");
			if(NULL==tmp){
				// TODO
				goto out;
			}
			// strncpy(global_vsecplat_config->outport_list[idx].mac, tmp->u.val_str, NM_ADDR_STR_LEN);
			str_to_mac(tmp->u.val_str, global_vsecplat_config->outport_list[idx].mac);	
		}
	}

	rte_destroy_json(json);
	return  0;

out:
	rte_destroy_json(json);
	return -1;
}