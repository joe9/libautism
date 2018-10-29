#include <u.h>
#include <libc.h>

#include "autism.h"

void*
fget(int fd, ulong bs, int *p)
{
	char *a;
	ulong b;
	int i, n, r;

	if(bs == 0)
		bs = 8192;

	if((r = iounit(fd)) <= 0)
		r = bs;
	else if(bs < r)
		bs = r;

	a = nil;
	b=*p = 0;
	for(i=0;; i++){
		if(b-*p < r)
			if((a = realloc(a, b+=bs)) == nil)
				return nil;

		if((n = read(fd, a+*p, bs)) <= 0)
			break;
		*p += n;
	}

	if(*p == 0){
		if(n < 0)
			*p = -1;

		if(a != nil){
			/* this should never be reached */
			free(a);
			a = nil;
		}
	}

	return a;
}
