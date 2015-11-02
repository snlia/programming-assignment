#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c (char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q (char *args) {
	return -1;
}

static int cmd_help (char *args);

static int cmd_si (char *args);

static int cmd_info (char *args);

static int cmd_p (char *args);

static int cmd_x (char *args);

static int cmd_w (char *args);

static int cmd_pw (char *args);

static int cmd_d (char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "si [N] : Execute N instructs and pause, the default value of N is 1; N should be an positive 32-bit number", cmd_si},
	{ "info", "info SUBCMD : r Print the informations about regs; w Print the informations about watchpoints", cmd_info},
	{ "p", "p EXPR : Show the value of EXPR", cmd_p},
	{ "x", "x N EXPR : Show 4N continious bytes begins at EXPR", cmd_x},
	{ "w", "w EXPR : Seting watchpoint on EXPR", cmd_w},
	{ "pw", "Print all informations about watchpoints", cmd_pw},
	{ "d", "d [N] : delete the watchpoint with index N, or, without N, delete the watchpoint with biggist index", cmd_d}
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

static int cmd_si (char *args)
{
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	uint32_t i;
	if (arg == NULL) i = 1; /* no argument given */
	else 
		if (!sscanf (arg, "%u", &i))
		{
			printf("Invalid input --- %s\n", cmd_table[3].description);
			return 0;
		}
	cpu_exec (i);
	return 0;
}

static int cmd_info (char *args)
{
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;
	if (!strcmp(arg, "r"))
	{
		for(i = R_EAX; i <= R_EDI; i ++) 
			printf ("%s : %x\n", regsl[i], reg_l (i));
		printf ("eip : %x\n", cpu.eip);
	}
	else if (!strcmp (arg, "w")) ;//will finish in stage 3
	return 0;
}

static int cmd_p (char *args)
{
	bool success = 1;
	int val = expr (args, &success);
	if (success) printf ("int : %-15dunsigned int : %-30uhexadecimal : 0x%-45x\n", val, val, val);
	return 0;
}

static int cmd_x (char *args)
{
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	uint32_t i, j;
	if (arg == NULL) 
	{
		puts ("Missing arguments."); 
		return 0;
	}
	if (!sscanf (arg, "%u", &i))
	{
		puts ("The first argument should be a number.");
		return 0;
	}
	bool flag = 1;
	int adress = expr (args + strlen (arg) + 1, &flag);
	printf ("%s", args + strlen (arg) + 1);
	if (!flag) 
	{
		puts ("Something wrong with expression, please check it.");
		return 0;
	}
	for (j = 0; j < i; ++j)
	{
		printf ("0x%-10d	", swaddr_read (adress + j * 4, 4));
		if (!((j + 1) % 5)) puts ("");
	}
	if (i % 5) puts ("");
	return 0;
}

static int cmd_w (char *args)
{
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
//	uint32_t i, j;
	if (arg == NULL) 
	{
		puts ("Missing arguments."); 
		return 0;
	}
	bool flag = 1;
	//int adress = expr (args + strlen (arg) + 1, &flag);
	if (!flag) 
	{
		puts ("Something wrong with expression, please check it.");
		return 0;
	}
	//new_wp ();
	return 0;
}

static int cmd_pw (char *args)
{
	return 0;
}

static int cmd_d (char *args)
{
	return 0;
}


void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
