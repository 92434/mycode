#ifndef _DVB_PRO_SI_H_
#define _DVB_PRO_SI_H_
#include <unistd.h>
#include <bitstream/mpeg/psi.h>

#define MAX_PROGRAM_NUM 64
typedef struct sid_t {
    uint16_t i_sid, i_pmt_pid;
    uint8_t *p_current_pmt;
    PSI_TABLE_DECLARE(pp_eit_sections);
} sid_t;

int init_dvb_process(void);
void handle_psi_packet(uint8_t *p_ts);
int get_pat_result(sid_t *pp_sids_out);
#endif