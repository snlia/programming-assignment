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

/*return the index of new watchpoint with expression s and value*/
int new_wp (char *s, uint32_t value);

/*print the informations about applied watchpoint*/
void pt_wp (); 

/*delete the watchpoint with index Index, Index set -1 means delete lastest watchpoint*/
void free_wp (int Index); 

/*check if the value of watchpoint change, return true means change*/
bool ck_wp (); 
#endif
