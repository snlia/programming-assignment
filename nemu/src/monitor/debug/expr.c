#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#define downcase(x) ((x) <= 'Z' && (x) >= 'A' ? (x) - 'A' + 'a' : (x))
#define check_parentheses(l, r) (tokens[l].type == '(' && tokens[r].type == ')')

enum {
	NOTYPE = 256, EQ, UEQ, NOT, PRE_MUL, PRE_PLUS, PRE_SUBTRACT, AND, OR, SHL, SHR, LEQ, REQ,
	NUMBER_D, NUMBER_H, 
	EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI, EIP,
	AX, CX, DX, BX, SP, BP, SI, DI,
	AL, CL, DL, BL, AH, CH, DH, BH
		/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	//(flod) Caculate operator
	{" +",	NOTYPE},					// spaces
	{"\\+", '+'},						// plus
	{"\\-", '-'},						// subtraction
	{"\\*", '*'},						// multiplication
	{"\\%", '%'},						// mod
	{"\\/", '/'},						// division
	{"&&", AND},						// and
	{"\\|\\|", OR},						// or
	{"\\&", '&'},						// &
	{"\\|", '|'},						// or
	{"\\^", '^'},						// xor
	{"\\~", '~'},						// not
	{"<<", SHL},						// shift_left
	{">>", SHR},						// shift_right
	//(flod end)

	//(flod) Number (decimal & hexadecimal)
	{"0x[0-9abcdef]+", NUMBER_H},		// hexadecimal number
	{"[0-9]+", NUMBER_D},				// decimal number
	//(flod end)

	//(flod) Parentheses
	{"\\(", '('},						// left parentheses
	{"\\)", ')'},						// right parentheses
	//(flod end)

	//(flod) Judging operator
	{"==", EQ},							// equal
	{"!=", UEQ},						// unequal
	{"!", NOT},							// equal to zero
	{"<=", LEQ},						// leq
	{">=", REQ},						// req
	{"<", '<'},							// <
	{">", '>'},							// >
	//(flod end)

	//(flod) 32 bit Register
	{"\\$eax", EAX},					//eax
	{"\\$ecx", ECX},					//ecx
	{"\\$edx", EDX},					//edx
	{"\\$ebx", EBX},					//ebx
	{"\\$esp", ESP},					//esp
	{"\\$ebp", EBP},					//ebp
	{"\\$esi", ESI},					//esi
	{"\\$edi", EDI},					//edi
	//(flod end)

	{"\\$eip", EIP},					//eip

	//(flod) 16 bit Register
	{"\\$ax", AX},						//ax
	{"\\$cx", CX},						//cx
	{"\\$dx", DX},						//dx
	{"\\$bx", BX},						//bx
	{"\\$sp", SP},						//sp
	{"\\$bp", BP},						//bp
	{"\\$si", SI},						//si
	{"\\$di", DI},						//di
	//(flod end)

	//(flod) 8 bit Register
	{"\\$al", AL},						//al
	{"\\$cl", CL},						//cl
	{"\\$dl", DL},						//dl
	{"\\$bl", BL},						//bl
	{"\\$ah", AH},						//ah
	{"\\$ch", CH},						//ch
	{"\\$dh", DH},						//dh
	{"\\$bh", BH},						//bh
	//(flod end)

	//Add more Register here
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

int Domination [1024];
int Stack_op [1024];
int Parentheses [1024];
int match [1024];
int Stack_top = 0;

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	//Init Domination
	memset (Domination, 0, sizeof (Domination));
	Domination['('] = Domination[')'] = 0;
	Domination[PRE_MUL] = Domination[PRE_PLUS] = Domination[PRE_SUBTRACT] = Domination['~'] = 1;
	Domination['*'] = Domination['/'] = Domination['%'] = 2;
	Domination['+'] = Domination['-'] = 3;
	Domination[SHL] = Domination[SHR] = 4;
	Domination['<'] = Domination['>'] = Domination[LEQ] = Domination[REQ] = 5;
	Domination[UEQ] = Domination[EQ] = 6;
	Domination['&'] = 7;
	Domination['^'] = 8;
	Domination['|'] = 9;
	Domination[AND] = 10;
	Domination[OR] = 11;
	//Compile Regex
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

//				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;
				if (!i) continue; // will not record token "spaces"
				if (substr_len > 32) return false; //the maxize size of a token is 32
				tokens[nr_token].type = rules[i].token_type;
				strncpy (tokens[nr_token].str, substr_start, substr_len);
				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */
				if (!nr_token || (tokens[nr_token - 1].type != ')' && tokens[nr_token - 1].type < NUMBER_D))
					switch(rules[i].token_type)
					{
						case '+' :
							tokens[nr_token].type = PRE_PLUS;
							break;
						case '-' :
							tokens[nr_token].type = PRE_SUBTRACT;
							break;
						case '*' :
							tokens[nr_token].type = PRE_MUL;
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

int get_dominant (int l, int r)
{
	int i, maxp = l;
	for (i = l; i <= r; ++i)
	{
		if (tokens[i].type == '(') i = match[i]; 
		else 
			if (Domination[tokens[i].type] > Domination[tokens[maxp].type]) maxp = i;
	}
	return maxp;
}

uint32_t eval (int p, int q, bool *success)
{
	if (!(*success)) return 0;
	uint32_t val1 = 0, val2, ans, i, len;
	if(p > q) 
	{
		puts ("Bad expression, please check it.");
		*success = false;
		return 0;
	}
	else if(p == q) 
	{ 
		if (tokens[p].type == EIP) return cpu.eip;
		if (tokens[p].type >= EAX && tokens[p].type <= EDI) return reg_l (tokens[p].type - EAX);
		if (tokens[p].type >= AX && tokens[p].type <= DI) return reg_w (tokens[p].type - AX);
		if (tokens[p].type >= AL && tokens[p].type <= BH) return reg_b (tokens[p].type - AL);
		switch (tokens[p].type)
		{
			case NUMBER_D :
				sscanf (tokens[p].str, "%d", &ans);
				return ans;
			case NUMBER_H :
				len = strlen (tokens[p].str);
				for (i = 0; i < len; ++i) tokens[p].str[i] = downcase (tokens[p].str[i]);
				sscanf (tokens[p].str, "%x", &ans);
				return ans;
				//Adress and Register here
			default : *success = false;
					  puts ("Error : you may miss a number.");
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
		uint32_t ans = eval(p + 1, q - 1, success); 
		Stack_op[Stack_top++] = '(';
		return ans;
	}
	else 
	{
		int op = get_dominant (p, q);
		if (op != p) val1 = eval (p, op - 1, success);
		if (!(*success)) return 0;
		val2 = eval (op + 1, q, success);
		if (!(*success)) return 0;

		Stack_op[Stack_top++] = tokens[op].type;

		if (op == p) 
		{
			switch (tokens[op].type)
			{
				case '~': return ~val2;
				case '!': return !val2;
				case PRE_PLUS : return val2;
				case PRE_SUBTRACT : return -val2;
				case PRE_MUL : return swaddr_read(val2, 4);
				default : *success = false; puts ("Error : you may miss a number."); return 0;
			}
		}

		switch(tokens[op].type) 
		{
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': 
					  {
						  if (val2)
							  return val1 / val2;
						  else 
						  {
							  *success = false; 
							  puts ("Error : division by zero.");
							  return 0;
						  }
					  }
			case '&': return val1 & val2;
			case '%': 
					  {
						  if (val2)
							  return val1 % val2;
						  else 
						  {
							  *success = false; 
							  puts ("Error : modular by zero.");
							  return 0;
						  }
					  }
			case '|': return val1 | val2;
			case '^': return val1 ^ val2;
			case '<': return val1 < val2;
			case '>': return val1 > val2;
			case EQ:  return val1 == val2;
			case UEQ: return val1 != val2;
			case LEQ: return val1 <= val2;
			case REQ: return val1 >= val2;
			case SHL : return val1 << val2;
			case SHR : return val1 >> val2;
			default: *success = false; return 0;
		}
	}
	return 0;
}

void warning ()
{
	int i; bool flag = 0;
	for (i = 1; i < Stack_top; ++i) 
	{
		if (Stack_op[i] == '+' && Stack_op[i - 1] == SHL) flag = 1;
		if (Stack_op[i - 1] == '+' && Stack_op[i] == SHL) flag = 1;
	}
	if (flag) puts ("warning : suggest parentheses around ‘+’ inside ‘<<’");
	flag = 0;
	for (i = 1; i < Stack_top; ++i) 
	{
		if (Stack_op[i] == '-' && Stack_op[i - 1] == SHL) flag = 1;
		if (Stack_op[i - 1] == '-' && Stack_op[i] == SHL) flag = 1;
	}
	if (flag) puts ("warning : suggest parentheses around ‘-’ inside ‘<<’");
	flag = 0;
	for (i = 1; i < Stack_top; ++i) 
	{
		if (Stack_op[i] == '+' && Stack_op[i - 1] == SHR) flag = 1;
		if (Stack_op[i - 1] == '+' && Stack_op[i] == SHR) flag = 1;
	}
	if (flag) puts ("warning : suggest parentheses around ‘+’ inside ‘>>’");
	flag = 0;
	for (i = 1; i < Stack_top; ++i) 
	{
		if (Stack_op[i] == '-' && Stack_op[i - 1] == SHR) flag = 1;
		if (Stack_op[i - 1] == '-' && Stack_op[i] == SHR) flag = 1;
	}
	if (flag) puts ("warning : suggest parentheses around ‘-’ inside ‘>>’");
	flag = 0;
	for (i = 1; i < Stack_top; ++i)
		flag = ((Stack_op[i] == '<' || Stack_op[i] == '>' || Stack_op[i] == LEQ || Stack_op[i] == REQ)
				&&(Stack_op[i - 1] == '<' || Stack_op[i - 1] == '>' || Stack_op[i - 1] == LEQ || Stack_op[i - 1] == REQ));
	if (flag) puts ("warning : comparisons like ‘X<=Y<=Z’ do not have their mathematical meaning");
	flag = 0;
	for (i = 1; i < Stack_top; ++i)
	{
		bool flag_x = (Stack_op[i] == UEQ || Stack_op[i] == EQ || Stack_op[i] == '<' || Stack_op[i] == '>' || Stack_op[i] == LEQ || Stack_op[i] == REQ);
		bool flag_y = (Stack_op[i - 1] == UEQ || Stack_op[i - 1] == EQ || Stack_op[i - 1] == '<' || Stack_op[i - 1] == '>' || Stack_op[i - 1] == LEQ || Stack_op[i - 1] == REQ);
		flag = ((flag_x && Stack_op[i - 1] == EQ) || (flag_y && Stack_op[i] == EQ));
	}
	if (flag) puts ("warning: suggest parentheses around comparison in operand of ‘==’");
	flag = 0;
	for (i = 1; i < Stack_top; ++i)
	{
		bool flag_x = (Stack_op[i] == UEQ || Stack_op[i] == '<' || Stack_op[i] == '>' || Stack_op[i] == LEQ || Stack_op[i] == REQ);
		bool flag_y = (Stack_op[i - 1] == UEQ || Stack_op[i - 1] == '<' || Stack_op[i - 1] == '>' || Stack_op[i - 1] == LEQ || Stack_op[i - 1] == REQ);
		flag = ((flag_x && Stack_op[i - 1] == UEQ) || (flag_y && Stack_op[i] == UEQ));
	}
	if (flag) puts ("warning: suggest parentheses around comparison in operand of ‘!=’");
}

bool Check_Parentheses ()
{
	int i, flag = 0;
	memset (match, 0, sizeof (match));
	memset (Parentheses, 0, sizeof (Parentheses));
	for (i = 0; i < nr_token; ++i) 
	{
		if (tokens[i].type == '(') Parentheses[++flag] = i;
		if (tokens[i].type == ')') 
		{
			match[Parentheses[flag]] = i;
			--flag;
		}
		if (flag < 0) return true;
	}
	return false;
}

uint32_t expr(char *e, bool *success) {
	Stack_top = 0;
	if(!make_token(e)) 
	{
		*success = false;
		return 0;
	}
	if (Check_Parentheses ()) // check if parentheses matched first
	{
		*success = false; 
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	//	panic("please implement me");
	uint32_t ans = eval(0, nr_token - 1, success);
	if (*success) warning ();
	return ans;
}

