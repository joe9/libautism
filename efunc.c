#include <u.h>
#include <libc.h>
#include <String.h>
#include <bio.h>

#include "autism.h"

int
erfork(int flags)
{
	int n;
	if((n = rfork(flags)) < 0)
		sysfatal("erfork: %r");
	return n;
}


void*
emalloc(ulong sz)
{
	void *v;

	if((v = mallocz(sz, 1)) == nil)
		error("emalloc: %r");

	setmalloctag(v, getcallerpc(&sz));
	return v;
}

char*
esmprint(char *fmt, ...)
{
	char *s;
	va_list arg;
	
	va_start(arg, fmt);
	s = vsmprint(fmt, arg);
	va_end(arg);
	if(s == nil)
		sysfatal("esmprint: %r");
	return s;
}

char*
estrdup(char *s)
{
	char *p;

	p = strdup(s);
	if(p == nil)
		error("estrdup: %r");
	setmalloctag(p, getcallerpc(&s));
	return p;
}


int
eopen(char *s, int m)
{
	int fd;
	if((fd = open(s, m)) < 0)
		sysfatal("eopen: %r");

	return fd;
}

vlong
eseek(int fd, vlong n, int type)
{
	vlong o;

	o = seek(fd, n, type);
	if(o < 0)
		sysfatal("eseek: %r");

	return o;
}


double
eatof(char *cp)
{
	return estrtod(cp, 0);
}

int
eatoi(char *s)
{
	return estrtol(s, nil, 10);
}

long
eatol(char *s)
{
	return estrtol(s, nil, 10);
}

vlong
eatoll(char *s)
{
	return estrtoll(s, nil, 10);
}

double
estrtod(char *as, char **aas)
{
	double n;
	char *p;

	n = strtod(as, &p);
	if(p == as)
		sysfatal("estrtod: bad input '%s'", as);
	else if(aas != nil)
		*aas = p;

	return n;
}

long
estrtol(char *as, char **aas, int base)
{
	long n;
	char *p;

	n = strtol(as, &p, base);
	if(p == as)
		sysfatal("estrtol: bad input '%s'", as);
	else if(aas != nil)
		*aas = p;

	return n;
}

ulong
estrtoul(char *as, char **aas, int base)
{
	ulong n;
	char *p;

	n = strtoul(as, &p, base);
	if(p == as)
		sysfatal("estrtoul: bad input '%s'", as);
	else if(aas != nil)
		*aas = p;

	return n;
}

vlong
estrtoll(char *as, char **aas, int base)
{
	vlong n;
	char *p;

	n = strtoll(as, &p, base);
	if(p == as)
		sysfatal("estrtoll: bad input '%s'", as);
	else if(aas != nil)
		*aas = p;

	return n;
}

uvlong
estrtoull(char *as, char **aas, int base)
{
	uvlong n;
	char *p;

	n = strtoull(as, &p, base);
	if(p == as)
		sysfatal("estrtoull: bad input '%s'", as);
	else if(aas != nil)
		*aas = p;

	return n;
}

/* below utilities from acme/wiki/src/util.c */

char *
estrstrdup(char *s, char *t)
{
	char *u = emalloc(strlen(s) + strlen(t) + 1);

	strcpy(u, s);
	strcat(u, t);
	return u;
}

char *
eappend(char *s, char *sep, char *t)
{
	char *u;

	if(t == nil)
		u = estrstrdup(s, sep);
	else {
		u = emalloc(strlen(s) + strlen(sep) + strlen(t) + 1);
		strcpy(u, s);
		strcat(u, sep);
		strcat(u, t);
	}
	free(s);
	return u;
}

char *
egrow(char *s, char *sep, char *t)
{
	s = eappend(s, sep, t);
	free(t);
	return s;
}

void
error(char *fmt, ...)
{
	int n;
	va_list arg;
	char buf[1024] = "";

	va_start(arg, fmt);
	n = vseprint(buf, buf + sizeof buf, fmt, arg) - buf;
	va_end(arg);
	write(2, buf, n);
	write(2, "\n", 1);
	/* threadexitsall(buf); */
	abort();
}

/* from spew of #cat-v
   usage of esnprintf:
static size_t
fmtGuardRecursive(char *buf, size_t sz, Node *g, struct GuardFmt *gfmt)
{
    size_t r;

    if (g == NULL) return 0;

    switch(g->type) {
    default:
	abort();    // Impossible type for a guard node.
    case TSYM:
	r  = fmtPrintf(buf, sz, "this->%L(evt, data, %E, %E)", g->sym, gfmt->src, gfmt->dst);
	break;
    case TAND:
	r  = fmtGuardRecursive(buf,   sz,   g->left, gfmt);
	r += esnprintf(buf+r, sz-r, " && ");
	r += fmtGuardRecursive(buf+r, sz-r, g->right, gfmt);
	break;
    case TOR:
	r  = fmtGuardRecursive(buf,   sz,   g->left, gfmt);
	r += esnprintf(buf+r, sz-r, " || ");
	r += fmtGuardRecursive(buf+r, sz-r, g->right, gfmt);
	break;
    case TPAREN:
	r  = esnprintf(buf,   sz,   "(");
	r += fmtGuardRecursive(buf+r, sz-r, g->left, gfmt);
	r += esnprintf(buf+r, sz-r, ")");
	break;
    case TNOT:
	r  = esnprintf(buf,   sz,   "!" );
	r += fmtGuardRecursive(buf+r, sz-r, g->left, gfmt);
	break;
    }
    return r;
}
 */
int
esnprint(char *buf, int sz, const char *fmt, ...)
{
	int r;
	va_list va;

	if(sz <= 0)
		error("esnprint: buf too small, invalid size is %d", sz);
	va_start(va, fmt);
	r = vsnprint(buf, sz, fmt, va);
	va_end(va);
	if(r >= sz)
		error("esnprint: buf too small, size is %d need %d", sz,
		      r);
	return r;
}

char *
eseprint(char *buf, char *ebuf, const char *fmt, ...)
{
	char *r;
	va_list va;

	if(buf >= ebuf)
		error("esnprint: buf too small, start=%p, end=%p", buf,
		      ebuf);
	va_start(va, fmt);
	r = vseprint(buf, ebuf, fmt, va);
	va_end(va);
	if(r >= ebuf)
		error("esnprint: buf too small, start=%p, end=%p", buf, r);
	return r;
}
