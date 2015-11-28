#include "monitor/monitor.h"
#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

void cpu_exec(uint32_t);//exec x instructions given in the first argument

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
	{ "d", "d [N] : delete the watchpoint with index N, or, without N, delete the last watchpoint.", cmd_d}
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
			printf ("%s : 0x%x\n", regsl[i], reg_l (i));
		printf ("eip : 0x%x\n", cpu.eip);
		printf ("eflags : 0x%x\n", cpu.eflags);
		printf ("VM : %x\n", cpu.VM);
		printf ("RF : %x\n", cpu.RF);
		printf ("NT : %x\n", cpu.NT);
		printf ("IOPL : %x\n", cpu.IOPL);
		printf ("OF : %x\n", cpu.OF);
		printf ("DF : %x\n", cpu.DF);
		printf ("IF : %x\n", cpu.IF);
		printf ("TF : %x\n", cpu.TF);
		printf ("SF : %x\n", cpu.SF);
		printf ("ZF : %x\n", cpu.ZF);
		printf ("AF : %x\n", cpu.AF);
		printf ("PF : %x\n", cpu.PF);
		printf ("CF : %x\n", cpu.CF);

	}
	else if (!strcmp (arg, "w")) 
		pt_wp ();
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
	int32_t i, j;
	if (arg == NULL) 
	{
		puts ("Missing arguments."); 
		return 0;
	}
	if (!sscanf (arg, "%d", &i))
	{
		puts ("The first argument should be a number.");
		return 0;
	}
	bool flag = 1;
	int adress = expr (args + strlen (arg) + 1, &flag);
	if (!flag) 
	{
		puts ("Something wrong with expression, please check it.");
		return 0;
	}
	int32_t head = min (adress, adress + i * 4), tail = max (adress, adress + i * 4);
	printf ("0x%x :", head);
	for (j = head; j < tail; j += 4)
	{
		printf ("0x%010x	", swaddr_read (j, 4));
		if (!((j - head) % 16)) printf ("\n0x%x :", j + 4);
	}
	if (tail % 16) puts ("");
	return 0;
}

static int cmd_w (char *args)
{
	/* extract the first argument */
	if (args == NULL) 
	{
		puts ("Missing arguments."); 
		return 0;
	}
	bool flag = 1;
	int value = expr (args, &flag);
	if (!flag) 
	{
		puts ("Something wrong with expression, please check it.");
		return 0;
	}
	printf ("Succes with index %d, value 0x%x\n", new_wp (args, value), value);
	return 0;
}

static int cmd_d (char *args)
{
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;
	if (arg == NULL) i = -1; /* no argument given */
	else 
		if (!sscanf (arg, "%u", &i))
		{
			printf("Invalid input --- %s\n", cmd_table[9].description);
			return 0;
		}
	free_wp (i);
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
