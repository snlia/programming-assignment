#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#define downcase(x) ((x) <= 'Z' && (x) >= 'A' ? (x) - 'A' + 'a' : (x))

enum {
	NOTYPE = 256, EQ, UEQ, NOT, NUMBER_D, NUMBER_H, PRE_MUL, PRE_PLUS, PRE_SUBTRACT

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"-", '-'},						// subtraction
	{"\\*", '*'},					// multiplication
	{"/", '/'},						// division
	{"0x[0-9abcdef]+", NUMBER_H},	// hexadecimal number
	{"[0-9]+", NUMBER_D},			// decimal number
	{"\\(", '('},					// left parentheses
	{"\\)", ')'},					// right parentheses
	{"==", EQ},						// equal
	{"!==", UEQ},					// unequal
	{"!", NOT}						// equal to zero
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED | REG_ICASE); // change here : use REG_ICASE option to be case insensitive
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret != 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;
				if (!i) continue; // will not record token "spaces"
				if (substr_len > 32) return false; //the maxize size of a token is 32
				tokens[nr_token].type = rule[i].token_type;
				strncpy (tokens[nr_token].str, substr_start, substr_len);
				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */
				if (!nr_token || (tokens[nr_token - 1].type != NUMBER_D && tokens[nr_token - 1].type != NUMBER_H && tokens[nr_token - 1].type != ')' ))
				switch(rules[i].token_type)
				{
					case '+' :
						tokens[nr_token] = PRE_PLUS;
						break;
					case '-' :
						tokens[nr_token] = PRE_SUBTRACT;
						break;
					case '*' :
						tokens[nr_token] = PRE_MUL;
						break;
					//default: panic("please implement me");
				}
				++nr_token;
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

int eval (int p, int q, bool *success)
{
	int val1, val2, ans, i, len;
	eval(p, q) 
	{
		if(p > q) 
		{
			puts ("Bad expression, please check it again.");
			success = 0;
			return 0;
		}
		else if(p == q) 
		{ 
			switch (tokens[p].type)
			{
				case NUMBER_D :
					sscanf (tokens[p].str, "%d", &ans);
					return ans;
				case NUMBER_H :
					len = strlen (tokens[p].str);
					for (i = 0; i < len; ++i) tokens[p].str[i] = downcase (tokens[p].str[i]);
					sscanf (tokens[p].str, "%x", &ans);

				default : puts ("Bad expression, you may miss a number.");
			}
			/* Single token.
			 * For now this token should be a number. 
			 * Return the value of the number.
			 */ 
		}
		else if(check_parentheses(p, q) == true) 
		{
			/* The expression is surrounded by a matched pair of parentheses. 
			 * If that is the case, just throw away the parentheses.
			 */
			return eval(p + 1, q - 1); 
		}
		else 
		{
			op = the position of dominant operator in the token expression;
			val1 = eval(p, op - 1, success);
			if (!success) return 0;
			val2 = eval(op + 1, q, success);
			if (!success) return 0;

			switch(op_type) 
			{
				case '+': return val1 + val2;
				case '-': return val1 + val2;
				case '*': return val1 * val2;
				case '/': return val1 / val2;
				default: assert(0);
			}
		}
	}
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) 
	{
		*success = 0;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	//	panic("please implement me");
	return eval(0, nr_token - 1, success);
}

