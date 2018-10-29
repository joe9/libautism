#include <u.h>
#include <libc.h>
#include <String.h>
#include <bio.h>

#include "autism.h"

/* the offsets are not helpful when reading tail -f input */
char *
Bgetline(int debug, Biobufhdr * b, int *size)
{
	char *p;
	int sz;
	vlong offset;

	if(b == nil)
		return nil;
	p = Brdline(b, '\n');
	sz = Blinelen(b);
	if(size != nil)
		*size = sz;
	/* proper line, send it out for further processing */
	if(p != nil && sz > 0 && p[sz - 1] == '\n') {
		D(2, "Bgetline: size=%d, line=%.*s\n", sz, sz, p);
		D(2, "Bgetline: size=%d\n", sz);
		return p;
	}
	/* true end of file */
	else if(p == nil && sz == 0)
		return nil;
	/* if line size is bigger than the buffer size */
	else if(p == nil && sz >= b->bsize) {
		offset = Boffset(b);
		error
		    ("fns.c: Bgetline: size %d >= Brdline maximum size %d, offset=%lld, line: %s, errstr:%r\n"
		     "        Bgetline line before offset=%lld worked. The next line failed, use :%lldgo in vim to see the failed line",
		     sz, b->bsize, offset, p, offset, offset + 1);
	} else if(p == nil) {
		/* end of file before a delimiter, such as a partial line */
		/* tail /usr/glenda/ddf/messages/20180406.messages | xd -c */
		offset = Boffset(b);
		fprint(2,
		       "fns.c: Bgetline ignoring partial line p==nil offset=%lld, status=%lld, line: %s, size: %d errstr: %r",
		       offset, (long long) p, p, sz);
		return nil;
	} else {
		/* should never be happening, error out */
		offset = Boffset(b);
		error
		    ("fns.c: Bgetline unexpected error p!=nil offset=%lld, status=%lld, line: %s, sz: %d errstr: %r",
		     offset, (long long) p, p, sz);
	}
	return nil;
}

