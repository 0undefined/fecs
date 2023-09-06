#ifndef TOKENS_H
#define TOKENS_H

enum Token {
	STRUCT,
	COMPONENT,
	TYPE,
	FUNCTION,
	SYSTEM,

	LPAR,
	RPAR,
	LCURLY,
	RCURLY,
	LBRACK,
	RBRACK,

	COLON,
	EQUAL,

	PLUS,
	MINUS,
	ASTERISK,
	SLASH,
	CONCAT,
	DOT,

	INT,
	FLOAT,
	TRUE,
	FALSE,
	BOOL,
	DATATYPE,

	VNAME,
};

#endif
