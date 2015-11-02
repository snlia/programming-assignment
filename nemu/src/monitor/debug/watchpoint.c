#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

int new_wp (char *s, uint32_t value)
{
	if (free_ == NULL) {assert(0); return -1;}
	WP *p = free_->next;
	free_->next = head;
	free_->value = value;
	memcpy (free_->str, s, strlen (s));
	head = free_;
	free_ = p;
	return head->NO;
}

void pt_wp ()
{
	WP *p = head;
	for (; p; p = p->next)
		printf ("Index : %d, expr : %s, value :0x%x\n", p->NO, p->str, p->value);
}


/* TODO: Implement the functionality of watchpoint */


