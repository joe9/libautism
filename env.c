#include <u.h>
#include <libc.h>
#include <String.h>
#include <bio.h>

#include "autism.h"

/* below from /sys/src/cmd/init.c
   usage: cpenv("/adm/timezone/US_Central", "#e/timezone");
   */
char*
readfile(char *name)
{
	int f, len;
	Dir *d;
	char *val;

	f = open(name, OREAD);
	if(f < 0){
		fprint(2, "init: can't open %s: %r\n", name);
		return nil;	
	}
	d = dirfstat(f);
	if(d == nil){
		fprint(2, "init: can't stat %s: %r\n", name);
		close(f);
		return nil;
	}
	len = d->length;
	free(d);
	if(len == 0)	/* device files can be zero length but have contents */
		len = 64;
	val = malloc(len+1);
	if(val == nil){
		fprint(2, "init: can't malloc %s: %r\n", name);
		close(f);
		return nil;
	}
	len = read(f, val, len);
	close(f);
	if(len < 0){
		fprint(2, "init: can't read %s: %r\n", name);
		return nil;
	}else
		val[len] = '\0';
	return val;
}

void
setenv(char *name, char *val)
{
	int fd;

	fd = create(name, OWRITE, 0644);
	if(fd < 0)
		fprint(2, "init: can't create %s: %r\n", name);
	else{
		write(fd, val, strlen(val));
		close(fd);
	}
}

void
cpenv(char *from, char *to)
{
	char *val;

	val = readfile(from);
	if(val != nil){
		setenv(to, val);
		free(val);
	}
}
