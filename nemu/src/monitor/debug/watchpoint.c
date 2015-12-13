#include "monitor/expr.h"
#include "monitor/watchpoint.h"

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
	strcpy (free_->str, s);
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

void free_wp (int Index)
{
	WP *p;
	if (head == NULL) {puts ("No watchpoint exist."); return ;}
	if (!~Index || head->NO == Index) 
	{
		p = head->next;
		head->next = free_;
		free_ = head;
		head = p;
		return ;
	}
	p = head;
	bool flag = 0;
	for (;p->next; p = p->next)
		if (p->next->NO == Index)
		{
			flag = 1;
			WP *pp = p->next->next;
			p->next->next = free_;
			free_ = p->next;
			p->next = pp;
			return ;
		}
	if (!flag) printf ("watchpoint with Index %d is not exist.", Index);
}

bool ck_wp ()
{
	WP *p = head;
	bool flag = 0;
	for (; p; p = p->next)
	{
		bool success = 1;
		uint32_t V = expr (p->str, &success);
		if (V != p->value || !success) 
		{
			printf ("Watchpoint %d: %s\n\n", p->NO, p->str);
			printf ("Old value = 0x%08x\n", p->value);
			printf ("New value = 0x%08x\n", V);
			p->value = V;
			flag = 1;
		}
	}
	return flag;
}


/* TODO: Implement the functionality of watchpoint */


