
#ifndef __NET_H__
#define __NET_H__


/* Byte swapping stuff */
#ifdef _BIG_ENDIAN
#define SWAP16(x)   (x)
#define SWAP32(x)   (x)
#else
#define SWAP16(x)   ((((x) & 0xff) << 8) | ((x) >> 8))
#define SWAP32(x)       ( \
                        (((x) >> 24) & 0x000000ff) | \
                        (((x) >>  8) & 0x0000ff00) | \
                        (((x) <<  8) & 0x00ff0000) | \
                        (((x) << 24) & 0xff000000) )
#endif


/* The number of receive packet buffers, and the required packet buffer alignment in memory. */

#define PKTBUFSRX	4
#define PKTALIGN	32

extern void eth_reset(void);
extern int  eth_init(u8 *bis);
extern int  eth_send(void *packet, int length);
extern int  eth_rx(void *buf);
extern void eth_halt(void);

/**********************************************************************/
/*
 *	Protocol headers.
 */

/* Ethernet header */
typedef struct {
	u8		et_dest[6];	/* Destination node     */
	u8		et_src[6];	/* Source node          */
	u16		et_protlen;	/* Protocol or length   */
	u8		et_dsap;	/* 802 DSAP             */
	u8		et_ssap;	/* 802 SSAP             */
	u8		et_ctl;		/* 802 control          */
	u8		et_snap1;	/* SNAP                 */
	u8		et_snap2;
	u8		et_snap3;
	u16		et_prot;	/* 802 protocol         */
} Ethernet_t;

#define ETHER_HDR_SIZE	14		/* Ethernet header size */
#define E802_HDR_SIZE	22		/* 802 header size      */
#define PROT_IP		0x0800		/* IP protocol          */
#define PROT_ARP	0x0806		/* IP ARP protocol      */
#define PROT_RARP	0x8035		/* IP ARP protocol      */

#define IPPROTO_ICMP     1      /* ICMP */
#define IPPROTO_UDP     17      /* UDP  */

/* Internet Protocol (IP) header. */
typedef struct {
	u8		ip_hl_v;	/* header length and version*/
	u8		ip_tos;		/* type of service          */
	u16		ip_len;		/* total length             */
	u16		ip_id;		/* identification           */
	u16		ip_off;		/* fragment offset field    */
	u8		ip_ttl;		/* time to live             */
	u8		ip_prot;	/* protocol                 */
	u16		ip_sum;		/* checksum                 */
	u32		ip_src;		/* Source IP address        */
	u32		ip_dst;		/* Destination IP address   */
	u16		udp_src;	/* UDP source port          */
	u16		udp_dst;	/* UDP destination port     */
	u16		udp_len;	/* Length of UDP packet     */
	u16		udp_xsum;	/* Checksum                 */
    u8      udp_data[0];
} IP_t;

#define IP_HDR_SIZE_NO_UDP (sizeof (IP_t) - 8)
#define IP_HDR_SIZE        (sizeof (IP_t))


/* Address Resolution Protocol (ARP) header. */
typedef struct
{
	u16		ar_hrd;		/* Format of hardware address   */
	u16		ar_pro;		/* Format of protocol address   */
	u8		ar_hln;		/* Length of hardware address   */
	u8		ar_pln;		/* Length of protocol address   */
	u16		ar_op;		/* Operation                    */

	u8		ar_sha[6];	/* Sender hardware address      */
	u8		ar_spa[4];	/* Sender protocol address      */
	u8		ar_tha[6];	/* Target hardware address      */
	u8		ar_tpa[4];	/* Target protocol address      */
} ARP_t;

#   define ARP_ETHER	    1		/* Ethernet  hardware address	*/
#   define ARPOP_REQUEST    1		/* Request  to resolve  address	*/
#   define ARPOP_REPLY	    2		/* Response to previous request	*/
#   define RARPOP_REQUEST   3		/* Request  to resolve  address	*/
#   define RARPOP_REPLY	    4		/* Response to previous request */

#define ARP_HDR_SIZE	(8+20)		/* Size assuming ethernet	*/

#define ARP_PKT 0x0806
#define IP_PKT 0x0800

typedef struct
{
    u16      type;
    u16      block;
    u8      data[0];
}TFTP_t;

/*
 * Maximum packet size; used to allocate packet storage.
 * TFTP packets can be 524 bytes + IP header + ethernet header.
 * Lets be conservative, and go for 38 * 16.  (Must also be
 * a multiple of 32 bytes).
 */
/*
 * AS.HARNOIS : Better to set PKTSIZE to maximum size because
 * traffic type is not always controlled
 * maximum packet size =  1518
 * maximum packet size and multiple of 32 bytes =  1536
 */
#define PKTSIZE			1518
#define PKTSIZE_ALIGN		1536




/*********************************************************************/

extern u8 packet_buf[2048];
extern u8 tx_buf[2048];
extern u8 local_hwaddr[8];
extern u8 server_hwaddr[8];
extern int local_port;
extern int server_port;

/* local ip: 10.98.95.15 */
extern int local_ipaddr;
/* server ip: 10.98.95.95 */
extern int server_ipaddr;


int arp_req(void);
void arp_ack(void);

int udp_recv(int timeout_ms);
int udp_send(char *buf, int len);


/**********************************************************************/

#endif /* __NET_H__ */

