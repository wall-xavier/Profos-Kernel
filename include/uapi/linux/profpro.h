#ifndef _UAPI_PROFPRO_H
#define _UAPI_PROFPRO_H

#include <linux/types.h>
#include <linux/socket.h>

#define ETH_P_PROFPRO 0x88B5

#define PROFPRO_ADDR_LEN 4

struct profpro_addr {
	__be16 network;
	__be16 host;
};

struct profpro_hdr {
	__u8 version;
	__u8 ttl;
	__b16 length;
	struct profpro_addr src;
	struct profpro_addr dst;
	__be16 checksum;
};

struct sockaddr_profpro {
	__kernel_sa_family_t sa_familt;
	__be32 pp_addr;
	__u16 pp_port;
	unsigned char __date[8];
};

#endif
