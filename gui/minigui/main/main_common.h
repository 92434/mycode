#ifndef _MAIN_COMMON_H
#define _MAIN_COMMON_H
typedef struct {
	int code;
	char *des;
} msg_info_t;

typedef struct {
	int nc;
	char *des;
} button_msg_info_t;


char *trans_msg_code(int message);
char *trans_button_msg_code(int nc);
#endif
