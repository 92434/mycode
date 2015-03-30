/*
 * Simple TFTP program.
 *  tpu.
 */

#include "mboot.h"
#include "net.h"


int tftp_receive(char *dlbuf, int dlsize, char *filename)
{
    int i, len, hlen;
    int next_block, last_block, current_block; 
    int block, size;
    int getip, retv;
    char buf[32];
    TFTP_t t_ack, *t_data;
   
    t_data = (TFTP_t *)(packet_buf+14+20+8);

    /* build tftp request packet */
    buf[0] = 0;
    buf[1] = 1;
    hlen = strlen(filename);
    memcpy(buf+2, filename, hlen);
    buf[hlen+2] = 0;
    hlen += 3;
    memcpy(buf+hlen, "octet", 5);
    buf[hlen+5] = 0;
    hlen += 6;

    eth_init(local_hwaddr);
    arp_req();

    server_port = 0x0045;
    local_port = 0x5555;
    
    udp_send(buf, hlen);
    block = 0;
    next_block = 1;
    last_block = 1;
    size = 0;
    while(1){
        len = udp_recv(1000);
		if(len==0){
			if(block==0){
				printk("TFTP request timeout! Restart ...\n");
			    udp_send(buf, hlen);
			}else{
				printk("TFTP packet %08x timeout ...\n", block);
			}
			continue;
		}
        if(t_data->type==SWAP16(0x0005)){
            printk(" tftp error %04x: %s\n", SWAP16(t_data->block), t_data->data);
            return 0;
        }
        if(block==0){
            printk(" tftp: start download %s ...\n", filename);
        }

        current_block = SWAP16(t_data->block);
        if(current_block==next_block){
            len -= 4;
            memcpy(dlbuf, t_data->data, len);
            dlbuf += len;
            size += len;
            next_block++;
            next_block &= 0xffff;
        }else if(current_block==last_block){
            len -= 4;
            block--;
            printk("\n tftp: dummy block: %08x(%08x)\n", current_block, next_block);
        }else{
            printk("\n tftp: wrong block: %08x(%08x)\n", current_block, next_block);
            continue;
        }

        last_block = current_block;
        t_ack.type = SWAP16(0x0004);
        t_ack.block = t_data->block;
        udp_send(&t_ack, 4);
        if(!(block&0xff))
            printk("\r  %08x", block);
        block++;

        if(len!=512){
            printk("\n");
            printk(" tftp: end download: %d block\n", block);
            break;
        }
    }

    return size;
}

