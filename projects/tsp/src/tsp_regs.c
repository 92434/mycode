#include <stdio.h>     
#include <unistd.h>    
#include <signal.h>
#include <stdlib.h>   
#include <errno.h>    
#include <string.h>
#include <fcntl.h>    
#include <netinet/in.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <bitstream/mpeg/pes.h>
#include <bitstream/dvb/si.h>
#include "common_fun.h"
#include "utils.h"
#include "tsp_regs.h"
#include "dvb_process_si.h"

//typedef int bool;
//#define true (1 == 1)
//#define false (1 == 0)


static _tsp_container* tsp_container=NULL;
static _pmt_result pmt_result[MAX_PROGRAM_NUM];


int read_regs(thread_arg_t *targ, int addr, int count) {
	int nread;

	lseek(targ->fd, ADDR_OFFSET(addr), SEEK_SET);
	nread = read(targ->fd, targ->buffer, count);

	return nread;
}

int write_regs(thread_arg_t *targ, int addr, int count) {
	int nwrite;

	lseek(targ->fd, ADDR_OFFSET(addr), SEEK_SET);
	nwrite = write(targ->fd, targ->buffer, count);

	return nwrite;
}



int select_slot(thread_arg_t *targ, uint32_t slot) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	*pbuffer = slot;//
	ret = write_regs(targ, ADDR_INDEX, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	return ret;
}

int select_pid_slot(thread_arg_t *targ, uint32_t pid_slot) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	*pbuffer = pid_slot;//
	ret = write_regs(targ, ADDR_PID_INDEX, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	return ret;
}

int set_pid(thread_arg_t *targ, uint32_t pid, bool pid_enable, bool pid_change) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	*pbuffer = PID_INFO(pid_change, pid_enable, pid);
	ret = write_regs(targ, ADDR_PID, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	return ret;
}

int get_pid(thread_arg_t *targ, uint32_t *pid, bool *pid_enable, bool *pid_change) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	ret = read_regs(targ, ADDR_PID, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}
	*pid = *pbuffer & 0xffff;
	*pid_enable = (((*pbuffer >> 16) & 1) == 0) ? false : true;
	*pid_change = (((*pbuffer >> 17) & 1) == 0) ? false : true;
	return ret;
}


int get_matched_count(thread_arg_t *targ, uint32_t *matched_count) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	ret = read_regs(targ, ADDR_MATCHED_COUNT, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}
	*matched_count = *pbuffer; 
	return ret;
}

int set_slot_enable(thread_arg_t *targ, bool slot_enable) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	*pbuffer = slot_enable ? 1 : 0;//
	ret = write_regs(targ, ADDR_MATCH_ENABLE, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	return ret;
}

int get_slot_enable(thread_arg_t *targ, bool *slot_enable) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	ret = read_regs(targ, ADDR_MATCH_ENABLE, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	*slot_enable = (*pbuffer == 1) ? true : false;

	return ret;
}

int write_ts_data(thread_arg_t *targ, int tx_size, unsigned char slot) {
	int ret = 0;
	//int i;
	//uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	if(tx_size <= 0) {
		return ret;
	}

	ret = write_regs(targ, ADDR_TS_DATA_BASE, tx_size);
	if (ret < 0) {
		printf("tx_size:%d\n", tx_size);
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	return ret;
}

int read_ts_data(thread_arg_t *targ, int rx_size) {
	int ret = 0;
	int i;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	if(rx_size <= 0) {
		return ret;
	}

	*pbuffer = 0;//any value
	ret = write_regs(targ, ADDR_READ_REQUEST, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	*pbuffer = 0;
	i = 0;
	while(*pbuffer != 1) {
		ret = read_regs(targ, ADDR_READ_REQUEST, sizeof(uint32_t));
		if (ret < 0) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return ret;
		}
		if(i >= 200) {
			printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
			return -1;
		}
		cs_sleepms(2);
		i++;
	}

	ret = read_regs(targ, ADDR_TS_DATA_BASE, rx_size);//will valid after 3 clock
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	/*printf("\n");
	for(i = 0; i < rx_size; i++) {
		if((i != 0) && (i % 16 == 0)) {
			printf("\n");
		}
		printf("%02x ", targ->buffer[i]);
	}
	printf("\n");
	*/
	return ret;
}
/*
int test_set(thread_arg_t *targ) {
	int ret = 0;
	int i;

	for(i = 0; i < MONITOR_SIZE + REPLACER_SIZE + COMMON_REPLACER_SIZE; i++) {//slot
		int j;

		int pid_slot = (i == MONITOR_SIZE + REPLACER_SIZE + COMMON_REPLACER_SIZE - 1) ? 16 : 1;
		int tx_size = (i == MONITOR_SIZE + REPLACER_SIZE + COMMON_REPLACER_SIZE - 1) ? 188 * 2 : ((i >= MONITOR_SIZE) && (i <  MONITOR_SIZE + REPLACER_SIZE)) ? 188 : 0;
		bool slot_enable;

		switch(i) {
			case 0:
				slot_enable = true;
				break;
			case 1:
				slot_enable = true;
				break;
			case 2:
				slot_enable = true;
				break;
			case 9:
				slot_enable = true;
				break;
			default:
				slot_enable = false;
				break;
		}

		select_slot(targ, i);
		for(j = 0; j < pid_slot; j++) {//pid_slot
			uint32_t pid;
			bool pid_enable;

			switch(j) {
				case 0:
					switch(i) {
						case 0:
							pid = 0x0205;
							pid_enable = true;
							break;
						case 1:
							pid = 0x0533;
							pid_enable = true;
							break;
						case 2:
							pid = 0x0534;
							pid_enable = true;
							break;
						case 9:
							pid = 0x0000;
							pid_enable = true;
							break;
						default:
							pid = 0x0000;
							pid_enable = false;
							break;
					}
					break;
				case 9:
					pid = 0x0001;
					pid_enable = true;
					break;
				default:
					pid = 0x0000;
					pid_enable = false;
					break;
			}
			select_pid_slot(targ, j);
			set_pid(targ, pid, pid_enable);
		}

		write_ts_data(targ, tx_size, i);

		set_slot_enable(targ, slot_enable);
	}

	return ret;
}

int test_get(thread_arg_t *targ) {
	int ret = 0;
	int i;

	for(i = 0; i < MONITOR_SIZE + REPLACER_SIZE + COMMON_REPLACER_SIZE; i++) {//slot
		int j;

		int pid_slot = (i == MONITOR_SIZE + REPLACER_SIZE + COMMON_REPLACER_SIZE - 1) ? 16 : 1;
		int rx_size = (i == MONITOR_SIZE + REPLACER_SIZE + COMMON_REPLACER_SIZE - 1) ? 188 * 2 : ((i >= 0) && (i <  MONITOR_SIZE + REPLACER_SIZE)) ? 188 : 0;
		bool slot_enable;
		uint32_t matched_count;

		switch(i) {
			default:
				break;
		}

		select_slot(targ, i);
		printf("slot %d:\n", i);

		for(j = 0; j < pid_slot; j++) {//pid_slot
			uint32_t pid;
			bool pid_enable;

			switch(j) {
				default:
					break;
			}
			select_pid_slot(targ, j);
			get_pid(targ, &pid, &pid_enable);
			printf("pid: %08x; pid_enable: %s\n", pid, pid_enable ? "true" : "false");
		}

		get_matched_count(targ, &matched_count);
		get_slot_enable(targ, &slot_enable);
		printf("slot_enable %s; matched_count: %08x(%d)\n", slot_enable ? "true" : "false", matched_count, matched_count);

		read_ts_data(targ, rx_size);
	}

	return ret;
}

void main_proc(thread_arg_t *arg) {
	thread_arg_t *targ = (thread_arg_t *)arg;

	//printids("main_proc: ");

	while(tsp_stop == 0) {
		test_set(targ);
		test_get(targ);
		return;
	}
}

int read_write(thread_arg_t *targ) {
	int err;
	pthread_t rtid;
	pthread_t wtid;

	err = pthread_create(&rtid, NULL, read_fn, targ);
	if (err != 0) {
		printf("can't create thread: %s\n", strerror(err));
	}

	err = pthread_create(&wtid, NULL, write_fn, targ);
	if (err != 0) {
		printf("can't create thread: %s\n", strerror(err));
	}

	main_proc(targ);

	//printids("main thread:");
	pthread_join(rtid,NULL);
	pthread_join(wtid,NULL);

	return EXIT_SUCCESS;
}
*/
int tsp_monitor_read(thread_arg_t *targ,unsigned short pid, int slot_idx, unsigned char *ts_buf){
	int ret=0;
	//int i;
	//unsigned short pid1=0;
	if(targ==NULL||ts_buf==NULL||slot_idx>=TOTAL_SLOT_SIZE)
		return -1;
	//TRACE("tsp_monitor_read,pid:%d,slot_idx:%d\n",pid,slot_idx);
	select_slot(targ, slot_idx);
	select_pid_slot(targ, 0);
	set_pid(targ, pid, true, 0);
	set_slot_enable(targ, true);
	ret=read_ts_data(targ, PACK_BYTE_SIZE);
	if(ret==PACK_BYTE_SIZE){
		//dump_packet("monitor",targ->buffer,PACK_BYTE_SIZE);
		tsp_container->monitor[slot_idx].inuse=1;
		tsp_container->monitor[slot_idx].pid=pid;
		memcpy(tsp_container->monitor[slot_idx].ts_pack,targ->buffer,ret);
		memcpy(ts_buf,targ->buffer,ret);
	}
	else
		TRACE("[tsp_monitor_read]read %d B,err:%s\n",ret,strerror(errno));
	return ret;
}

int tsp_monitor_stop(thread_arg_t *targ, int slot_idx){
	int ret=0;
	if(targ==NULL||slot_idx>=MONITOR_SIZE)
		return -1;
	ret=select_slot(targ, slot_idx);
	ret|=select_pid_slot(targ, 0);
	ret|=set_slot_enable(targ, false);
	memset(&(tsp_container->monitor[slot_idx]),0,sizeof(tsp_monitor));
	return ret;
}

int tsp_clear_single_slot_withpid(thread_arg_t *targ,unsigned short pid){
	int i,ret=0;
	for(i=MONITOR_SIZE;i<MONITOR_SIZE+REPLACER_SIZE;i++){//single
		if(pid==tsp_container->replacer_single[i-MONITOR_SIZE].pid
			&&tsp_container->replacer_single[i-MONITOR_SIZE].inuse==1){
			break;
		}
	}
	if(i>=MONITOR_SIZE+REPLACER_SIZE){
		TRACE("[tsp_clear_single_slot_withpid]can not find the slot for pid:(0x%0x)\n",pid);
		return -2;
	}
	else{
		memset(&(tsp_container->replacer_single[i-MONITOR_SIZE]),0,sizeof(tsp_replacer_single));
		ret=select_slot(targ, i);
		ret|=select_pid_slot(targ, 0);
		ret|=set_pid(targ, pid, false, 0);
		ret|=set_slot_enable(targ, false);
	}
	return ret;
}

int tsp_replace_single_tspack(thread_arg_t *targ,unsigned short pid, unsigned char *ts_buf){
	int i,ret=0;
	int reset = 0;
	if(targ==NULL||ts_buf==NULL)
		return -1;
	for(i=MONITOR_SIZE;i<MONITOR_SIZE+REPLACER_SIZE;i++){//single
		if(pid==tsp_container->replacer_single[i-MONITOR_SIZE].pid
			&&tsp_container->replacer_single[i-MONITOR_SIZE].inuse==1){
			if(memcmp(ts_buf,tsp_container->replacer_single[i-MONITOR_SIZE].ts_pack,PACK_BYTE_SIZE)){
				memcpy(tsp_container->replacer_single[i-MONITOR_SIZE].ts_pack,ts_buf,PACK_BYTE_SIZE);
				reset=1;
			}
			break;
		}
	}
	
	if(i>=MONITOR_SIZE+REPLACER_SIZE){
		for(i=MONITOR_SIZE;i<MONITOR_SIZE+REPLACER_SIZE;i++){
			if(tsp_container->replacer_single[i-MONITOR_SIZE].inuse==0){
				tsp_container->replacer_single[i-MONITOR_SIZE].inuse=1;
				tsp_container->replacer_single[i-MONITOR_SIZE].pid=pid;
				tsp_container->replacer_single[i-MONITOR_SIZE].slot_idx=i;
				memcpy(tsp_container->replacer_single[i-MONITOR_SIZE].ts_pack,ts_buf,PACK_BYTE_SIZE);
				reset=1;
				break;
			}
		}
		if(i>=MONITOR_SIZE+REPLACER_SIZE){
			TRACE("[tsp_replace_single_tspack]can not find a empty slot for pid:(0x%0x)\n",pid);
			return -2;
		}
	}
	else{
		TRACE("[tsp_replace_single_tspack]find a existing pid in slot %d,reset:%d \n",i,reset);
	}
		
	if(reset){
		ret=select_slot(targ, i);
		ret|=select_pid_slot(targ, 0);
		ret|=set_pid(targ, pid, 1, 1);
		memcpy(targ->buffer,ts_buf,PACK_BYTE_SIZE);
		ret|=write_ts_data(targ, PACK_BYTE_SIZE, i);
		ret|=set_slot_enable(targ, 1);
	}
	return ret;
}


int tsp_clear_dualslot_by_pid(thread_arg_t *targ,unsigned short pid){
	int i,j,ret=0;
	for(i=MONITOR_SIZE+REPLACER_SIZE;i<TOTAL_SLOT_SIZE;i++){//dual
		for(j=0;j<DUAL_SLOT_PID_COUNT;j++)
			if(pid==tsp_container->replacer_dual[i-(MONITOR_SIZE+REPLACER_SIZE)].pid[j]
				&&tsp_container->replacer_dual[i-(MONITOR_SIZE+REPLACER_SIZE)].inuse==1){
				break;
			}
	}
	if(i>=TOTAL_SLOT_SIZE){
		TRACE("[tsp_clear_dualslot_by_pid]can not find the slot for pid:(0x%0x)\n",pid);
		return -2;
	}
	else{
		tsp_container->replacer_dual[i-(MONITOR_SIZE+REPLACER_SIZE)].pid[j]=0;
		ret=select_slot(targ, i);
		ret|=select_pid_slot(targ, j);
		ret|=set_pid(targ, pid, false, 0);
	}
	return ret;
}

//notice:slot_idx is a global index
int tsp_clear_replace_slot(thread_arg_t *targ, int slot_idx){
	int ret=0;
	if(targ==NULL)
		return -1;
	ret=select_slot(targ, slot_idx);
	ret|=set_slot_enable(targ, false);
	if(slot_idx>=MONITOR_SIZE&&slot_idx<MONITOR_SIZE+REPLACER_SIZE)
		memset(&(tsp_container->replacer_single[slot_idx-MONITOR_SIZE]),0,sizeof(tsp_replacer_single));
	else if(slot_idx>=MONITOR_SIZE+REPLACER_SIZE&&slot_idx<TOTAL_SLOT_SIZE)
		memset(&(tsp_container->replacer_dual[slot_idx-MONITOR_SIZE-REPLACER_SIZE]),0,sizeof(tsp_replacer_single));
	else
		return -1;
	return ret;
}

int tsp_replace_dual_tspack(thread_arg_t *targ,unsigned short *pid_array, int pid_num, unsigned char *ts_buf){
	int i,j,ret=0;
	int reset = 0;
	if(targ==NULL||ts_buf==NULL||pid_array==NULL||pid_num>DUAL_SLOT_PID_COUNT)
		return -1;
	for(i=MONITOR_SIZE+REPLACER_SIZE;i<TOTAL_SLOT_SIZE;i++){//dual
		if(memcmp(ts_buf,tsp_container->replacer_dual[i-(MONITOR_SIZE+REPLACER_SIZE)].ts_pack,2*PACK_BYTE_SIZE)==0){
			for(j=0;j<pid_num;j++)
				tsp_container->replacer_dual[i-(MONITOR_SIZE+REPLACER_SIZE)].pid[j]=pid_array[j];
			reset=1;
			break;
		}
	}
	
	if(i>=TOTAL_SLOT_SIZE){
		for(i=MONITOR_SIZE+REPLACER_SIZE;i<TOTAL_SLOT_SIZE;i++){
			if(tsp_container->replacer_dual[i-(MONITOR_SIZE+REPLACER_SIZE)].inuse==0){
				tsp_container->replacer_dual[i-(MONITOR_SIZE+REPLACER_SIZE)].inuse=1;
				tsp_container->replacer_dual[i-(MONITOR_SIZE+REPLACER_SIZE)].slot_idx=i;
				memcpy(tsp_container->replacer_dual[i-(MONITOR_SIZE+REPLACER_SIZE)].ts_pack,ts_buf,2*PACK_BYTE_SIZE);
				for(j=0;j<pid_num;j++)
					tsp_container->replacer_dual[i-(MONITOR_SIZE+REPLACER_SIZE)].pid[j]=pid_array[j];
				reset=1;
				break;
			}
		}
		if(i>=TOTAL_SLOT_SIZE){
			TRACE("[tsp_replace_dual_tspack]can not find a empty dual slot\n");
			return -2;
		}
	}
	else{
		TRACE("[tsp_replace_dual_tspack]find a existing pid in slot %d,reset:%d \n",i,reset);
	}
		
	if(reset){
		ret=select_slot(targ, i);
		for(j=0;j<pid_num;j++){
			ret|=select_pid_slot(targ, j);
			ret|=set_pid(targ, pid_array[j], 1, 0);
		}
		memcpy(targ->buffer,ts_buf,2*PACK_BYTE_SIZE);
		ret|=write_ts_data(targ, 2*PACK_BYTE_SIZE, i);
		ret|=set_slot_enable(targ, 1);
	}
	return ret;
}

int tsp_add_dual_slot_pid(thread_arg_t *targ,unsigned short pid){
	int i,j,ret=0;
	//int reset = 0;
	if(targ==NULL||pid==0)
		return -1;
	for(i=MONITOR_SIZE+REPLACER_SIZE;i<TOTAL_SLOT_SIZE;i++){//dual
		for(j=0;j<DUAL_SLOT_PID_COUNT;j++){
			if(tsp_container->replacer_dual[i-(MONITOR_SIZE+REPLACER_SIZE)].pid[j]==pid)//already exist
				return 1;
		}
	}
	for(i=MONITOR_SIZE+REPLACER_SIZE;i<TOTAL_SLOT_SIZE;i++){//dual
		for(j=0;j<DUAL_SLOT_PID_COUNT;j++){
			if(tsp_container->replacer_dual[i-(MONITOR_SIZE+REPLACER_SIZE)].pid[j]==0){
				ret=select_slot(targ, i);
				ret|=select_pid_slot(targ, j);
				ret|=set_pid(targ, pid, 1, 0);
				ret|=set_slot_enable(targ, 1);
				tsp_container->replacer_dual[i-(MONITOR_SIZE+REPLACER_SIZE)].pid[j]=pid;
				return 0;
			}
		}
	}
	
	return -1;
}

static void parse_pmt(uint8_t *p_pmt, _pmt_result* pmt_array)
{
	uint8_t *p_es;
    uint8_t i=0,j = 0;
	unsigned short pid=0,streamtype=0;
	pmt_array->sid= pmt_get_program(p_pmt);
	pmt_array->pcr_pid=pmt_get_pcrpid(p_pmt);
	while ((p_es = pmt_get_es(p_pmt, j)) != NULL) {
        j++;
        pid=pmtn_get_pid(p_es);
		streamtype=pmtn_get_streamtype(p_es);
		switch(streamtype){
			case 0x01:
			case 0x02:
			case 0x10:
			case 0x1B:
			case 0x42:
				pmt_array->video_pid=pid;
				break;
			case 0x03:
			case 0x04:
			case 0x06:
			case 0x0F:
			case 0x11:
			case 0x73:
			case 0x81:
			{
				if(streamtype==0x06){
					uint8_t *p_descs;
					uint16_t i_length;
					uint8_t *p_descl;
					uint8_t *p_desc;
					uint16_t k = 0;
					uint8_t private_audio_flag=0; 
					p_descs=pmtn_get_descs(p_es);
					i_length=descs_get_length(p_descs);
					p_descl=p_descs + DESCS_HEADER_SIZE;
					while ((p_desc = descl_get_desc(p_descl, i_length, k)) != NULL) {
				        uint8_t i_tag = desc_get_tag(p_desc);
						TRACE("i_tag:%0x\n",i_tag);
						if(i_tag==0x6a		//AC-3 Audio
							||i_tag==0x73	//DTS Audio
							||i_tag==0x81)	//AC-3 Audio
							private_audio_flag=1;
				        k++;
					}
					if(private_audio_flag==0)
						break;
				}
				for(i=0;i<5;i++){
					if(pmt_array->audio_pid[i]==pid)
						break;
				}
				if(i==5){
					for(i=0;i<5;i++){
						if(pmt_array->audio_pid[i]==0){
							pmt_array->audio_pid[i]=pid;
							break;
						}
					}
				}
				break;
			}
			default:
				break;
		}
    }
	if(pmt_array->video_pid==0)//audio program
		memset(pmt_array,0,sizeof(_pmt_result));
}

int tsp_check_desc_ac3(uint8_t *p_es){
	uint8_t *buf=NULL;
	int cursor=5;
	int desc_seg_len=pmtn_get_desclength(p_es);
	while(desc_seg_len>0){
		int tag_len=0;
		buf=p_es+cursor;
		if(buf[0]==0x6a||buf[0]==0x81)
			return cursor;
		tag_len=buf[1];
		cursor+=tag_len;
		desc_seg_len-=(2+tag_len);
	}
	return 0;
}
int tsp_modify_pmt_toac3(uint8_t *ts, _pmt_result* pmt_array,uint8_t *new_buf){
	uint8_t *p_pmt;
	uint16_t cursor=0;
	uint16_t section_length=0,desc_seg_len=0;
	uint8_t *p_es;
    uint8_t i=0,j = 0,find_pid=0;
	unsigned short pid=0;
	unsigned char crc[4]={0};
	//unsigned char audio_buf[16]={
		//0x06,0xE0,0x00,0xF0,0x0B,0x6A,0x03,0xC0,0x44,0x08,0x0A,0x04,0x63,0x68,0x69,0x00
	//};
	unsigned char ac3_desc[5]={0x6A,0x03,0xC0,0x44,0x08};
	memset(new_buf,0xFF,PACK_BYTE_SIZE);
	p_pmt=ts+5;
	section_length=psi_get_length(p_pmt);
	memcpy(crc,ts+5+3+section_length-4,4);
	p_es = pmt_get_es(p_pmt, 0);
	memcpy(new_buf,ts,(p_es-ts));
	cursor+=(p_es-ts);
	while ((p_es = pmt_get_es(p_pmt, j)) != NULL) {
        j++;
        pid=pmtn_get_pid(p_es);
		desc_seg_len=5+pmtn_get_desclength(p_es);
		TRACE("pid:%0x,desc_seg_len:%0x\n",pid,desc_seg_len);
		for(i=0;i<5;i++){
			if(pid!=0&&pid==pmt_array->audio_pid[i]){
				unsigned char *aud=new_buf+cursor;
				int ac3_pos=0;
				
				ac3_pos=tsp_check_desc_ac3(p_es);
				if(ac3_pos>=5){
					int tag_len=p_es[ac3_pos+1];
					if(ac3_pos==5){//ac3 is the first tag
						memcpy(new_buf+cursor,p_es,5);
						aud[0]=0x06;
						pmtn_set_desclength(new_buf+cursor,desc_seg_len-5-(2+tag_len)+5);
						memcpy(new_buf+cursor+5,ac3_desc,5);
						cursor+=10;
						if(desc_seg_len-5-5>0){
							memcpy(new_buf+cursor,p_es+5+5,desc_seg_len-5-5);
							cursor+=(desc_seg_len-5-5);
						}
					}
					else{
						memcpy(new_buf+cursor,p_es,ac3_pos);
						aud[0]=0x06;
						pmtn_set_desclength(new_buf+cursor,desc_seg_len-5-(2+tag_len)+5);
						memcpy(new_buf+cursor+ac3_pos,ac3_desc,5);
						cursor+=ac3_pos+5;
						if(desc_seg_len-ac3_pos-5>0){
							memcpy(new_buf+cursor,p_es+ac3_pos+5,desc_seg_len-ac3_pos-5);
							cursor+=(desc_seg_len-ac3_pos-5);
						}
					}
				}
				else{
					memcpy(new_buf+cursor,p_es,desc_seg_len);
					aud[0]=0x06;
					pmtn_set_desclength(new_buf+cursor,desc_seg_len);
					cursor+=desc_seg_len;
					memcpy(new_buf+cursor,ac3_desc,5);
					cursor+=5;
				}
				find_pid=1;
				break;
			}
		}
		if(find_pid==0){
			memcpy(new_buf+cursor,p_es,desc_seg_len);
			cursor+=desc_seg_len;
		}
	}
	memcpy(new_buf+cursor,crc,4);
	cursor+=4;
	psi_set_length(new_buf+5,cursor-5-3);
	return 0;
}

int tsp_get_program_info(thread_arg_t *targ, sid_t *sids){
	int i,ret=-1;
	int read_bytes=0;
	int service_num=0;
	unsigned char ts_buf[PACK_BYTE_SIZE]={0},new_pmt[PACK_BYTE_SIZE]={0};
	memset(&pmt_result,0,MAX_PROGRAM_NUM*sizeof(_pmt_result));
	
	read_bytes=tsp_monitor_read(targ, 0, 0, ts_buf);//PAT
	if(read_bytes==PACK_BYTE_SIZE){
		handle_psi_packet(ts_buf);
		service_num=get_pat_result(sids);
		
		if(service_num>0){
			for(i=0;i<service_num;i++){
				TRACE("[%d]pmt pid:%d\n",i,sids[i].i_pmt_pid);
				while(sids[i].i_pmt_pid!=0){
					read_bytes=tsp_monitor_read(targ,sids[i].i_pmt_pid, 0, ts_buf);//PMT
					if(read_bytes==PACK_BYTE_SIZE&&ts_get_unitstart(ts_buf)==1){
						handle_psi_packet(ts_buf);
						parse_pmt(ts_section(ts_buf),&pmt_result[i]);
						if(0){//pmt_result[i].video_pid!=0){
							int pmt_valid=0;
							uint8_t *p_pmt_sec=NULL;
							tsp_modify_pmt_toac3(ts_buf,&pmt_result[i],new_pmt);
							dump_packet("new pmt",new_pmt,PACK_BYTE_SIZE);
							p_pmt_sec=ts_section(new_pmt);
							psi_set_crc(p_pmt_sec);
							pmt_valid=pmt_validate(p_pmt_sec);
							TRACE("pmt_valid:%d\n",pmt_valid);
							tsp_replace_single_tspack(targ,sids[i].i_pmt_pid,new_pmt);
						}
						break;
					}
				}
			}
			ret=0;
		}
	}
	return ret;
}

unsigned char pid_ac3[188*2]={
0x47,0x41,0x05,0x35,0x0a,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,
0x00,0x01,0xbd,0x00,0xb6,0x80,0x80,0x05,0x21,0x00,0x01,0x00,0x01,0x0b,0x77,0xa1,
0xdd,0x42,0x40,0x2f,0x84,0x2b,0x03,0x81,0x43,0x43,0x39,0xfa,0x53,0x2f,0xe6,0x0c,
0x1f,0x34,0x4d,0x72,0x05,0x56,0x09,0xfc,0x64,0x8f,0x03,0x67,0x9c,0x3a,0xa1,0xee,
0x1f,0x6c,0x51,0xa6,0x01,0x8e,0x45,0x50,0xa6,0x07,0x00,0x8f,0x01,0x3a,0x3f,0x00,
0x31,0xa4,0x7e,0xef,0xc3,0x00,0x4f,0x1e,0x06,0xe4,0x75,0xaa,0x42,0xe5,0x80,0xcb,
0x23,0xb1,0x8d,0x33,0x25,0x20,0x3c,0xd3,0x7d,0x7c,0x09,0x5c,0x4a,0xe6,0xc2,0xaa,
0x69,0x80,0x7a,0xba,0x12,0x18,0x6e,0x13,0xe4,0x0e,0x85,0xdf,0xb8,0x2c,0x4d,0x59,
0xda,0xd6,0xb1,0x00,0x6e,0x85,0x7a,0x18,0x66,0x30,0xec,0x26,0x2e,0x4e,0x33,0x4b,
0x38,0xc1,0xce,0x51,0x3d,0x20,0x3c,0x0a,0x65,0x75,0x89,0x92,0x67,0xae,0x94,0x34,
0xb7,0x40,0x44,0x27,0x6b,0xc0,0x54,0xdf,0x24,0x97,0xc1,0x48,0xb3,0x95,0x60,0x90,
0xd6,0x5c,0x6a,0x12,0xa5,0x00,0x53,0x8b,0xb1,0x7e,0xe8,0x89,

0x47,0x01,0x05,0x3b,0xa8,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x24,0x45,0x1c,
0xc1,0x6d,0xdb,0x13,0xe6,0xb5,0x93,0x5e,0xd6,0xb0,0xd7,0xa5,};

#define TEST_PID 650

int init_tsp_reg(char *dev) {
	int ret = 0;
	int i,j,audio_pid_cnt=0;
	unsigned short pid_array[DUAL_SLOT_PID_COUNT]={0};
	sid_t *sids=NULL;//[MAX_PROGRAM_NUM];
	thread_arg_t targ;

	if ((targ.fd = open(dev, O_RDWR))<0) {
		printf("err: can't open device(%s)!(%s)\n", dev, strerror(errno));
		ret = -1;
		return ret;
	}
	if(tsp_container==NULL)
		tsp_container=(_tsp_container *)malloc(sizeof(_tsp_container));
	if(tsp_container==NULL){
		printf("err: malloc memory fail for tsp_container\n");
		close(targ.fd);
		return -2;
	}

	targ.buffer = (unsigned char *)malloc(TSP_BUFSIZE);
	if(targ.buffer == 0) {
		printf("err: no memory for targ.buffer!\n");
		ret = -1;
		return ret;
	}
	sids=(sid_t*)malloc(MAX_PROGRAM_NUM*sizeof(sid_t));
	if(sids==NULL){
		TRACE("sids malloc fail\n");
		ret = -2;
		return ret;
	}
	memset(sids,0,MAX_PROGRAM_NUM*sizeof(sid_t));
	for(i=MONITOR_SIZE;i<TOTAL_SLOT_SIZE;i++){
		tsp_clear_replace_slot(&targ,i);
	}
	//goto tsp_exit;
	init_dvb_process();
	ret=tsp_get_program_info(&targ, sids);
	if(ret<0){
		TRACE("tsp_get_program_info fail,ret:%d\n",ret);
	}
	else{

		for(i=0;i<MAX_PROGRAM_NUM;i++){
			if(pmt_result[i].sid!=0){
				TRACE("idx:%d,sid:%d,video_pid:%d,audio:",i,pmt_result[i].sid,pmt_result[i].video_pid);
				for(j=0;j<5;j++) if(pmt_result[i].audio_pid[j]!=0) TRACE("%d,",pmt_result[i].audio_pid[j]);
				TRACE("\n");
			}
		}
	}
	
	for(i=0;i<MAX_PROGRAM_NUM;i++){
		for(j=0;j<5;j++){
			if(pmt_result[i].audio_pid[j]!=0){
				pid_array[audio_pid_cnt]=pmt_result[i].audio_pid[j];
				audio_pid_cnt++;
				if(audio_pid_cnt>=DUAL_SLOT_PID_COUNT)
					break;
			}
		}
	}
	/*
	ret=tsp_replace_dual_tspack(&targ,pid_array, audio_pid_cnt, pid_ac3);
	if(ret<0){
		TRACE("tsp_replace_dual_tspack fail,ret:%d\n",ret);
	}
	*/
	int header_size;
	uint64_t pts=0;
	uint8_t *p_ts;
	unsigned char ts_buf[PACK_BYTE_SIZE]={0};
	while(1){
		memset(pid_array,0,sizeof(pid_array));
		tsp_monitor_read(&targ, TEST_PID, 0, ts_buf);
		p_ts=ts_buf;
		header_size = TS_HEADER_SIZE + (ts_has_adaptation(p_ts) ? 1+ ts_get_adaptation(p_ts) : 0) ;
		if(ts_get_unitstart(p_ts) &&pes_validate(p_ts+TS_HEADER_SIZE)){//has pes header
			if(pes_validate_header(p_ts + header_size) &&pes_has_pts(p_ts + header_size)
                		&&pes_validate_pts(p_ts + header_size)){
                //pes_payload_len=pes_get_length(p_ts + header_size)-pes_get_headerlength(p_ts + header_size);
				pts=pes_get_pts(p_ts + header_size);
				header_size = TS_HEADER_SIZE + (ts_has_adaptation(pid_ac3) ? 1+ ts_get_adaptation(pid_ac3) : 0) ;
				pes_set_pts(pid_ac3+header_size,pts);
				//printf("========pid:%0d=======pts:%0lx=============\n",t_pid,pts);
				pid_array[0]=TEST_PID;
				if(1){
					static int tm=0;
					int cur_tm=(int)time((time_t *) 0);
					if(cur_tm-tm>=2){
						pid_ac3[188-2]++;
						tm=cur_tm;
						TRACE("replace byte:%02x\n",pid_ac3[188-2]);
					}
				}
				ret=tsp_replace_dual_tspack(&targ,pid_array, 1, pid_ac3);
				if(ret<0){
					TRACE("tsp_replace_dual_tspack fail,ret:%d\n",ret);
				}
				cs_sleepms(10);
				tsp_clear_replace_slot(&targ,TOTAL_SLOT_SIZE-1);
			}
		}
		/*
		if(ts_has_adaptation(ts_buf)){
			 if ( ts_get_adaptation(ts_buf) &&
                tsaf_has_pcr(ts_buf)){
                i_pcr = tsaf_get_pcr(ts_buf);
				tsaf_set_pcr(pid_ac3,i_pcr);
				pid_array[0]=TEST_PID;
				ret=tsp_replace_dual_tspack(&targ,pid_array, 1, pid_ac3);
				if(ret<0){
					TRACE("tsp_replace_dual_tspack fail,ret:%d\n",ret);
				}
				cs_sleepms(10);
				tsp_clear_replace_slot(&targ,TOTAL_SLOT_SIZE-1);
			 }
		}*/
		cs_sleepms(10);
		#if 0
		pid_ac3[188-2]++;
		TRACE("replace byte:%02x\n",pid_ac3[188-2]);
		
		ret=tsp_replace_dual_tspack(&targ,pid_array, audio_pid_cnt, pid_ac3);
		if(ret<0){
			TRACE("tsp_replace_dual_tspack fail,ret:%d\n",ret);
		}
		
		
		cs_sleepms(20);
		tsp_clear_replace_slot(&targ,TOTAL_SLOT_SIZE-1);
		cs_sleepms(500);
		#endif
	}
//tsp_exit:
	destroy_dvb_process();
	if(targ.buffer){
		free(targ.buffer);
		targ.buffer=NULL;
	}
	if(tsp_container){
		free(tsp_container);
		tsp_container=NULL;
	}
	close(targ.fd);

	return ret;
}
