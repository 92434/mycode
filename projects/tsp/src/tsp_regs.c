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

int set_pid(thread_arg_t *targ, uint32_t pid, bool pid_enable) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	*pbuffer = PID_INFO(pid_enable ? 1 : 0, pid);
	ret = write_regs(targ, ADDR_PID, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}

	return ret;
}

int get_pid(thread_arg_t *targ, uint32_t *pid, bool *pid_enable) {
	int ret = 0;
	uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	ret = read_regs(targ, ADDR_PID, sizeof(uint32_t));
	if (ret < 0) {
		printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		return ret;
	}
	*pid = *pbuffer & 0xffff;
	*pid_enable = (((*pbuffer >> 16) & 1) == 0) ? false : true;
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
	int i;
	//uint32_t *pbuffer = (uint32_t *)(targ->buffer);

	if(tx_size <= 0) {
		return ret;
	}

	for(i = 0; i < tx_size; i++) {
		switch(i) {
			case 0:
				targ->buffer[i] = 0x47;
				break;
			case 1:
				targ->buffer[i] = 0x1f;
				break;
			case 2:
				targ->buffer[i] = 0xff;
				break;
			case 3:
				targ->buffer[i] = 0xff;
				break;
			case 188:
				targ->buffer[i] = 0x47;
				break;
			case 189:
				targ->buffer[i] = 0x1f;
				break;
			case 190:
				targ->buffer[i] = 0xff;
				break;
			case 191:
				targ->buffer[i] = 0xff;
				break;
			default:
				targ->buffer[i] = 0;
				break;
		}
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
	int i;
	unsigned short pid1=0;
	if(targ==NULL||ts_buf==NULL||slot_idx>=MONITOR_SIZE)
		return -1;
	TRACE("tsp_monitor_read,pid:%d,slot_idx:%d\n",pid,slot_idx);
	select_slot(targ, slot_idx);
	select_pid_slot(targ, 0);
	set_pid(targ, pid, true);
	set_slot_enable(targ, true);
	for(i=0;i<200;i++){
		ret=read_ts_data(targ, PACK_BYTE_SIZE);
		if(ret==PACK_BYTE_SIZE){
			pid1=ts_get_pid(targ->buffer);
			if(pid1==pid)
				break;
		}
		cs_sleepms(5);
	}
	if(ret==PACK_BYTE_SIZE){
		dump_packet("monitor",targ->buffer,PACK_BYTE_SIZE);
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

int tsp_clear_single_slot(thread_arg_t *targ,unsigned short pid){
	int i,ret=0;
	for(i=MONITOR_SIZE;i<MONITOR_SIZE+REPLACER_SIZE;i++){//single
		if(pid==tsp_container->replacer_single[i].pid&&tsp_container->replacer_single[i].inuse==1){
			break;
		}
	}
	if(i>=MONITOR_SIZE+REPLACER_SIZE){
		TRACE("[tsp_clear_single_slot]can not find the slot for pid:(0x%0x)\n",pid);
		return -2;
	}
	else{
		memset(&(tsp_container->replacer_single[i]),0,sizeof(tsp_replacer_single));
		ret=select_slot(targ, i);
		ret|=select_pid_slot(targ, 0);
		ret|=set_pid(targ, pid, false);
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
		if(pid==tsp_container->replacer_single[i].pid&&tsp_container->replacer_single[i].inuse==1){
			if(memcmp(ts_buf,tsp_container->replacer_single[i].ts_pack,PACK_BYTE_SIZE)){
				memcpy(tsp_container->replacer_single[i].ts_pack,ts_buf,PACK_BYTE_SIZE);
				reset=1;
			}
			break;
		}
	}
	
	if(i>=MONITOR_SIZE+REPLACER_SIZE){
		for(i=MONITOR_SIZE;i<MONITOR_SIZE+REPLACER_SIZE;i++){
			if(tsp_container->replacer_single[i].inuse==0){
				tsp_container->replacer_single[i].inuse=1;
				tsp_container->replacer_single[i].pid=pid;
				tsp_container->replacer_single[i].slot_idx=i;
				memcpy(tsp_container->replacer_single[i].ts_pack,ts_buf,PACK_BYTE_SIZE);
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
		ret|=set_pid(targ, pid, 1);
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
			if(pid==tsp_container->replacer_dual[i].pid[j]&&tsp_container->replacer_dual[i].inuse==1){
				break;
			}
	}
	if(i>=TOTAL_SLOT_SIZE){
		TRACE("[tsp_clear_dual_slot]can not find the slot for pid:(0x%0x)\n",pid);
		return -2;
	}
	else{
		tsp_container->replacer_dual[i].pid[j]=0;
		ret=select_slot(targ, i);
		ret|=select_pid_slot(targ, j);
		ret|=set_pid(targ, pid, false);
	}
	return ret;
}

//notice:slot_idx is a global index
int tsp_clear_dual_slot(thread_arg_t *targ, int slot_idx){
	int ret=0;
	if(targ==NULL||slot_idx>=TOTAL_SLOT_SIZE||slot_idx<MONITOR_SIZE+REPLACER_SIZE)
		return -1;
	ret=select_slot(targ, slot_idx);
	ret|=set_slot_enable(targ, false);
	memset(&(tsp_container->replacer_dual[slot_idx]),0,sizeof(tsp_replacer_dual));
	return 0;
}

int tsp_replace_dual_tspack(thread_arg_t *targ,unsigned short *pid_array, int pid_num, unsigned char *ts_buf){
	int i,j,ret=0;
	int reset = 0;
	if(targ==NULL||ts_buf==NULL||pid_array==NULL||pid_num>DUAL_SLOT_PID_COUNT)
		return -1;
	for(i=MONITOR_SIZE+REPLACER_SIZE;i<TOTAL_SLOT_SIZE;i++){//dual
		if(memcmp(ts_buf,tsp_container->replacer_dual[i].ts_pack,2*PACK_BYTE_SIZE)==0){
			for(j=0;j<pid_num;j++)
				tsp_container->replacer_dual[i].pid[j]=pid_array[j];
			reset=1;
			break;
		}
	}
	
	if(i>=TOTAL_SLOT_SIZE){
		for(i=MONITOR_SIZE+REPLACER_SIZE;i<TOTAL_SLOT_SIZE;i++){
			if(tsp_container->replacer_dual[i].inuse==0){
				tsp_container->replacer_dual[i].inuse=1;
				tsp_container->replacer_dual[i].slot_idx=i;
				memcpy(tsp_container->replacer_dual[i].ts_pack,ts_buf,2*PACK_BYTE_SIZE);
				for(j=0;j<pid_num;j++)
					tsp_container->replacer_dual[i].pid[j]=pid_array[j];
				reset=1;
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
			ret|=set_pid(targ, pid_array[j], 1);
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
			if(tsp_container->replacer_dual[i].pid[j]==pid)//already exist
				return 1;
		}
	}
	for(i=MONITOR_SIZE+REPLACER_SIZE;i<TOTAL_SLOT_SIZE;i++){//dual
		for(j=0;j<DUAL_SLOT_PID_COUNT;j++){
			if(tsp_container->replacer_dual[i].pid[j]==0){
				ret=select_slot(targ, i);
				ret|=select_pid_slot(targ, j);
				ret|=set_pid(targ, pid, 1);
				ret|=set_slot_enable(targ, 1);
				tsp_container->replacer_dual[i].pid[j]=pid;
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
			case 0x0F:
			case 0x11:
			{
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
}

int init_tsp_reg(char *dev) {
	int ret = 0;
	int i;
	unsigned char ts_buf[188]={0};
	int read_bytes=0;
	
	int service_num=0;
	sid_t *sids=NULL;//[MAX_PROGRAM_NUM];
	thread_arg_t targ;
	
	//int mmap_size;
	//char *mmap_memory;

	

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
	//flags = fcntl(targ.fd, F_GETFL, 0);
	//fcntl(targ.fd, F_SETFL, flags | O_NONBLOCK);

	//ret = ioctl(targ.fd, PCIE_DEVICE_IOCTL_GET_LIST_BUFFER_SIZE, &mmap_size);
	//if (ret != 0) {
		//printf("[%s:%s:%d]:%s\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, strerror(errno));
		//return ret;
	//}
	//printf("mmap_size:%d(%x)\n", mmap_size, mmap_size);

	//mmap_memory = (char *)mmap(NULL, mmap_size, PROT_READ/* | PROT_WRITE*/, MAP_SHARED, targ.fd, (off_t)0);
	//if(mmap_memory == (void *)-1) {
	//	printf("xiaofei: %s:%d: %s\n", __PRETTY_FUNCTION__, __LINE__, strerror(errno));
	//	ret = -1;
	//	return ret;
	//}


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
	memset(&pmt_result,0,MAX_PROGRAM_NUM*sizeof(_pmt_result));
	init_dvb_process();
	while(1){
		read_bytes=tsp_monitor_read(&targ,0, 0, ts_buf);
		//dump_packet("monitor data",ts_buf,read_bytes);
		if(read_bytes==PACK_BYTE_SIZE){
			handle_psi_packet(ts_buf);
			service_num=get_pat_result(sids);
			
			if(service_num>0){
				for(i=0;i<service_num;i++){
					TRACE("[%d]pmt pid:%d\n",i,sids[i].i_pmt_pid);
					read_bytes=tsp_monitor_read(&targ,sids[i].i_pmt_pid, 0, ts_buf);
					if(read_bytes==PACK_BYTE_SIZE){
						handle_psi_packet(ts_buf);
						parse_pmt(ts_section(ts_buf),&pmt_result[i]);
					}
				}
			}
		}
		break;
	}
	destroy_dvb_process();
	//read_write(&targ);
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
