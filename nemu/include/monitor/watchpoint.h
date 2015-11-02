#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	uint32_t value;
	struct watchpoint *next;
	char str [255];

	/* TODO: Add more members if necessary */


} WP;

int new_wp (char *s, uint32_t value);
#endif
