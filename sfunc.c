#include <u.h>
#include <libc.h>
#include <String.h>

#include "autism.h"

/* could also use a macro instead of a function
 https://stackoverflow.com/questions/150543/forward-an-invocation-of-a-variadic-function-in-c */
/* this is expensive, stop using it for realtime usage. It is ok for debug operations with show
	but not for writes */
/*  from plan9front/sys/src/libc/fmt/smprint.c   */
String *
s_smprint(String * str, char *fmt, ...)
{
	va_list args;
	char *p;
	String *result;

	if(str == nil)
		result = s_new();
	else
		result = str;

	va_start(args, fmt);
	p = vsmprint(fmt, args);
	va_end(args);
	s_append(result, p);
	free(p);
	return result;
}
