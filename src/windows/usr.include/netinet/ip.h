#pragma once

#include <inaddr.h>
#include <stdint.h>

struct ip {
	// Use uint8_t instead of unsigned for ip_hl and ip_v so they
	// occupy one byte instead of four on Windows.  (MSVC allocates
	// space sufficient for the bit field's type rather than its
	// width.  This corresponds to GCC's -mms-bitfields option,
	// which is enabled by default on Windows.  On other platforms,
	// GCC allocates space sufficient for the bit field's width
	// regardless of its type.)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	uint8_t ip_hl:4;
	uint8_t ip_v:4;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	uint8_t ip_v:4;
	uint8_t ip_hl:4;
#else
#error Unknown byte order.
#endif
	uint8_t  ip_tos;
	uint16_t ip_len;
	uint16_t ip_id;
	uint16_t ip_off;
	uint8_t  ip_ttl;
	uint8_t  ip_p;
	uint16_t ip_sum;
	struct in_addr ip_src, ip_dst;
};
