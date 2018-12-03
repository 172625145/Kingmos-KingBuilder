#ifndef BISON_BUILD_TAB_HPP
# define BISON_BUILD_TAB_HPP

#ifndef YYSTYPE
typedef union {
unsigned int	uival;
char			*lpstr;
struct words	*lpwds;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
# define	TOKEN_NEWLINE	257
# define	TOKEN_EQUAL	258
# define	TOKEN_VALUE	259
# define	TOKEN_NULL	260


extern YYSTYPE ambolval;

#endif /* not BISON_BUILD_TAB_HPP */
