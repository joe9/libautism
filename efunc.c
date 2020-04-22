#include <u.h>
#include <libc.h>
#include <String.h>
#include <bio.h>

#include "autism.h"

extern s8 *argv0;

s32
erfork(int flags)
{
	s32 n;

	if((n = rfork(flags)) < 0)
		sysfatal("erfork: %r");
	return n;
}

void *
emalloc(ulong sz)
{
	void *v;

	if((v = mallocz(sz, 1)) == nil)
		error("emalloc: %r");

	setmalloctag(v, getcallerpc(&sz));
	return v;
}

s8 *
esmprint(s8 *fmt, ...)
{
	s8 *s;
	va_list arg;

	va_start(arg, fmt);
	s = vsmprint(fmt, arg);
	va_end(arg);
	if(s == nil)
		sysfatal("esmprint: %r");
	return s;
}

s8 *
estrdup(s8 *s)
{
	s8 *p;

	p = strdup(s);
	if(p == nil)
		error("estrdup: %r");
	setmalloctag(p, getcallerpc(&s));
	return p;
}

s32
eopen(s8 *s, s32 m)
{
	s32 fd;

	if((fd = open(s, m)) < 0)
		sysfatal("eopen: %r");

	return fd;
}

s64
eseek(int fd, s64 n, s32 type)
{
	s64 o;

	o = seek(fd, n, type);
	if(o < 0)
		sysfatal("eseek: %r");

	return o;
}

double
eatof(s8 *cp)
{
	return estrtod(cp, 0);
}

s32
eatoi(s8 *s)
{
	return estrtol(s, nil, 10);
}

s32
eatol(s8 *s)
{
	return estrtol(s, nil, 10);
}

s64
eatoll(s8 *s)
{
	return estrtoll(s, nil, 10);
}

double
estrtod(s8 *as, s8 **aas)
{
	double n;
	s8 *p;

	n = strtod(as, &p);
	if(p == as)
		sysfatal("estrtod: bad input '%s'", as);
	else if(aas != nil)
		*aas = p;

	return n;
}

s32
estrtol(s8 *as, s8 **aas, s32 base)
{
	long n;
	s8 *p;

	n = strtol(as, &p, base);
	if(p == as)
		sysfatal("estrtol: bad input '%s'", as);
	else if(aas != nil)
		*aas = p;

	return n;
}

u32
estrtoul(s8 *as, s8 **aas, s32 base)
{
	ulong n;
	s8 *p;

	n = strtoul(as, &p, base);
	if(p == as)
		sysfatal("estrtoul: bad input '%s'", as);
	else if(aas != nil)
		*aas = p;

	return n;
}

s64
estrtoll(s8 *as, s8 **aas, s32 base)
{
	s64 n;
	s8 *p;

	n = strtoll(as, &p, base);
	if(p == as)
		sysfatal("estrtoll: bad input '%s'", as);
	else if(aas != nil)
		*aas = p;

	return n;
}

u64
estrtoull(s8 *as, s8 **aas, s32 base)
{
	uvlong n;
	s8 *p;

	n = strtoull(as, &p, base);
	if(p == as)
		sysfatal("estrtoull: bad input '%s'", as);
	else if(aas != nil)
		*aas = p;

	return n;
}

/* below utilities from acme/wiki/src/util.c */

s8 *
estrstrdup(s8 *s, s8 *t)
{
	s8 *u = emalloc(strlen(s) + strlen(t) + 1);

	strcpy(u, s);
	strcat(u, t);
	return u;
}

s8 *
eappend(s8 *s, s8 *sep, s8 *t)
{
	s8 *u;

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

s8 *
egrow(s8 *s, s8 *sep, s8 *t)
{
	s = eappend(s, sep, t);
	free(t);
	return s;
}

/* from disk/mkext.c, except for the abort() at the end */
/* assumes argv0 is set */
#pragma varargck argpos error 1
void
error(s8 *fmt, ...)
{
	s8 buf[1024];
	va_list arg;

	sprint(buf, "%s: ", argv0);
	va_start(arg, fmt);
	vseprint(buf + strlen(buf), buf + sizeof(buf), fmt, arg);
	va_end(arg);
	fprint(2, "%s\n", buf);
	/* exits(0); */
	abort();
}

#pragma varargck argpos warn 1
void
warn(s8 *fmt, ...)
{
	s8 buf[1024];
	va_list arg;

	sprint(buf, "%s: ", argv0);
	va_start(arg, fmt);
	vseprint(buf + strlen(buf), buf + sizeof(buf), fmt, arg);
	va_end(arg);
	fprint(2, "%s\n", buf);
}

/* from spew of #cat-v
   usage of esnprintf:
static size_t
fmtGuardRecursive(s8 *buf, size_t sz, Node *g, struct GuardFmt *gfmt)
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
s32
esnprint(s8 *buf, s32 sz, const s8 *fmt, ...)
{
	s32 r;
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

s8 *
eseprint(s8 *buf, s8 *ebuf, const s8 *fmt, ...)
{
	s8 *r;
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
