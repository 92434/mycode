#ifndef _TSP_REG_H_
#define _TSP_REG_H_
#include "utils.h"
#define	PACK_BYTE_SIZE 188
#define	PACK_WORD_SIZE (PACK_BYTE_SIZE / sizeof(uint32_t))

typedef enum {
	ADDR_INDEX = 0,
	ADDR_PID_INDEX,
	ADDR_PID,
	ADDR_MATCH_ENABLE,
	ADDR_READ_REQUEST,
	ADDR_MATCHED_COUNT,
	ADDR_TS_DATA_BASE = 128,
	ADDR_TS_DATA_END = ADDR_TS_DATA_BASE + PACK_WORD_SIZE * 2,
} addr_t;

#define ADDR_OFFSET(addr) (addr * 4)
#define PID_INFO(ENABLE, PID) (((ENABLE == 0 ? 0 : 1) << 16)/*match enable*/ + PID)

#define TSP_BUFSIZE (PACK_BYTE_SIZE * 2)

#define MONITOR_SIZE 1
#define REPLACER_SIZE 8
#define COMMON_REPLACER_SIZE 1
#define TOTAL_SLOT_SIZE (MONITOR_SIZE+REPLACER_SIZE+COMMON_REPLACER_SIZE)

#define DUAL_SLOT_PID_COUNT 16
typedef struct{
	unsigned char	inuse:1;
	unsigned char	slot_idx:7;
	unsigned short	pid;	//PID range [0-65535]
	unsigned char	ts_pack[2*PACK_BYTE_SIZE];
}tsp_monitor;

typedef struct{
	unsigned char	inuse:1;
	unsigned char	slot_idx:7;
	unsigned short	pid;	//PID range [0-65535]
	unsigned char	ts_pack[PACK_BYTE_SIZE];
}tsp_replacer_single;

typedef struct{
	unsigned char	inuse:1;
	unsigned char	slot_idx:7;
	unsigned short	pid[DUAL_SLOT_PID_COUNT];//PID range [1-65535],can not be zero
	unsigned char	ts_pack[2*PACK_BYTE_SIZE];
}tsp_replacer_dual;

typedef struct{
	tsp_monitor monitor[MONITOR_SIZE];
	tsp_replacer_single replacer_single[REPLACER_SIZE];
	tsp_replacer_dual replacer_dual[COMMON_REPLACER_SIZE];
}_tsp_container;


int init_tsp_reg(char *dev);
int tsp_monitor_read(thread_arg_t *targ,unsigned short pid, int slot_idx, unsigned char *ts_buf);
int tsp_monitor_stop(thread_arg_t *targ, int slot_idx);
int tsp_clear_single_slot(thread_arg_t *targ,unsigned short pid);
int tsp_replace_single_tspack(thread_arg_t *targ,unsigned short pid, unsigned char *ts_buf);
int tsp_clear_dualslot_by_pid(thread_arg_t *targ,unsigned short pid);
int tsp_clear_dual_slot(thread_arg_t *targ, int slot_idx);
int tsp_replace_dual_tspack(thread_arg_t *targ,unsigned short *pid_array, int pid_num, unsigned char *ts_buf);
int tsp_add_dual_slot_pid(thread_arg_t *targ,unsigned short pid);
#endif
