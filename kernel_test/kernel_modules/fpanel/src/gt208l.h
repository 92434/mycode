#ifndef __GT208L_H__
#define __GT208L_H__

int gt208l_init(void);
void gt208l_exit(void);
u8 gt208l_read_key(void);
void gt280l_set_led(u8 i, int on);
#endif