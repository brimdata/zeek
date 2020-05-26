#pragma once

#include <stdint.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RES_INIT 0

#define dn_expand(a, b, c, d, e) (-1)
#define res_init() (-1)
#define res_mkquery(a, b, c, d, e, f, g, h, i) (-1)

struct __res_state {
	struct sockaddr_in nsaddr_list[1];
	int nscount;
	int options;
};

static struct __res_state _res;

#ifdef __cplusplus
extern }
#endif

