#pragma once

#include <stdint.h>

#define ARPHRD_ETHER 1

struct arphdr {
	uint16_t ar_hrd;
	uint16_t ar_pro;
	uint8_t  ar_hln;
	uint8_t  ar_pln;
	uint16_t ar_op;
};
