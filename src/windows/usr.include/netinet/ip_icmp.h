#pragma once

#include <stdint.h>

#define ICMP_ECHOREPLY 0
#define ICMP_ECHO      8

#define ICMP_MINLEN 8

#define ICMP_UNREACH       3
#define ICMP_ROUTERADVERT  9
#define ICMP_ROUTERSOLICIT 10
#define ICMP_TIMXCEED      11
#define ICMP_TSTAMP        13
#define ICMP_TSTAMPREPLY   14
#define ICMP_IREQ          15
#define ICMP_IREQREPLY     16
#define ICMP_MASKREQ       17
#define ICMP_MASKREPLY     18

struct icmphdr {
	uint8_t  type;
	uint8_t  code;
	uint16_t checksum;
	union {
		struct {
			uint16_t id;
			uint16_t sequence;
		} echo;
		uint32_t gateway;
		struct {
			uint16_t __unused;
			uint16_t mtu;
		} frag;
		uint8_t reserved[4];
	} un;
};

struct icmp_ra_addr {
	uint32_t ira_addr;
	uint32_t ira_preference;
};

#define icmp_num_addrs icmp_hun.ih_rtradv.irt_num_addrs
#define icmp_wpa       icmp_hun.ih_rtradv.irt_wpa
#define icmp_lifetime  icmp_hun.ih_rtradv.irt_lifetime

struct icmp {
	uint8_t  icmp_type;
	uint8_t  icmp_code;
	uint16_t icmp_cksum;
	union {
		uint8_t ih_pptr;
		struct in_addr ih_gwaddr;
		struct ih_idseq {
			uint16_t icd_id;
			uint16_t icd_seq;
		} ih_idseq;
		uint32_t ih_void;
		struct ih_pmtu {
			uint16_t ipm_void;
			uint16_t ipm_nextmtu;
		} ih_pmtu;
		struct ih_rtradv {
			uint8_t  irt_num_addrs;
			uint8_t  irt_wpa;
			uint16_t irt_lifetime;
		} ih_rtradv;
	} icmp_hun;
	union {
		struct {
			uint32_t its_otime;
			uint32_t its_rtime;
			uint32_t its_ttime;
		} id_ts;
		struct {
			struct ip idi_ip;
		} id_ip;
		struct icmp_ra_addr id_radv;
		uint32_t id_mask;
		uint8_t  id_data[1];
	} icmp_dun;
};
