typedef union {
unsigned int	uival;
char			*lpstr;
struct words	*lpwds;
} YYSTYPE;
#define	TOKEN_NEWLINE	258
#define	TOKEN_EQUAL	259
#define	TOKEN_VALUE	260
#define	TOKEN_NULL	261


extern YYSTYPE ambolval;
